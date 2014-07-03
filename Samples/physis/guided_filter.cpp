#include "pp.h"
#include <framework.h>
#include <utils.h>
#include <ml.h>

extern GLuint programs[PRG_ID_COUNT];
extern GLint  uniEPS;

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

void guidedFilter(GLuint dst, GLuint src, GLuint guide, GLuint tmp[9], int kernelWidth, float eps, GLsizei w, GLsizei h)
{
    guidedFilterPack(tmp[0], tmp[1], src, guide, w, h);
        
    boxFilter2Pass(tmp[2], tmp[0], tmp[8], kernelWidth, w, h);
    boxFilter2Pass(tmp[3], tmp[1], tmp[8], kernelWidth, w, h);
        
    guidedFilterAB(tmp[4], tmp[5], tmp[2], tmp[3], eps, w, h);
        
    boxFilter2Pass(tmp[6], tmp[4], tmp[8], kernelWidth, w, h);
    boxFilter2Pass(tmp[7], tmp[5], tmp[8], kernelWidth, w, h);
        
    guidedFilterAI_B(dst, tmp[6], tmp[7], guide, w, h);

    CHECK_GL_ERROR();
}
