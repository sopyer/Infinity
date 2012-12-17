#include <framework.h>
#include <vg/VG.h>
#include <ml.h>
#include <utils.h>

#include "Perlin.h"

static const float quadVertices[6] = {
    -1,  1, 3,  1, -1, -3,
};

static const int texSize = 1024;

enum TextureIDs
{
    TEX_PERLIN0 = 0,
    TEX_PERLIN1,
    TEX_PERLIN2,
    TEX_PERLIN3,
    TEX_PERLIN4,
    TEX_PERLIN5,
    TEX_PERLIN6,
    TEX_PERLIN7,
    TEX_TEXTURE_SRT,
    TEX_COMBINED0,
    TEX_COMBINED1,
    TEX_COMBINED2,
    TEX_COMBINED3,
    TEX_COMBINED4,
    TEX_LUMINANCE,
    TEX_TMP0,
    TEX_TMP1,
    TEX_TMP2,
    TEX_TMP3,
    TEX_TMP4,
    TEX_TMP5,
    TEX_TMP6,
    TEX_TMP7,
    TEX_GUIDED_RESULT,

    TEX_ID_COUNT
};

enum ProgramIDs
{
    PRG_LUM,
    PRG_FILTER,
    PRG_GUIDED_PACK,
    PRG_GUIDED_AB,
    PRG_GUIDED_AI_B,

    PRG_ID_COUNT
};

struct ProgramDesc
{
    GLuint      id;
    const char* srcPath; 
};

struct RTDesc
{
    GLuint  id;
    GLsizei width;
    GLsizei height;
    GLenum  internalFmt;
    GLenum  channels;
    GLenum  type;
};

const RTDesc rtDesc[] = 
{
    {TEX_PERLIN0,       texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_PERLIN1,       texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_PERLIN2,       texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_PERLIN3,       texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_PERLIN4,       texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_PERLIN5,       texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_PERLIN6,       texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_PERLIN7,       texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_TEXTURE_SRT,   texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_COMBINED0,     texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_COMBINED1,     texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_COMBINED2,     texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_COMBINED3,     texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_COMBINED4,     texSize, texSize, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_LUMINANCE,         256,     256, GL_RGBA8,        GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_GUIDED_RESULT,     256,     256, GL_RGBA8,        GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_TMP0,              256,     256, GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP1,              256,     256, GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP2,              256,     256, GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP3,              256,     256, GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP4,              256,     256, GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP5,              256,     256, GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP6,              256,     256, GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP7,              256,     256, GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT},
};

const ProgramDesc prgDesc[] = 
{
    {PRG_LUM,         "PP.Luminance.frag"        },
    {PRG_FILTER,      "PP.Filter.frag"           },
    {PRG_GUIDED_PACK, "PP.GuidedFilter.Pack.frag"},
    {PRG_GUIDED_AB,   "PP.GuidedFilter.ab.frag"},
    {PRG_GUIDED_AI_B, "PP.GuidedFilter.aI+b.frag"},
};

class PhysisDemo: public ui::Stage
{

    GLuint textures[TEX_ID_COUNT];
    GLuint programs[PRG_ID_COUNT];
    GLuint fbo;

    GLint vp[4];

    GLuint texGtorProg, perlinGtorProg;

    GLuint permTex[9], gradTex;

    GLint  uniInvTexDim;

    GLint	uniOctaves;
    GLint	uniAmp;
    GLint	uniAmpScale;
    GLint	uniFreq;
    GLint	uniFreqScale;

    GLint   uniEPS;

public:
    void allocTextures()
    {
        glGenTextures(TEX_ID_COUNT, textures);

        for (size_t i=0; i<ARRAY_SIZE(rtDesc); ++i)
        {
            const RTDesc&  desc = rtDesc[i];

            glBindTexture(GL_TEXTURE_2D, textures[desc.id]);
            glTexImage2D(GL_TEXTURE_2D, 0, desc.internalFmt,
                         desc.width, desc.height, 0,
                         desc.channels, desc.type, 0);
            CHECK_GL_ERROR();
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void createPrograms()
    {
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
    };

    void CreatePerlinNoiseGtor()
    {
        perlinGtorProg = resources::createProgramFromFiles(0, NULL, 3,
                             GL_VERTEX_SHADER,   "PP.common.vert",
                             GL_FRAGMENT_SHADER, "Perlin.Noise.frag",
                             GL_FRAGMENT_SHADER, "TextureGtor.Perlin.frag"
                         );

        CHECK_GL_ERROR();

        uniInvTexDim    = glGetUniformLocation(perlinGtorProg, "invTexDim");
        uniOctaves		= glGetUniformLocation(perlinGtorProg, "uOctaves");
        uniAmp			= glGetUniformLocation(perlinGtorProg, "uAmp");
        uniAmpScale		= glGetUniformLocation(perlinGtorProg, "uAmpScale");
        uniFreq			= glGetUniformLocation(perlinGtorProg, "uFreq");
        uniFreqScale	= glGetUniformLocation(perlinGtorProg, "uFreqScale");

        CHECK_GL_ERROR();

        glUseProgram(perlinGtorProg);
        glUniform1f(uniInvTexDim, 1.0f/PERMUTATION_DIM);
        glUseProgram(0);

        CHECK_GL_ERROR();

        //Gen perlin permutation and gradient textures
        glGenTextures(9, permTex);
        for (size_t i=0; i<9; ++i)
        {
            glBindTexture(GL_TEXTURE_1D, permTex[i]);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, PERMUTATION_DIM, 0, GL_RED, GL_UNSIGNED_BYTE, permutation256[i]);
        }

        glGenTextures(1, &gradTex);
        glBindTexture(GL_TEXTURE_1D, gradTex);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, 8, 0, GL_RG, GL_FLOAT, gradients2D[0]);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLint uniSamplerInput, uniRotationScale, uniTranslate;

    GLuint texSource;

    GLuint ubo;

    GLint  uboAlignment;

    static const size_t MAX_SAMPLE_COUNT = 64;

    struct FilterDesc
    {
    	int   sampleCount;
        int   pad[3];
	    v128  weights[MAX_SAMPLE_COUNT];
	    v128  offsets[MAX_SAMPLE_COUNT];
    };

    PhysisDemo():
          mOffsetX(0.0f),
          mOffsetY(0.0f),
          mScale(1.0f),
          mIsDragging(false)
    {
        VFS::mount("AppData");
        VFS::mount("..\\AppData");
        VFS::mount("..\\..\\AppData");

        glGenFramebuffers(1, &fbo);
    
        // Assumption - align is power of 2
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uboAlignment);
        --uboAlignment;


        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, 
                     (sizeof(FilterDesc)|uboAlignment)+1,
                     0, GL_DYNAMIC_DRAW);

        allocTextures();

        glGetIntegerv(GL_VIEWPORT, vp);

        texGtorProg     = resources::createProgramFromFiles(
                              "PP.common.vert",
                              "TextureGtor.Metal.frag"
                          );

        createPrograms();

        GLint structSize;
        GLint uniFilterDesc;
  
        uniFilterDesc  = glGetUniformBlockIndex(programs[PRG_FILTER], "uFilterDesc");
        uniEPS         = glGetUniformLocation(programs[PRG_GUIDED_AB], "uEPS");

        glGetActiveUniformBlockiv(programs[PRG_FILTER], uniFilterDesc, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(FilterDesc));

        texSource  = resources::createTexture2D("coin.dds");

        CHECK_GL_ERROR();

        CreatePerlinNoiseGtor();

        CHECK_GL_ERROR();
    }

    CPUTimer	mCPUTimer;
    GPUTimer	mGPUTimer;

    double mCPUTime;
    double mGPUTime;

    ~PhysisDemo()
    {
        glDeleteTextures(TEX_ID_COUNT, textures);
        for (size_t i=0; i<PRG_ID_COUNT; ++i)
            glDeleteProgram(programs[i]);
        glDeleteBuffers(1, &ubo);
        glDeleteProgram(texGtorProg);
        glDeleteProgram(perlinGtorProg);
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &texSource);
        glDeleteTextures(9, permTex);
        glDeleteTextures(1, &gradTex);
    }

protected:
    void generateTextureCombined(TextureIDs dest)
    {
        CHECK_GL_ERROR();

        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, textures[TEX_PERLIN0]);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, textures[TEX_PERLIN1]);
        glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, textures[TEX_PERLIN2]);
        glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, textures[TEX_PERLIN3]);
        glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, textures[TEX_PERLIN4]);
        glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, textures[TEX_PERLIN5]);
        glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, textures[TEX_PERLIN6]);
        glActiveTexture(GL_TEXTURE7); glBindTexture(GL_TEXTURE_2D, textures[TEX_PERLIN7]);

        glUseProgram(texGtorProg);
        //glUniformMatrix4fv(uniHSCB, 1, false, getHSCB(0.0f, 1.0f, 1.375f, 2.5f));

        generateTexture(textures[dest], texSize);

        CHECK_GL_ERROR();
    }

    void generateTexture(GLuint texID, GLsizei texSize)
    {
		GLenum buffers[] = {GL_COLOR_ATTACHMENT0};

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
		glDrawBuffers(ARRAY_SIZE(buffers), buffers);
        glViewport(0, 0, texSize, texSize);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void generatePerlin(TextureIDs dest, int seed, int octaves, float amp, float ampScale, float freq, float freqScale)
    {
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_1D, permTex[seed%9]);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_1D, gradTex);

        glUseProgram(perlinGtorProg);

        glUniform1i(uniOctaves,   octaves);
        glUniform1f(uniAmp,       amp);
        glUniform1f(uniAmpScale,  ampScale);
        glUniform1f(uniFreq,      freq);
        glUniform1f(uniFreqScale, freqScale);

        generateTexture(textures[dest], texSize);

        CHECK_GL_ERROR();
    }

    glm::mat4 getHSCB(float hue, float saturation, float contrast, float brightness)
    {
        glm::mat4 brightnessMatrix = glm::scale3DGTX(glm::mat4(), brightness, brightness, brightness);
        glm::mat4 contrastMatrix = glm::translate3DGTX(glm::mat4(), 0.5f, 0.5f, 0.5f)*
            glm::scale3DGTX(glm::mat4(), contrast, contrast, contrast)*
            glm::translate3DGTX(glm::mat4(), -0.5f, -0.5f, -0.5f);

        //TODO:add hue and saturation matrices

        glm::mat4 hueMatrix = glm::rotateGTX(hue, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

        const float RED_EYE_SENSIBILITY = 0.3086f;
        const float GREEN_EYE_SENSIBILITY = 0.6094f;
        const float BLUE_EYE_SENSIBILITY = 0.0820f;

        float s = saturation;
        glm::mat4 saturationMatrix = glm::mat4(
            glm::vec4((1.0f-s)*RED_EYE_SENSIBILITY + s, (1.0f-s)*RED_EYE_SENSIBILITY,       (1.0f-s)*RED_EYE_SENSIBILITY,       0.0f),
            glm::vec4((1.0f-s)*GREEN_EYE_SENSIBILITY,   (1.0f-s)*GREEN_EYE_SENSIBILITY + s, (1.0f-s)*GREEN_EYE_SENSIBILITY,     0.0f),
            glm::vec4((1.0f-s)*BLUE_EYE_SENSIBILITY,    (1.0f-s)*BLUE_EYE_SENSIBILITY,      (1.0f-s)*BLUE_EYE_SENSIBILITY + s,  0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
            );

        return brightnessMatrix*contrastMatrix;
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

    //Optimize!!!!!!!!!!!!!!!
    //Create 2 pass version!!!!!!!!!!!!!!!
    //Generate weights for even kernels as well!!!!!!!!!!!
    void generateSinglePassBoxFilter(FilterDesc* desc, int kernelWidth, float texelSizeU, float texelSizeV)
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

    void boxFilter(GLuint dst, GLuint src, GLsizei w, GLsizei h)
    {
		GLenum buffers[] = {GL_COLOR_ATTACHMENT0};

        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, src);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        FilterDesc* desc = (FilterDesc*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
        
        generateSinglePassBoxFilter(desc, 7, 1.0f/w, 1.0f/h);
                
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

    void guidedFilterPack(GLuint pI, GLuint pIII, GLuint src, GLuint guide, GLsizei w, GLsizei h)
    {
		GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, src  );
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, guide);

        glUseProgram(programs[PRG_GUIDED_PACK]);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pI,   0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, pIII, 0);
		glDrawBuffers(ARRAY_SIZE(buffers), buffers);
        glViewport(0, 0, w, h);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    }

    void guidedFilterAB(GLuint A, GLuint B, GLuint src, GLuint guide, float eps, GLsizei w, GLsizei h)
    {
		GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, src  );
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, guide);

        glUseProgram(programs[PRG_GUIDED_AB]);
        glUniform1f(uniEPS, eps);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, A, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, B, 0);
		glDrawBuffers(ARRAY_SIZE(buffers), buffers);
        glViewport(0, 0, w, h);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    }

    void guidedFilterAI_B(GLuint res, GLuint A, GLuint B, GLuint guide, GLsizei w, GLsizei h)
    {
		GLenum buffers[] = {GL_COLOR_ATTACHMENT0};

        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, A    );
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, B    );
        glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, guide);

        glUseProgram(programs[PRG_GUIDED_AI_B]);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, res, 0);
		glDrawBuffers(ARRAY_SIZE(buffers), buffers);
        glViewport(0, 0, w, h);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void guidedFilter(GLuint dst, GLuint src, GLuint guide, GLuint tmp[8], float eps, GLsizei w, GLsizei h)
    {
        guidedFilterPack(tmp[0], tmp[1], src, guide, w, h);
        boxFilter(tmp[2], tmp[0], 256, 256);
        boxFilter(tmp[3], tmp[1], 256, 256);
        guidedFilterAB(tmp[4], tmp[5], tmp[2], tmp[3], eps, w, h);
        boxFilter(tmp[6], tmp[4], 256, 256);
        boxFilter(tmp[7], tmp[5], 256, 256);
        guidedFilterAI_B(dst, tmp[6], tmp[7], guide, w, h);

        CHECK_GL_ERROR();
    }

    virtual void onPaint()
    {
        mCPUTimer.start();
        mGPUTimer.start();

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, quadVertices);

        //generatePerlin(PERLIN0, 8, 6, 1.86f, 0.75f,   4.0f, 2.0f);
        //generatePerlin(PERLIN1, 1, 5, 0.50f, 0.32f,   4.0f, 2.0f);
        //generatePerlin(PERLIN2, 5, 6, 1.86f, 0.75f,   4.0f, 2.0f);
        //generatePerlin(PERLIN3, 5, 1, 1.86f, 0.75f,   4.0f, 2.0f);
        //generatePerlin(PERLIN4, 5, 6, 1.36f, 0.75f,   4.0f, 2.0f);
        //generatePerlin(PERLIN5, 5, 1, 1.36f, 0.75f,   4.0f, 2.0f);
        //generatePerlin(PERLIN6, 0, 6, 0.68f, 1.00f,   8.0f, 2.0f);
        //generatePerlin(PERLIN7, 0, 2, 1.00f, 1.00f, 128.0f, 2.0f);

        //generateTextureCombined(COMBINED0);

        convertToLuminance(textures[TEX_LUMINANCE], texSource, 256, 256);

        boxFilter(textures[TEX_TMP2], textures[TEX_LUMINANCE], 256, 256);
        GLuint tmp[8] = {
            textures[TEX_TMP0], textures[TEX_TMP1],
            textures[TEX_TMP2], textures[TEX_TMP3],
            textures[TEX_TMP4], textures[TEX_TMP5],
            textures[TEX_TMP6], textures[TEX_TMP7],
        };
        guidedFilter(textures[TEX_GUIDED_RESULT], texSource, textures[TEX_LUMINANCE], tmp, 0.16f, 256, 256);

        glDisableVertexAttribArray(0);

        mGPUTimer.stop();
        mCPUTime = mCPUTimer.elapsed();
        mGPUTime = mGPUTimer.getResult();
        mCPUTimer.stop();

        CHECK_GL_ERROR();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(vp[0], vp[1], vp[2], vp[3]);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(0, 0, -10);

        vg::drawImage( -8,  2, -6,  0, textures[TEX_PERLIN0]);
        vg::drawImage( -5,  2, -3,  0, textures[TEX_PERLIN1]);
        vg::drawImage( -2,  2,  0,  0, textures[TEX_PERLIN2]);
        vg::drawImage(  1,  2,  3,  0, textures[TEX_PERLIN3]);
        vg::drawImage(  4,  2,  6,  0, textures[TEX_PERLIN4]);
        vg::drawImage(  7,  2,  9,  0, textures[TEX_PERLIN5]);

        vg::drawImage( -8, -1, -6, -3, textures[TEX_PERLIN6]);
        vg::drawImage( -5, -1, -3, -3, textures[TEX_PERLIN7]);
        vg::drawImage( -2, -1,  0, -3, textures[TEX_COMBINED0]);

        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(mOffsetX, mOffsetY, 0);
        glScalef(mScale, mScale, 1);

        GLint swizzleAAA1[4] = {GL_ALPHA, GL_ALPHA, GL_ALPHA, GL_ONE};
        GLint swizzleRGBA[4] = {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA};

        vg::drawImage(  0.0f,   0.0f,   0.0f+256.0f, 256.0f, textures[TEX_TMP0]);
        glBindTexture(GL_TEXTURE_2D, textures[TEX_TMP0]);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleAAA1);
        vg::drawImage(300.0f,   0.0f, 300.0f+256.0f, 256.0f, textures[TEX_TMP0]);
        glBindTexture(GL_TEXTURE_2D, textures[TEX_TMP0]);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRGBA);

        vg::drawImage(600.0f, 0.0f, 600.0f+256.0f, 256.0f, textures[TEX_TMP1]);
        glBindTexture(GL_TEXTURE_2D, textures[TEX_TMP1]);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleAAA1);
        vg::drawImage(900.0f, 0.0f, 900.0f+256.0f, 256.0f, textures[TEX_TMP1]);
        glBindTexture(GL_TEXTURE_2D, textures[TEX_TMP1]);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRGBA);

        vg::drawImage(  0.0f, 300.0f,   0.0f+256.0f, 300.0f+256.0f, textures[TEX_TMP2]);
        glBindTexture(GL_TEXTURE_2D, textures[TEX_TMP2]);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleAAA1);
        vg::drawImage(300.0f, 300.0f, 300.0f+256.0f, 300.0f+256.0f, textures[TEX_TMP2]);
        glBindTexture(GL_TEXTURE_2D, textures[TEX_TMP2]);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRGBA);

        vg::drawImage(600.0f, 300.0f, 600.0f+256.0f, 300.0f+256.0f, textures[TEX_TMP3]);
        glBindTexture(GL_TEXTURE_2D, textures[TEX_TMP3]);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleAAA1);
        vg::drawImage(900.0f, 300.0f, 900.0f+256.0f, 300.0f+256.0f, textures[TEX_TMP3]);
        glBindTexture(GL_TEXTURE_2D, textures[TEX_TMP3]);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRGBA);

        vg::drawImage(  0.0f, 600.0f,   0.0f+256.0f, 600.0f+256.0f, textures[TEX_TMP4]);
        vg::drawImage(300.0f, 600.0f, 300.0f+256.0f, 600.0f+256.0f, textures[TEX_TMP5]);

        vg::drawImage(  0.0f, 900.0f,   0.0f+256.0f, 900.0f+256.0f, textures[TEX_TMP6]);
        vg::drawImage(300.0f, 900.0f, 300.0f+256.0f, 900.0f+256.0f, textures[TEX_TMP7]);

        vg::drawImage( 600.0f, 900.0f,  600.0f+256.0f, 900.0f+256.0f, texSource                  );
        vg::drawImage(1200.0f, 900.0f, 1200.0f+256.0f, 900.0f+256.0f, textures[TEX_TMP2]);
        vg::drawImage( 900.0f, 900.0f,  900.0f+256.0f, 900.0f+256.0f, textures[TEX_GUIDED_RESULT]);

        glPopMatrix();

        CHECK_GL_ERROR();

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, (float)mCPUTime, (float)mGPUTime);
    }

    void onUpdate(float dt)
    {
        if (ui::mouseIsPressed(SDL_BUTTON_LEFT))
        {
            int dx, dy;

            ui::mouseRelOffset(&dx, &dy);
            mOffsetX += dx;
            mOffsetY += dy;
        }
    }

    void onTouch(const ButtonEvent& event)
    {
        if (event.button == SDL_BUTTON_WHEELUP)
        {
            mScale *= 1.2f;
            mOffsetX -= (event.x-mOffsetX)*(1.2f - 1);
            mOffsetY -= (event.y-mOffsetY)*(1.2f - 1);
        }
        else if (event.button == SDL_BUTTON_WHEELDOWN)
        {
            mScale /= 1.2f;
            if (mScale<1.0f)
            {
                //fix it a bit
                mOffsetX -= (event.x-mOffsetX)*(1/mScale/1.2f - 1);
                mOffsetY -= (event.y-mOffsetY)*(1/mScale/1.2f - 1);
                mScale = 1.0f;
            }
            else
            {
                mOffsetX -= (event.x-mOffsetX)*(1/1.2f - 1);
                mOffsetY -= (event.y-mOffsetY)*(1/1.2f - 1);
            }
        }
    }

private:
    bool		mIsDragging;
    float		mOffsetX;
    float		mOffsetY;
    float		mScale;

    VFS				mVFS;
};

extern "C" int main(int argc, char** argv)
{
    PhysisDemo app;
    app.run();

    return 0;
}