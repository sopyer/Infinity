#include "pp.h"
#include <framework.h>
#include <ml.h>
#include <graphics.h>

static const size_t MAX_SAMPLE_COUNT = 64;

struct FilterDesc
{
    int   sampleCount;
    int   pad[3];
    v128  weights[MAX_SAMPLE_COUNT];
    v128  offsets[MAX_SAMPLE_COUNT];
};

const char* programFSPath[PRG_ID_COUNT] = 
{
    "PP.Luminance.frag"                  ,
    "PP.Filter.frag"                     ,
    "PP.GuidedFilter.Pack.frag"          ,
    "PP.GuidedFilter.ab.frag"            ,
    "PP.GuidedFilter.aI+b.frag"          ,
    "PP.ColorGuidedFilter.PackVarI.frag" ,
    "PP.ColorGuidedFilter.PackVarIp.frag",
    "PP.ColorGuidedFilter.ab.frag"       ,
    "PP.ColorGuidedFilter.aI+b.frag"     ,
};

GLuint programs[PRG_ID_COUNT];
GLuint samLinearClamp;
GLuint fbo;

void ppInit()
{
    glGenFramebuffers(1, &fbo);

    GLuint vert = resources::createShaderFromFile(GL_VERTEX_SHADER, "Mesh.ScreenTri.UV.vert");

    for (size_t i=0; i<PRG_ID_COUNT; ++i)
    {
        GLuint frag;

        frag        = resources::createShaderFromFile(GL_FRAGMENT_SHADER, programFSPath[i]);
        programs[i] = resources::linkProgram(2, vert, frag);
        glDeleteShader(frag);

        CHECK_GL_ERROR();
    }

    glDeleteShader(vert);

    ppOnShaderReload();

    glGenSamplers(1, &samLinearClamp);
    glSamplerParameteri(samLinearClamp, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(samLinearClamp, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(samLinearClamp, GL_TEXTURE_WRAP_S,     GL_CLAMP );
    glSamplerParameteri(samLinearClamp, GL_TEXTURE_WRAP_T,     GL_CLAMP );

    CHECK_GL_ERROR();
}

void ppOnShaderReload()
{
    GLint structSize;
    GLint uniFilterDesc;
  
    uniFilterDesc  = glGetUniformBlockIndex(programs[PRG_FILTER], "uFilterDesc");

    glGetActiveUniformBlockiv(programs[PRG_FILTER], uniFilterDesc, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
    assert(structSize==sizeof(FilterDesc));
}

void ppFini()
{
    for (size_t i=0; i<PRG_ID_COUNT; ++i)
        glDeleteProgram(programs[i]);

    glDeleteSamplers(1, &samLinearClamp);
    glDeleteFramebuffers(1, &fbo);
}

void ppBegin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glBindVertexArray(vf::empty_geom_t::vao);

    GLuint samplers[4] = {samLinearClamp, samLinearClamp, samLinearClamp, samLinearClamp};
    glBindSamplers(0, ARRAY_SIZE(samplers), samplers);
}

void ppEnd()
{
    GLuint samplers[4] = {0, 0, 0, 0};
    glBindSamplers(0, ARRAY_SIZE(samplers), samplers);

    glBindVertexArray(0);
}

void convertToLuminance(GLuint dst, GLuint src, GLsizei w, GLsizei h)
{
    GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
    glBindTextures(0, 1, &src);

    glUseProgram(programs[PRG_LUM]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst, 0);
    glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    CHECK_GL_ERROR();
}

void generateSinglePassBoxFilterBruteForce(FilterDesc* desc, int kernelWidth, float texelSizeU, float texelSizeV)
{
    assert(kernelWidth%2==1);
    desc->sampleCount = kernelWidth*kernelWidth;

    v128 weight = vi_set_f000(1.0f/kernelWidth/kernelWidth);
    for(int i=0; i<kernelWidth*kernelWidth; ++i)
    {
        vi_store_v4(desc->weights+i, weight);
    }

    float left  = -(float)(kernelWidth/2);
    float right =  (float)(kernelWidth/2+1);

    ml::vec2 iter;
    ml::vec2 texelSize        = {texelSizeU, texelSizeV};
    v128     vecTexelSize     = vi_load_v2(&texelSize);
    v128*    data             = desc->offsets;
    for(iter.y = left; iter.y < right; iter.y += 1.0f)
    {
        for(iter.x = left; iter.x < right; iter.x += 1.0f)
        {
            v128 offset = vi_mul(vi_load_v2(&iter), vecTexelSize);
            vi_store_v4(data++, offset);
        }
    }
}

void generateBoxFilterSinglePass(FilterDesc* desc, int kernelWidth, float texelSizeU, float texelSizeV)
{
    assert(kernelWidth%2 == 1);
    assert(kernelWidth > 0);

    int   kernelWidthM1 = kernelWidth - 1;
    int   samples4      = kernelWidthM1 * kernelWidthM1 / 4;
    int   samples2      = kernelWidthM1;

    desc->sampleCount = samples4 + samples2 + 1;

    // Calculate kernel weights
    v128* weightPtr = desc->weights;

    v128 weight4 = vi_set_f000(4.0f/kernelWidth/kernelWidth);
    for(int i=0; i<samples4; ++i, ++weightPtr)
    {
        vi_store_v4(weightPtr, weight4);
    }

    v128 weight2 = vi_set_f000(2.0f/kernelWidth/kernelWidth);
    for (int i=0; i<samples2; ++i, ++weightPtr)
    {
        vi_store_v4(weightPtr, weight2);
    }

    v128 weight1 = vi_set_f000(1.0f/kernelWidth/kernelWidth);
    vi_store_v4(weightPtr, weight1);


    // Calculate bilinear sampling offsets
    ml::vec2 iter;
    ml::vec2 texelSize    = {texelSizeU, texelSizeV};
    v128     vecTexelSize = vi_load_v2(&texelSize);

    float left  = -(float)(kernelWidth / 2);
    float right =  (float)(kernelWidth / 2);

    v128* offsetPtr = desc->offsets;

    v128 sampleOffset4 = vi_set(0.5f*texelSizeU, 0.5f*texelSizeV, 0.0f, 0.0f);
    for(iter.y = left; iter.y < right; iter.y += 2.0f)
    {
        for(iter.x = left; iter.x < right; iter.x += 2.0f)
        {
            v128 offset4 = vi_mad(vi_load_v2(&iter), vecTexelSize, sampleOffset4);
            vi_store_v4(offsetPtr++, offset4);
        }
    }

    v128 sampleOffset2y = vi_set(0.0f, 0.5f*texelSizeV, 0.0f, 0.0f);
    for(iter.y = left; iter.y < right; iter.y += 2.0f)
    {
        v128 offset2 = vi_mad(vi_load_v2(&iter), vecTexelSize, sampleOffset2y);
        vi_store_v4(offsetPtr++, offset2);
    }

    v128 sampleOffset2x = vi_set(0.5f*texelSizeU, 0.0f, 0.0f, 0.0f);
    for(iter.x = left; iter.x < right; iter.x += 2.0f)
    {
        v128 offset2 = vi_mad(vi_load_v2(&iter), vecTexelSize, sampleOffset2x);
        vi_store_v4(offsetPtr++, offset2);
    }

    v128 offset1 = vi_mul(vi_load_v2(&iter), vecTexelSize);
    vi_store_v4(offsetPtr++, offset1);
}

void generateBoxFilterHPass(FilterDesc* desc, int kernelWidth, float texelSizeU, float texelSizeV)
{
    assert(kernelWidth%2 == 1);
    assert(kernelWidth > 0);

    int   kernelWidthM1 = kernelWidth - 1;
    int   samples2      = kernelWidthM1 / 2;

    desc->sampleCount = samples2 + 1;

    // Calculate kernel weights
    v128* weightPtr = desc->weights;

    v128 weight2 = vi_set_f000(2.0f/kernelWidth);
    for (int i=0; i<samples2; ++i, ++weightPtr)
    {
        vi_store_v4(weightPtr, weight2);
    }

    v128 weight1 = vi_set_f000(1.0f/kernelWidth);
    vi_store_v4(weightPtr, weight1);


    // Calculate bilinear sampling offsets
    ml::vec2 iter;
    ml::vec2 texelSize    = {texelSizeU, texelSizeV};
    v128     vecTexelSize = vi_load_v2(&texelSize);

    float left  = -(float)(kernelWidth / 2);
    float right =  (float)(kernelWidth / 2);

    v128* offsetPtr = desc->offsets;

    iter.y = 0.0f;
    v128 sampleOffset2x = vi_set(0.5f*texelSizeU, 0.0f, 0.0f, 0.0f);
    for(iter.x = left; iter.x < right; iter.x += 2.0f)
    {
        v128 offset2 = vi_mad(vi_load_v2(&iter), vecTexelSize, sampleOffset2x);
        vi_store_v4(offsetPtr++, offset2);
    }

    v128 offset1 = vi_mul(vi_load_v2(&iter), vecTexelSize);
    vi_store_v4(offsetPtr++, offset1);
}

void generateBoxFilterVPass(FilterDesc* desc, int kernelWidth, float texelSizeU, float texelSizeV)
{
    assert(kernelWidth%2 == 1);
    assert(kernelWidth > 0);

    int   kernelWidthM1 = kernelWidth - 1;
    int   samples2      = kernelWidthM1 / 2;

    desc->sampleCount = samples2 + 1;

    // Calculate kernel weights
    v128* weightPtr = desc->weights;

    v128 weight2 = vi_set_f000(2.0f/kernelWidth);
    for (int i=0; i<samples2; ++i, ++weightPtr)
    {
        vi_store_v4(weightPtr, weight2);
    }

    v128 weight1 = vi_set_f000(1.0f/kernelWidth);
    vi_store_v4(weightPtr, weight1);


    // Calculate bilinear sampling offsets
    ml::vec2 iter;
    ml::vec2 texelSize    = {texelSizeU, texelSizeV};
    v128     vecTexelSize = vi_load_v2(&texelSize);

    float left  = -(float)(kernelWidth / 2);
    float right =  (float)(kernelWidth / 2);

    v128* offsetPtr = desc->offsets;

    iter.x = 0.0f;
    v128 sampleOffset2y = vi_set(0.0f, 0.5f*texelSizeV, 0.0f, 0.0f);
    for(iter.y = left; iter.y < right; iter.y += 2.0f)
    {
        v128 offset2 = vi_mad(vi_load_v2(&iter), vecTexelSize, sampleOffset2y);
        vi_store_v4(offsetPtr++, offset2);
    }

    v128 offset1 = vi_mul(vi_load_v2(&iter), vecTexelSize);
    vi_store_v4(offsetPtr++, offset1);
}

void boxFilter1Pass(GLuint dst, GLuint src, int kernelWidth, GLsizei w, GLsizei h)
{
    FilterDesc* desc;
    GLuint      offset;
    GLenum      buffers[] = {GL_COLOR_ATTACHMENT0};

    glUseProgram(programs[PRG_FILTER]);

    glBindTextures(0, 1, &src);

    desc = (FilterDesc*)gfx::dynbufAllocMem(sizeof(FilterDesc), gfx::caps.uboAlignment, &offset);
    generateBoxFilterSinglePass(desc, kernelWidth, 1.0f/w, 1.0f/h);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, gfx::dynBuffer, offset, sizeof(FilterDesc));

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst, 0);
    glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    CHECK_GL_ERROR();
}

void boxFilter2Pass(GLuint dst, GLuint src, GLuint tmp, int kernelWidth, GLsizei w, GLsizei h)
{
    FilterDesc* desc;
    GLuint      offset;
    GLenum      buffers[] = {GL_COLOR_ATTACHMENT0};

    glUseProgram(programs[PRG_FILTER]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tmp, 0);
    glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glBindTextures(0, 1, &src);

    desc = (FilterDesc*)gfx::dynbufAllocMem(sizeof(FilterDesc), gfx::caps.uboAlignment, &offset);
    generateBoxFilterHPass(desc, kernelWidth, 1.0f/w, 1.0f/h);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, gfx::dynBuffer, offset, sizeof(FilterDesc));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst, 0);
    glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glBindTextures(0, 1, &tmp);

    desc = (FilterDesc*)gfx::dynbufAllocMem(sizeof(FilterDesc), gfx::caps.uboAlignment, &offset);
    generateBoxFilterVPass(desc, kernelWidth, 1.0f/w, 1.0f/h);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, gfx::dynBuffer, offset, sizeof(FilterDesc));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    CHECK_GL_ERROR();
}

