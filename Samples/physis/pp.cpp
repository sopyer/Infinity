#include "pp.h"
#include <framework.h>
#include <utils/ml.h>

static const size_t MAX_SAMPLE_COUNT = 64;

struct ProgramDesc
{
    GLuint      id;
    const char* srcPath; 
};

struct FilterDesc
{
    int   sampleCount;
    int   pad[3];
	v128  weights[MAX_SAMPLE_COUNT];
	v128  offsets[MAX_SAMPLE_COUNT];
};

const ProgramDesc prgDesc[] = 
{
    {PRG_LUM,                     "PP.Luminance.frag"                  },
    {PRG_FILTER,                  "PP.Filter.frag"                     },
    {PRG_GUIDED_PACK,             "PP.GuidedFilter.Pack.frag"          },
    {PRG_GUIDED_AB,               "PP.GuidedFilter.ab.frag"            },
    {PRG_GUIDED_AI_B,             "PP.GuidedFilter.aI+b.frag"          },
    {PRG_COLOR_GUIDED_PACK_VARI,  "PP.ColorGuidedFilter.PackVarI.frag" },
    {PRG_COLOR_GUIDED_PACK_VARIP, "PP.ColorGuidedFilter.PackVarIp.frag"},
    {PRG_COLOR_GUIDED_AB,         "PP.ColorGuidedFilter.ab.frag"       },
    {PRG_COLOR_GUIDED_AI_B,       "PP.ColorGuidedFilter.aI+b.frag"     },
};


static const float triVertices[6] = {
    -1,  1, 3,  1, -1, -3,
};

GLuint programs[PRG_ID_COUNT];
GLint  uniEPS;
GLint  uniEPS3ch;
GLuint ubo;
GLint  uboAlignment;
GLuint samLinearClamp;
GLuint fbo;

void ppInit()
{
    glGenFramebuffers(1, &fbo);

    // Assumption - align is power of 2
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uboAlignment);
    --uboAlignment;


    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 
                    (sizeof(FilterDesc)|uboAlignment)+1,
                    0, GL_DYNAMIC_DRAW);

    GLuint vert = resources::createShaderFromFile(GL_VERTEX_SHADER, "PP.common.vert");

    for (size_t i=0; i<ARRAY_SIZE(prgDesc); ++i)
    {
        const  ProgramDesc&  desc = prgDesc[i];
        GLuint frag;
            
        frag              = resources::createShaderFromFile(GL_FRAGMENT_SHADER, desc.srcPath);
        programs[desc.id] = resources::linkProgram(2, vert, frag);
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
    uniEPS         = glGetUniformLocation(programs[PRG_GUIDED_AB],    "uEPS");
    uniEPS3ch      = glGetUniformLocation(programs[PRG_COLOR_GUIDED_AB], "uEPS");

    glGetActiveUniformBlockiv(programs[PRG_FILTER], uniFilterDesc, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
    assert(structSize==sizeof(FilterDesc));
}

void ppFini()
{
    for (size_t i=0; i<PRG_ID_COUNT; ++i)
        glDeleteProgram(programs[i]);
    glDeleteBuffers(1, &ubo);
    glDeleteSamplers(1, &samLinearClamp);
    glDeleteFramebuffers(1, &fbo);
}

void ppBegin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, triVertices);

    glBindSampler(0, samLinearClamp);
    glBindSampler(1, samLinearClamp);
    glBindSampler(2, samLinearClamp);
    glBindSampler(3, samLinearClamp);
}

void ppEnd()
{
    glBindSampler(0, 0);
    glBindSampler(1, 0);
    glBindSampler(2, 0);
    glBindSampler(3, 0);

    glActiveTexture(GL_TEXTURE0);
    glDisableVertexAttribArray(0);
}

void convertToLuminance(GLuint dst, GLuint src, GLsizei w, GLsizei h)
{
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, src);

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
        
    v128 weight = vi_set_x000(1.0f/kernelWidth/kernelWidth);
    for(int i=0; i<kernelWidth*kernelWidth; ++i)
    {
        vi_store(desc->weights+i, weight);
    }
        
    float left  = -(float)(kernelWidth/2);
    float right =  (float)(kernelWidth/2+1);

    ml::vec2 iter;
    ml::vec2 texelSize        = {texelSizeU, texelSizeV};
    v128     vecTexelSize     = ml::load_vec2(&texelSize);
    v128*    data             = desc->offsets;
    for(iter.y = left; iter.y < right; iter.y += 1.0f)
    {
        for(iter.x = left; iter.x < right; iter.x += 1.0f)
        {
            v128 offset = vi_mul(ml::load_vec2(&iter), vecTexelSize);
            vi_store(data++, offset);
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

    v128 weight4 = vi_set_x000(4.0f/kernelWidth/kernelWidth);
    for(int i=0; i<samples4; ++i, ++weightPtr)
    {
        vi_store(weightPtr, weight4);
    }

    v128 weight2 = vi_set_x000(2.0f/kernelWidth/kernelWidth);
    for (int i=0; i<samples2; ++i, ++weightPtr)
    {
        vi_store(weightPtr, weight2);
    }
        
    v128 weight1 = vi_set_x000(1.0f/kernelWidth/kernelWidth);
    vi_store(weightPtr, weight1);
        

    // Calculate bilinear sampling offsets
    ml::vec2 iter;
    ml::vec2 texelSize    = {texelSizeU, texelSizeV};
    v128     vecTexelSize = ml::load_vec2(&texelSize);

    float left  = -(float)(kernelWidth / 2);
    float right =  (float)(kernelWidth / 2);

    v128* offsetPtr = desc->offsets;

    v128 sampleOffset4 = vi_set(0.5f*texelSizeU, 0.5f*texelSizeV, 0.0f, 0.0f);
    for(iter.y = left; iter.y < right; iter.y += 2.0f)
    {
        for(iter.x = left; iter.x < right; iter.x += 2.0f)
        {
            v128 offset4 = vi_mad(ml::load_vec2(&iter), vecTexelSize, sampleOffset4);
            vi_store(offsetPtr++, offset4);
        }
    }
        
    v128 sampleOffset2y = vi_set(0.0f, 0.5f*texelSizeV, 0.0f, 0.0f);
    for(iter.y = left; iter.y < right; iter.y += 2.0f)
    {
        v128 offset2 = vi_mad(ml::load_vec2(&iter), vecTexelSize, sampleOffset2y);
        vi_store(offsetPtr++, offset2);
    }

    v128 sampleOffset2x = vi_set(0.5f*texelSizeU, 0.0f, 0.0f, 0.0f);
    for(iter.x = left; iter.x < right; iter.x += 2.0f)
    {
        v128 offset2 = vi_mad(ml::load_vec2(&iter), vecTexelSize, sampleOffset2x);
        vi_store(offsetPtr++, offset2);
    }

    v128 offset1 = vi_mul(ml::load_vec2(&iter), vecTexelSize);
    vi_store(offsetPtr++, offset1);
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

    v128 weight2 = vi_set_x000(2.0f/kernelWidth);
    for (int i=0; i<samples2; ++i, ++weightPtr)
    {
        vi_store(weightPtr, weight2);
    }
        
    v128 weight1 = vi_set_x000(1.0f/kernelWidth);
    vi_store(weightPtr, weight1);
        

    // Calculate bilinear sampling offsets
    ml::vec2 iter;
    ml::vec2 texelSize    = {texelSizeU, texelSizeV};
    v128     vecTexelSize = ml::load_vec2(&texelSize);

    float left  = -(float)(kernelWidth / 2);
    float right =  (float)(kernelWidth / 2);

    v128* offsetPtr = desc->offsets;

    iter.y = 0.0f;
    v128 sampleOffset2x = vi_set(0.5f*texelSizeU, 0.0f, 0.0f, 0.0f);
    for(iter.x = left; iter.x < right; iter.x += 2.0f)
    {
        v128 offset2 = vi_mad(ml::load_vec2(&iter), vecTexelSize, sampleOffset2x);
        vi_store(offsetPtr++, offset2);
    }

    v128 offset1 = vi_mul(ml::load_vec2(&iter), vecTexelSize);
    vi_store(offsetPtr++, offset1);
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

    v128 weight2 = vi_set_x000(2.0f/kernelWidth);
    for (int i=0; i<samples2; ++i, ++weightPtr)
    {
        vi_store(weightPtr, weight2);
    }
        
    v128 weight1 = vi_set_x000(1.0f/kernelWidth);
    vi_store(weightPtr, weight1);
        

    // Calculate bilinear sampling offsets
    ml::vec2 iter;
    ml::vec2 texelSize    = {texelSizeU, texelSizeV};
    v128     vecTexelSize = ml::load_vec2(&texelSize);

    float left  = -(float)(kernelWidth / 2);
    float right =  (float)(kernelWidth / 2);

    v128* offsetPtr = desc->offsets;

    iter.x = 0.0f;
    v128 sampleOffset2y = vi_set(0.0f, 0.5f*texelSizeV, 0.0f, 0.0f);
    for(iter.y = left; iter.y < right; iter.y += 2.0f)
    {
        v128 offset2 = vi_mad(ml::load_vec2(&iter), vecTexelSize, sampleOffset2y);
        vi_store(offsetPtr++, offset2);
    }

    v128 offset1 = vi_mul(ml::load_vec2(&iter), vecTexelSize);
    vi_store(offsetPtr++, offset1);
}

void boxFilter1Pass(GLuint dst, GLuint src, int kernelWidth, GLsizei w, GLsizei h)
{
    FilterDesc* desc;
	GLenum      buffers[] = {GL_COLOR_ATTACHMENT0};


    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, src);
          
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    desc = (FilterDesc*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    generateBoxFilterSinglePass(desc, kernelWidth, 1.0f/w, 1.0f/h);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, sizeof(FilterDesc));

    glUseProgram(programs[PRG_FILTER]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst, 0);
	glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    CHECK_GL_ERROR();
}

void boxFilter2Pass(GLuint dst, GLuint src, GLuint tmp, int kernelWidth, GLsizei w, GLsizei h)
{
	FilterDesc* desc;
    GLenum      buffers[] = {GL_COLOR_ATTACHMENT0};


    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, sizeof(FilterDesc));
    glUseProgram(programs[PRG_FILTER]);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);


    desc = (FilterDesc*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    generateBoxFilterHPass(desc, kernelWidth, 1.0f/w, 1.0f/h);
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, src);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tmp, 0);
	glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);


    desc = (FilterDesc*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    generateBoxFilterVPass(desc, kernelWidth, 1.0f/w, 1.0f/h);
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, tmp);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst, 0);
	glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);


    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    CHECK_GL_ERROR();
}

