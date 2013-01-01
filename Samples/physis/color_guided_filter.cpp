#include "pp.h"
#include <framework.h>
#include <utils.h>

extern GLuint programs[PRG_ID_COUNT];
extern GLint  uniEPS3ch;

void colorGuidedFilterPackVarI(GLuint I_xx_yy_zz, GLuint I_xy_xz_yz, GLuint guide, GLsizei w, GLsizei h)
{
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, guide);

    glUseProgram(programs[PRG_COLOR_GUIDED_PACK_VARI]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, I_xx_yy_zz, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, I_xy_xz_yz, 0);
	glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);

    CHECK_GL_ERROR();
}

void colorGuidedFilterPackVarIp(GLuint Ir_r, GLuint Ig_g, GLuint Ib_b,
                                GLuint source, GLuint guide,
                                GLsizei w, GLsizei h)
{
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, source);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, guide);

    glUseProgram(programs[PRG_COLOR_GUIDED_PACK_VARIP]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Ir_r, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, Ig_g, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, Ib_b, 0);
	glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);

    CHECK_GL_ERROR();
}

void colorGuidedFilterAB(GLuint ABx, GLuint ABy, GLuint ABz,
    GLuint guide, GLuint I_xx_yy_zz, GLuint I_xy_xz_yz,
    GLuint Ir_r, GLuint Ig_g, GLuint Ib_b,
    float eps, GLsizei w, GLsizei h)
{
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, guide     );
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, I_xx_yy_zz);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, I_xy_xz_yz);
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, Ir_r      );
    glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, Ig_g      );
    glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, Ib_b      );

    glUseProgram(programs[PRG_COLOR_GUIDED_AB]);
    glUniform1f(uniEPS3ch, eps);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ABx, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, ABy, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, ABz, 0);
	glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
}

void colorGuidedFilterAI_B(GLuint res, GLuint guide, GLuint ABx, GLuint ABy, GLuint ABz, GLsizei w, GLsizei h)
{
	GLenum buffers[] = {GL_COLOR_ATTACHMENT0};

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, guide);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, ABx  );
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, ABy  );
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, ABz  );

    glUseProgram(programs[PRG_COLOR_GUIDED_AI_B]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, res, 0);
	glDrawBuffers(ARRAY_SIZE(buffers), buffers);
    glViewport(0, 0, w, h);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void colorGuidedFilter(GLuint dst, GLuint src, GLuint guide, GLuint tmp[19], int kernelWidth, float eps, GLsizei w, GLsizei h)
{
    colorGuidedFilterPackVarI(tmp[0], tmp[1], guide, w, h);
    colorGuidedFilterPackVarIp(tmp[2], tmp[3], tmp[4], src, guide, w, h);

    boxFilter2Pass(tmp[ 5], src,    tmp[18], kernelWidth, w, h);//TODO is not used - for visualization purpose only
    boxFilter2Pass(tmp[ 6], guide,  tmp[18], kernelWidth, w, h);
    boxFilter2Pass(tmp[ 7], tmp[0], tmp[18], kernelWidth, w, h);
    boxFilter2Pass(tmp[ 8], tmp[1], tmp[18], kernelWidth, w, h);
    boxFilter2Pass(tmp[ 9], tmp[2], tmp[18], kernelWidth, w, h);
    boxFilter2Pass(tmp[10], tmp[3], tmp[18], kernelWidth, w, h);
    boxFilter2Pass(tmp[11], tmp[4], tmp[18], kernelWidth, w, h);

    colorGuidedFilterAB(tmp[12], tmp[13], tmp[14],
                        guide,   tmp[7],  tmp[8],
                        tmp[9],  tmp[10], tmp[11],
                        eps, w, h);

    boxFilter2Pass(tmp[15], tmp[12], tmp[18], kernelWidth, w, h);
    boxFilter2Pass(tmp[16], tmp[13], tmp[18], kernelWidth, w, h);
    boxFilter2Pass(tmp[17], tmp[14], tmp[18], kernelWidth, w, h);

    colorGuidedFilterAI_B(dst, guide, tmp[15], tmp[16], tmp[17], w, h);

    CHECK_GL_ERROR();
}
