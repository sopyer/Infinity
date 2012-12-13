#include <framework.h>
#include <vg/VG.h>
#include <vi.h>

#include "Perlin.h"

static const float quadVertices[6] = {
    -1,  1, 3,  1, -1, -3,
};

class PhysisDemo: public ui::Stage
{
    enum TextureNames
    {
        PERLIN0 = 0,
        PERLIN1,
        PERLIN2,
        PERLIN3,
        PERLIN4,
        PERLIN5,
        PERLIN6,
        PERLIN7,
        TEXTURE_SRT,
        COMBINED0,
        COMBINED1,
        COMBINED2,
        COMBINED3,
        COMBINED4,

        TEXTURE_NAMES_COUNT
    };

    GLuint mTextures[TEXTURE_NAMES_COUNT];

    static const int texSize = 1024;
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

    GLuint  prgConvertToLum;

public:
    void allocTextures()
    {
        glGenTextures(TEXTURE_NAMES_COUNT, mTextures);

        for (size_t i=0; i<TEXTURE_NAMES_COUNT; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, mTextures[i]);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, texSize, texSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint allocTexture(GLsizei w, GLsizei h)
    {
        GLuint tex;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
    }

    void freeTextures()
    {
        glDeleteTextures(TEXTURE_NAMES_COUNT, mTextures);
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
    GLuint texLum;

    GLuint texTmp0;
    GLuint texBlurred;

    GLuint prgFilter;

    GLuint ibo;

    GLint  iboAlignment;

    static const size_t MAX_SAMPLE_COUNT = 64;

    struct FilterDesc
    {
    	int   sampleCount;
        int   pad[3];
	    v128  weights[MAX_SAMPLE_COUNT];
	    v128  offsets[MAX_SAMPLE_COUNT];
    };

    PhysisDemo()
    {
        VFS::mount("AppData");
        VFS::mount("..\\AppData");
        VFS::mount("..\\..\\AppData");

        glGenFramebuffers(1, &fbo);
    
        // Assumption - align is power of 2
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &iboAlignment);
        --iboAlignment;


        glGenBuffers(1, &ibo);

        allocTextures();

        glGetIntegerv(GL_VIEWPORT, vp);

        texGtorProg     = resources::createProgramFromFiles(
                              "PP.common.vert",
                              "TextureGtor.Metal.frag"
                          );
        prgConvertToLum = resources::createProgramFromFiles(
                              "PP.common.vert",
                              "PP.ConvertToLuminance.frag"
                          );
        prgFilter       = resources::createProgramFromFiles(
                              "PP.common.vert",
                              "PP.Filter.frag"
                          );

        GLint structSize;
        GLint uniFilterDesc;
  
        uniFilterDesc  = glGetUniformBlockIndex(prgFilter, "uniFilterDesc");
 
        glGetActiveUniformBlockiv(prgFilter, uniFilterDesc, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(FilterDesc));

        texLum     = allocTexture(256, 256);
        texBlurred = allocTexture(256, 256);
        texTmp0    = allocTexture(256, 256);
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
        freeTextures();
        glDeleteBuffers(1, &ibo);
        glDeleteProgram(texGtorProg);
        glDeleteProgram(prgConvertToLum);
        glDeleteProgram(prgFilter);
        glDeleteProgram(perlinGtorProg);
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &texLum);
        glDeleteTextures(1, &texTmp0);
        glDeleteTextures(1, &texBlurred);
        glDeleteTextures(1, &texSource);
        glDeleteTextures(9, permTex);
        glDeleteTextures(1, &gradTex);
    }

protected:
    void generateTextureCombined(TextureNames dest)
    {
        CHECK_GL_ERROR();

        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN0]);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN1]);
        glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN2]);
        glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN3]);
        glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN4]);
        glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN5]);
        glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN6]);
        glActiveTexture(GL_TEXTURE7); glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN7]);

        glUseProgram(texGtorProg);
        //glUniformMatrix4fv(uniHSCB, 1, false, getHSCB(0.0f, 1.0f, 1.375f, 2.5f));

        generateTexture(mTextures[dest], texSize);

        CHECK_GL_ERROR();
    }

    void generateTexture(GLuint texID, GLsizei texSize)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
        glViewport(0, 0, texSize, texSize);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void generatePerlin(TextureNames dest, int seed, int octaves, float amp, float ampScale, float freq, float freqScale)
    {
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_1D, permTex[seed%9]);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_1D, gradTex);

        glUseProgram(perlinGtorProg);

        glUniform1i(uniOctaves,   octaves);
        glUniform1f(uniAmp,       amp);
        glUniform1f(uniAmpScale,  ampScale);
        glUniform1f(uniFreq,      freq);
        glUniform1f(uniFreqScale, freqScale);

        generateTexture(mTextures[dest], texSize);

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
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, src);

        glUseProgram(prgConvertToLum);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst, 0);
        glViewport(0, 0, w, h);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        CHECK_GL_ERROR();
    }

    virtual void onPaint()
    {
        mCPUTimer.start();
        mGPUTimer.start();

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, quadVertices);

        generatePerlin(PERLIN0, 8, 6, 1.86f, 0.75f,   4.0f, 2.0f);
        generatePerlin(PERLIN1, 1, 5, 0.50f, 0.32f,   4.0f, 2.0f);
        generatePerlin(PERLIN2, 5, 6, 1.86f, 0.75f,   4.0f, 2.0f);
        generatePerlin(PERLIN3, 5, 1, 1.86f, 0.75f,   4.0f, 2.0f);
        generatePerlin(PERLIN4, 5, 6, 1.36f, 0.75f,   4.0f, 2.0f);
        generatePerlin(PERLIN5, 5, 1, 1.36f, 0.75f,   4.0f, 2.0f);
        generatePerlin(PERLIN6, 0, 6, 0.68f, 1.00f,   8.0f, 2.0f);
        generatePerlin(PERLIN7, 0, 2, 1.00f, 1.00f, 128.0f, 2.0f);

        generateTextureCombined(COMBINED0);

        convertToLuminance(texLum, texSource, 256, 256);

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

        vg::drawImage( -8,  2, -6,  0, mTextures[PERLIN0]);
        vg::drawImage( -5,  2, -3,  0, mTextures[PERLIN1]);
        vg::drawImage( -2,  2,  0,  0, mTextures[PERLIN2]);
        vg::drawImage(  1,  2,  3,  0, mTextures[PERLIN3]);
        vg::drawImage(  4,  2,  6,  0, mTextures[PERLIN4]);
        vg::drawImage(  7,  2,  9,  0, mTextures[PERLIN5]);

        vg::drawImage( -8, -1, -6, -3, mTextures[PERLIN6]);
        vg::drawImage( -5, -1, -3, -3, mTextures[PERLIN7]);
        vg::drawImage( -2, -1,  0, -3, mTextures[COMBINED0]);

        glPopMatrix();

        vg::drawImage(350.0f, 000.0f, 350.0f+256.0f, 256.0f, texSource);
        vg::drawImage(650.0f, 000.0f, 650.0f+256.0f, 256.0f, texLum);

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, (float)mCPUTime, (float)mGPUTime);
    }

private:
    VFS				mVFS;
};

extern "C" int main(int argc, char** argv)
{
    PhysisDemo app;
    app.run();

    return 0;
}