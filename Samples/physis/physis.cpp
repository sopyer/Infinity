#include <framework.h>
#include <vg/VG.h>
#include <ml.h>
#include <utils.h>
#include "pp.h"
#include "Perlin.h"
#include "lighting.h"

static const int texSize = 1024;

GLint swizzleAAA1[4] = {GL_ALPHA, GL_ALPHA, GL_ALPHA, GL_ONE  };
GLint swizzleRGBA[4] = {GL_RED,   GL_GREEN, GL_BLUE,  GL_ALPHA};
GLint swizzleR001[4] = {GL_RED,   GL_ZERO,  GL_ZERO,  GL_ONE  };
GLint swizzle0G01[4] = {GL_ZERO,  GL_GREEN, GL_ZERO,  GL_ONE  };
GLint swizzle00B1[4] = {GL_ZERO,  GL_ZERO,  GL_BLUE,  GL_ONE  };

struct ImageDesc
{
    float  x;
    float  y;
    float  w;
    float  h;
    GLuint tex;
    GLint* swizzle;
};

struct TextDesc
{
    float x;
    float y;
    const char* text;
};

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
    TEX_TMP8,
    TEX_TMP9,
    TEX_TMP10,
    TEX_TMP11,
    TEX_TMP12,
    TEX_TMP13,
    TEX_TMP14,
    TEX_TMP15,
    TEX_TMP16,
    TEX_TMP17,
    TEX_TMP18,
    TEX_TMP19,
    TEX_TMP20,
    TEX_TMP21,
    TEX_TMP22,
    TEX_TMP23,
    TEX_TMP24,
    TEX_GUIDED_RESULT,
    TEX_TMP_BOXF,

    TEX_ID_COUNT
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

RTDesc rtDesc[] = 
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
    {TEX_LUMINANCE,           0,       0, GL_RGBA8,        GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_GUIDED_RESULT,       0,       0, GL_RGBA8,        GL_RGBA, GL_UNSIGNED_BYTE },
    {TEX_TMP0,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP1,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP2,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP3,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP4,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP5,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP6,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP7,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP8,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP9,                0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP10,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP11,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP12,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP13,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP14,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP15,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP16,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP17,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP18,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP19,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP20,               0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
    {TEX_TMP_BOXF,            0,       0, GL_RGBA8,       GL_RGBA, GL_UNSIGNED_SHORT},
};

extern GLuint programs[PRG_ID_COUNT];

class PhysisDemo: public ui::Stage
{

    GLuint textures[TEX_ID_COUNT];

    GLint vp[4];

    GLuint texGtorProg, perlinGtorProg;

    GLuint permTex[9], gradTex;

    GLint  uniInvTexDim;

    GLint	uniOctaves;
    GLint	uniAmp;
    GLint	uniAmpScale;
    GLint	uniFreq;
    GLint	uniFreqScale;

    GLuint samNearestRepeat;

    GLint imgWidth, imgHeight;
public:
    void allocTextures()
    {
        glGenTextures(TEX_ID_COUNT, textures);

        for (size_t i=0; i<ARRAY_SIZE(rtDesc); ++i)
        {
            const RTDesc&  desc = rtDesc[i];

            assert(desc.width!=0 && desc.height!=0);

            glBindTexture(GL_TEXTURE_2D, textures[desc.id]);
            glTexImage2D(GL_TEXTURE_2D, 0, desc.internalFmt,
                         desc.width, desc.height, 0,
                         desc.channels, desc.type, 0);
            CHECK_GL_ERROR();
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

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
            glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, PERMUTATION_DIM, 0, GL_RED, GL_UNSIGNED_BYTE, permutation256[i]);
        }

        glGenTextures(1, &gradTex);
        glBindTexture(GL_TEXTURE_1D, gradTex);
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, 8, 0, GL_RG, GL_FLOAT, gradients2D[0]);

        glBindTexture(GL_TEXTURE_2D, 0);

        addPrograms(PRG_ID_COUNT, programs);
    }

    GLint uniSamplerInput, uniRotationScale, uniTranslate;

    GLuint texSource;

    PhysisDemo():
          mOffsetX(0.0f),
          mOffsetY(0.0f),
          mScale(1.0f),
          mIsDragging(false)
    {
        ppInit();
 
        texSource  = resources::createTexture2D("coin.dds", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, TRUE, &imgWidth, &imgHeight);

        assert(rtDesc[14].id == TEX_LUMINANCE);
        for (size_t i = 14; i < ARRAY_SIZE(rtDesc); ++i)
        {
            rtDesc[i].width  = imgWidth;
            rtDesc[i].height = imgHeight;
        }

        allocTextures();

        glGetIntegerv(GL_VIEWPORT, vp);

        texGtorProg     = resources::createProgramFromFiles(
                              "PP.common.vert",
                              "TextureGtor.Metal.frag"
                          );

        CHECK_GL_ERROR();

        CreatePerlinNoiseGtor();

        glGenSamplers(1, &samNearestRepeat);
        glSamplerParameteri(samNearestRepeat, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(samNearestRepeat, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(samNearestRepeat, GL_TEXTURE_WRAP_S,     GL_REPEAT );
        glSamplerParameteri(samNearestRepeat, GL_TEXTURE_WRAP_T,     GL_REPEAT );

        lighting::init();

        CHECK_GL_ERROR();

        currentTab = 1;
    }

    int currentTab;

    CPUTimer	mCPUTimer;
    GPUTimer	mGPUTimer;

    double mCPUTime;
    double mGPUTime;

    ~PhysisDemo()
    {
        lighting::fini();

        glDeleteSamplers(1, &samNearestRepeat);
        glDeleteTextures(TEX_ID_COUNT, textures);
        glDeleteProgram(texGtorProg);
        glDeleteProgram(perlinGtorProg);
        glDeleteTextures(1, &texSource);
        glDeleteTextures(9, permTex);
        glDeleteTextures(1, &gradTex);

        ppFini();
    }

protected:

	void initLightingResources();
	void finiLightingResources();

	void renderLightingScene();

    void onShaderRecompile()
    {
        ppOnShaderReload();
    }

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
        glBindSampler(0, samNearestRepeat);
        glBindSampler(1, samNearestRepeat);
        CHECK_GL_ERROR();
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

    void generateTexture()
    {
        generatePerlin(TEX_PERLIN0, 8, 6, 1.86f, 0.75f,   4.0f, 2.0f);
        generatePerlin(TEX_PERLIN1, 1, 5, 0.50f, 0.32f,   4.0f, 2.0f);
        generatePerlin(TEX_PERLIN2, 5, 6, 1.86f, 0.75f,   4.0f, 2.0f);
        generatePerlin(TEX_PERLIN3, 5, 1, 1.86f, 0.75f,   4.0f, 2.0f);
        generatePerlin(TEX_PERLIN4, 5, 6, 1.36f, 0.75f,   4.0f, 2.0f);
        generatePerlin(TEX_PERLIN5, 5, 1, 1.36f, 0.75f,   4.0f, 2.0f);
        generatePerlin(TEX_PERLIN6, 0, 6, 0.68f, 1.00f,   8.0f, 2.0f);
        generatePerlin(TEX_PERLIN7, 0, 2, 1.00f, 1.00f, 128.0f, 2.0f);

        generateTextureCombined(TEX_COMBINED0);
    }

    void generateColorGuided()
    {
        //boxFilter(textures[TEX_TMP2], textures[TEX_LUMINANCE], 256, 256);
        //boxFilter2Pass(textures[TEX_TMP1], textures[TEX_LUMINANCE], textures[TEX_TMP0], 7, 256, 256);

        GLuint tmp[19] = {
            textures[TEX_TMP0],  textures[TEX_TMP1],
            textures[TEX_TMP2],  textures[TEX_TMP3],
            textures[TEX_TMP4],  textures[TEX_TMP5],
            textures[TEX_TMP6],  textures[TEX_TMP7],
            textures[TEX_TMP8],  textures[TEX_TMP9],
            textures[TEX_TMP10], textures[TEX_TMP11],
            textures[TEX_TMP12], textures[TEX_TMP13],
            textures[TEX_TMP14], textures[TEX_TMP15],
            textures[TEX_TMP16], textures[TEX_TMP17],
            textures[TEX_TMP_BOXF]
        };

        colorGuidedFilter(textures[TEX_GUIDED_RESULT], texSource, texSource, tmp, 7, 0.04f, imgWidth, imgHeight);
    }

    void generateGuided()
    {
        convertToLuminance(textures[TEX_LUMINANCE], texSource, imgWidth, imgHeight);

        //boxFilter(textures[TEX_TMP2], textures[TEX_LUMINANCE], 256, 256);
        //boxFilter2Pass(textures[TEX_TMP1], textures[TEX_LUMINANCE], textures[TEX_TMP0], 7, 256, 256);

        GLuint tmp[9] = {
            textures[TEX_TMP0], textures[TEX_TMP1],
            textures[TEX_TMP2], textures[TEX_TMP3],
            textures[TEX_TMP4], textures[TEX_TMP5],
            textures[TEX_TMP6], textures[TEX_TMP7],
            textures[TEX_TMP_BOXF]
        };

        guidedFilter(textures[TEX_GUIDED_RESULT], texSource, textures[TEX_LUMINANCE], tmp, 7, 0.04f, imgWidth, imgHeight);
    }

    void drawStrings(size_t count, TextDesc* strings)
    {
        for (size_t i = 0; i < count; ++i)
        {
            vg::drawString(ui::defaultFont, strings[i].x, strings[i].y,
                           strings[i].text, strlen(strings[i].text));
        }
    }

    void drawImages(size_t count, ImageDesc* images)
    {
        for (size_t i = 0; i < count; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, images[i].tex);
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, images[i].swizzle);

            vg::drawImage(
                images[i].x, images[i].y,
                images[i].x + images[i].w,
                images[i].y + images[i].h,
                images[i].tex
            );

            glBindTexture(GL_TEXTURE_2D, images[i].tex);
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRGBA);
        }
    }

    void visualizeColorGuided()
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(mOffsetX, mOffsetY, 0);
        glScalef(mScale, mScale, 1);

        TextDesc strings[] = {
            { 450.0f,   15.0f, "source"              },
            { 750.0f,   15.0f, "guided filter result"},
            {1050.0f,   15.0f, "box filter result"   },

            {   0.0f,  315.0f, "source"              },
            { 300.0f,  315.0f, "guide"               },
            { 600.0f,  315.0f, "I_xx_yy_zz"          },
            { 900.0f,  315.0f, "I_xy_xz_yz"          },
            {1200.0f,  315.0f, "I*r"                 },
            {1500.0f,  315.0f, "red"                 },
            {1800.0f,  315.0f, "I*g"                 },
            {2100.0f,  315.0f, "green"               },
            {2400.0f,  315.0f, "I*b"                 },
            {2700.0f,  315.0f, "blue"                },

            {   0.0f,  615.0f, "filtered source"     },
            { 300.0f,  615.0f, "filtered guide"      },
            { 600.0f,  615.0f, "filtered I_xx_yy_zz" },
            { 900.0f,  615.0f, "filtered I_xy_xz_yz" },
            {1200.0f,  615.0f, "filtered I*r"        },
            {1500.0f,  615.0f, "filtered red"        },
            {1800.0f,  615.0f, "filtered I*g"        },
            {2100.0f,  615.0f, "filtered green"      },
            {2400.0f,  615.0f, "filtered I*b"        },
            {2700.0f,  615.0f, "filtered blue"       },

            {   0.0f,  915.0f, "Ar"                  },
            { 300.0f,  915.0f, "Br"                  },
            { 600.0f,  915.0f, "Ag"                  },
            { 900.0f,  915.0f, "Bg"                  },
            {1200.0f,  915.0f, "Ab"                  },
            {1500.0f,  915.0f, "Bb"                  },

            {   0.0f, 1215.0f, "filtered Ar"         },
            { 300.0f, 1215.0f, "filtered Br"         },
            { 600.0f, 1215.0f, "filtered Ag"         },
            { 900.0f, 1215.0f, "filtered Bg"         },
            {1200.0f, 1215.0f, "filtered Ab"         },
            {1500.0f, 1215.0f, "filtered Bb"         },
        };

        drawStrings(ARRAY_SIZE(strings), strings);

        ImageDesc images[] = {
            { 450.0f,   25.0f, 256.0f, 256.0f, texSource,                   swizzleRGBA},
            { 750.0f,   25.0f, 256.0f, 256.0f, textures[TEX_GUIDED_RESULT], swizzleRGBA},
            {1050.0f,   25.0f, 256.0f, 256.0f, textures[TEX_TMP5],          swizzleRGBA},

            {   0.0f,  325.0f, 256.0f, 256.0f, texSource,                   swizzleRGBA},
            { 300.0f,  325.0f, 256.0f, 256.0f, texSource,                   swizzleRGBA},
            { 600.0f,  325.0f, 256.0f, 256.0f, textures[TEX_TMP0],          swizzleRGBA},
            { 900.0f,  325.0f, 256.0f, 256.0f, textures[TEX_TMP1],          swizzleRGBA},
            {1200.0f,  325.0f, 256.0f, 256.0f, textures[TEX_TMP2],          swizzleRGBA},
            {1500.0f,  325.0f, 256.0f, 256.0f, textures[TEX_TMP2],          swizzleR001},
            {1800.0f,  325.0f, 256.0f, 256.0f, textures[TEX_TMP3],          swizzleRGBA},
            {2100.0f,  325.0f, 256.0f, 256.0f, textures[TEX_TMP3],          swizzle0G01},
            {2400.0f,  325.0f, 256.0f, 256.0f, textures[TEX_TMP4],          swizzleRGBA},
            {2700.0f,  325.0f, 256.0f, 256.0f, textures[TEX_TMP4],          swizzle00B1},

            {   0.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP5],          swizzleRGBA},
            { 300.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP6],          swizzleRGBA},
            { 600.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP7],          swizzleRGBA},
            { 900.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP8],          swizzleRGBA},
            {1200.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP9],          swizzleRGBA},
            {1500.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP9],          swizzleR001},
            {1800.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP10],         swizzleRGBA},
            {2100.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP10],         swizzle0G01},
            {2400.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP11],         swizzleRGBA},
            {2700.0f,  625.0f, 256.0f, 256.0f, textures[TEX_TMP11],         swizzle00B1},

            {   0.0f,  925.0f, 256.0f, 256.0f, textures[TEX_TMP12],         swizzleRGBA},
            { 300.0f,  925.0f, 256.0f, 256.0f, textures[TEX_TMP12],         swizzleAAA1},
            { 600.0f,  925.0f, 256.0f, 256.0f, textures[TEX_TMP13],         swizzleRGBA},
            { 900.0f,  925.0f, 256.0f, 256.0f, textures[TEX_TMP13],         swizzleAAA1},
            {1200.0f,  925.0f, 256.0f, 256.0f, textures[TEX_TMP14],         swizzleRGBA},
            {1500.0f,  925.0f, 256.0f, 256.0f, textures[TEX_TMP14],         swizzleAAA1},

            {   0.0f, 1225.0f, 256.0f, 256.0f, textures[TEX_TMP15],         swizzleRGBA},
            { 300.0f, 1225.0f, 256.0f, 256.0f, textures[TEX_TMP15],         swizzleAAA1},
            { 600.0f, 1225.0f, 256.0f, 256.0f, textures[TEX_TMP16],         swizzleRGBA},
            { 900.0f, 1225.0f, 256.0f, 256.0f, textures[TEX_TMP16],         swizzleAAA1},
            {1200.0f, 1225.0f, 256.0f, 256.0f, textures[TEX_TMP17],         swizzleRGBA},
            {1500.0f, 1225.0f, 256.0f, 256.0f, textures[TEX_TMP17],         swizzleAAA1},
        };

        drawImages(ARRAY_SIZE(images), images);

        glPopMatrix();
    }

    void visualizeGuided()
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(mOffsetX, mOffsetY, 0);
        glScalef(mScale, mScale, 1);

        TextDesc strings[] = {
            { 450.0f,  15.0f, "source"              },
            { 750.0f,  15.0f, "guided filter result"},
            {1050.0f,  15.0f, "box filter result"   },
            {   0.0f, 315.0f, "p(source)"           },
            { 300.0f, 315.0f, "I(guide)"            },
            { 600.0f, 315.0f, "p*I"                 },
            { 900.0f, 315.0f, "I*I"                 },
            {1200.0f, 315.0f, "a"                   },
            {1500.0f, 315.0f, "b"                   },
            {   0.0f, 615.0f, "filtered p"          },
            { 300.0f, 615.0f, "filtered I"          },
            { 600.0f, 615.0f, "filtered p*I"        },
            { 900.0f, 615.0f, "filtered I*I"        },
            {1200.0f, 615.0f, "filtered a"          },
            {1500.0f, 615.0f, "filtered b"          },
        };

        drawStrings(ARRAY_SIZE(strings), strings);

        ImageDesc images[] = {
            { 450.0f,  25.0f, 256.0f, 256.0f, texSource,                   swizzleRGBA},
            { 750.0f,  25.0f, 256.0f, 256.0f, textures[TEX_GUIDED_RESULT], swizzleRGBA},
            {1050.0f,  25.0f, 256.0f, 256.0f, textures[TEX_TMP2],          swizzleRGBA},
            {   0.0f, 325.0f, 256.0f, 256.0f, textures[TEX_TMP0],          swizzleRGBA},
            { 300.0f, 325.0f, 256.0f, 256.0f, textures[TEX_TMP0],          swizzleAAA1},
            { 600.0f, 325.0f, 256.0f, 256.0f, textures[TEX_TMP1],          swizzleRGBA},
            { 900.0f, 325.0f, 256.0f, 256.0f, textures[TEX_TMP1],          swizzleAAA1},
            {1200.0f, 325.0f, 256.0f, 256.0f, textures[TEX_TMP4],          swizzleRGBA},
            {1500.0f, 325.0f, 256.0f, 256.0f, textures[TEX_TMP5],          swizzleRGBA},
            {   0.0f, 625.0f, 256.0f, 256.0f, textures[TEX_TMP2],          swizzleRGBA},
            { 300.0f, 625.0f, 256.0f, 256.0f, textures[TEX_TMP2],          swizzleAAA1},
            { 600.0f, 625.0f, 256.0f, 256.0f, textures[TEX_TMP3],          swizzleRGBA},
            { 900.0f, 625.0f, 256.0f, 256.0f, textures[TEX_TMP3],          swizzleAAA1},
            {1200.0f, 625.0f, 256.0f, 256.0f, textures[TEX_TMP6],          swizzleRGBA},
            {1500.0f, 625.0f, 256.0f, 256.0f, textures[TEX_TMP7],          swizzleRGBA},
        };
        
        drawImages(ARRAY_SIZE(images), images);

        glPopMatrix();
    }

    void visualizeTexture()
    {
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
    }

    virtual void onPaint()
    {
        mCPUTimer.start();
        mGPUTimer.start();

        ppBegin();

        switch (currentTab)
        {
            case 0:
                generateTexture();
                break;
            case 1:
                generateGuided();
                break;
            case 2:
                generateColorGuided();
                break;
        }

        ppEnd();

        mGPUTimer.stop();
        mCPUTime = mCPUTimer.elapsed();
        mGPUTime = mGPUTimer.getResult();
        mCPUTimer.stop();

        CHECK_GL_ERROR();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(vp[0], vp[1], vp[2], vp[3]);

        vg::drawRect(3.0f, 3.0f, 1277.0f, 37.0f, 0xFF3E3E3E, 0xFF3E3E3E);

        const float     size    = 120.0f;
        const float     margin  = 2.0f;
        const float     margin2 = 1.0f;
        const uint32_t  count   = 4;

        float pos = (1280.0f/*width*/ - size * count) / 2.0f;

        const char* caption[4] = {
            "PHYSIS",
            "GUIDED",
            "COLOR GUIDED",
            "LIGHTING"
        };

        for (uint32_t i = 0; i < count; ++i)
        {
            if (i > 0)
                vg::drawRect(pos-margin2, 10.0f, pos+margin2, 30.0f, 0xFF585858, 0xFF585858);

            vg::drawRect(pos+margin, 34.0f-(currentTab==i?3.0f:0.0f), pos+size-margin, 35.0f, 0xFFEDB734, 0xFF34B7ED);

            float x = vg::getTextHExtent(ui::defaultFont, caption[i]);
            vg::drawString(ui::defaultFont, pos + 0.5f * (size - x), 25.0f, caption[i], strlen(caption[i]));
            pos += size;
        }
 
        glViewport(vp[0], vp[1], vp[2], vp[3]-40);

        switch (currentTab)
        {
            case 0:
                visualizeTexture();
                break;
            case 1:
                visualizeGuided();
                break;
            case 2:
                visualizeColorGuided();
                break;
            case 3:
                lighting::draw();
                break;
        }

        CHECK_GL_ERROR();

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, (float)mCPUTime, (float)mGPUTime);
    }

    void onUpdate(float dt)
    {
        const float     size   = 118.0f;
        const float     margin = 2.0f;
        const uint32_t  count  = 4;

        float pos = (1280.0f/*width*/ - size * count) / 2.0f  + margin;

        int x, y;

        ui::mouseAbsOffset(&x, &y);
        for (uint32_t i = 0; i < count; ++i)
        {
            if (testPtInRect((float)x, (float)y, pos, 4.0f, size, 32.0f) && ui::mouseWasReleased(SDL_BUTTON_LEFT) )
            {
                currentTab = i;
                break;
            };
            pos += size + 2.0f*margin;
        }

        if (currentTab == 3)
        {
            lighting::update(dt);
        }
        else if (y > 36)
        {
            ui::processZoomAndPan(mScale, mOffsetX, mOffsetY, mIsDragging);
        }
    }

private:
    bool		mIsDragging;
    float		mOffsetX;
    float		mOffsetY;
    float		mScale;
};

extern "C" int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    {
        PhysisDemo app;
        app.run();
    }

    fwk::cleanup();

    return 0;
}