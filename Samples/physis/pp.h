#ifndef __GRAPHICS_PP_H_INCLUDED__
#   define __GRAPHICS_PP_H_INCLUDED__

#include <opengl.h>

enum ProgramIDs
{
    PRG_LUM,
    PRG_FILTER,

    PRG_GUIDED_PACK,
    PRG_GUIDED_AB,
    PRG_GUIDED_AI_B,

    PRG_COLOR_GUIDED_PACK_VARI,
    PRG_COLOR_GUIDED_PACK_VARIP,
    PRG_COLOR_GUIDED_AB,
    PRG_COLOR_GUIDED_AI_B,

    PRG_ID_COUNT
};

void ppInit();
void ppFini();

void ppBegin();
void ppEnd();

void ppOnShaderReload();

void convertToLuminance(GLuint dst, GLuint src, GLsizei w, GLsizei h);
void boxFilter1Pass(GLuint dst, GLuint src, int kernelWidth, GLsizei w, GLsizei h);
void boxFilter2Pass(GLuint dst, GLuint src, GLuint tmp, int kernelWidth, GLsizei w, GLsizei h);
void guidedFilter(GLuint dst, GLuint src, GLuint guide, GLuint tmp[9], int kernelWidth, float eps, GLsizei w, GLsizei h);
void colorGuidedFilter(GLuint dst, GLuint src, GLuint guide, GLuint tmp[19], int kernelWidth, float eps, GLsizei w, GLsizei h);


#endif