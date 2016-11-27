#include "pp.h"
#include <core/core.h>

extern GLuint programs[PRG_ID_COUNT];

void guidedFilterPack(GLuint pI, GLuint pIII, GLuint src, GLuint guide, GLsizei w, GLsizei h)
{
    GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

    GLuint textures[2] = {src, guide};
    glBindTextures(0, ARRAY_SIZE(textures), textures);

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

    GLuint textures[2] = {src, guide};
    glBindTextures(0, ARRAY_SIZE(textures), textures);

    glUseProgram(programs[PRG_GUIDED_AB]);
    glProgramUniform1f(programs[PRG_GUIDED_AB], 0, eps);

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

    GLuint textures[3] = {A, B, guide};
    glBindTextures(0, ARRAY_SIZE(textures), textures);

    glUseProgram(programs[PRG_GUIDED_AI_B]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, res, 0);
    glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void guidedFilter(GLuint dst, GLuint src, GLuint guide, GLuint tmp[9], int kernelWidth, float eps, GLsizei w, GLsizei h)
{
    guidedFilterPack(tmp[0], tmp[1], src, guide, w, h);
        
    boxFilter2Pass(tmp[2], tmp[0], tmp[8], kernelWidth, w, h);
    boxFilter2Pass(tmp[3], tmp[1], tmp[8], kernelWidth, w, h);
        
    guidedFilterAB(tmp[4], tmp[5], tmp[2], tmp[3], eps, w, h);
        
    boxFilter2Pass(tmp[6], tmp[4], tmp[8], kernelWidth, w, h);
    boxFilter2Pass(tmp[7], tmp[5], tmp[8], kernelWidth, w, h);
        
    guidedFilterAI_B(dst, tmp[6], tmp[7], guide, w, h);
}
