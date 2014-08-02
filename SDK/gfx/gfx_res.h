#pragma once

namespace impl
{
    enum
    {
        //PRG_SIMPLE_UI uniforms
        UNI_SIMPLE_UI_FILL_COLOR   = 0,
        UNI_SIMPLE_UI_BORDER_COLOR = 1,
        UNI_SIMPLE_UI_ZONES        = 2,
    };

    extern GLuint   simpleUIProgram;

    extern GLuint   stencilCubicAreaProgram;
    extern GLuint   stencilCubicAreaAAProgram;
    extern GLuint   stencilQuadAreaProgram;
    extern GLuint   stencilArcAreaProgram;

    enum
    {
        UNI_LIN_GRAD_STOPS          =  0,
        UNI_LIN_GRAD_SCALES         =  8,
        UNI_LIN_GRAD_INV_STOP_COUNT = 16,
        UNI_LIN_GRAD_START_POINT    = 17,
        UNI_LIN_GRAD_DIRECTION      = 18,
    };

    extern GLuint   linGradProgram;

    void allocResources();
    void freeResources();
}
