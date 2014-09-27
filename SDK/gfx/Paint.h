#pragma once

#include <gfx/gfx.h>

namespace vg
{
    struct PaintOpaque
    {
        GLuint    program;
        uint16_t  allocUniforms;
        GLsizei   size;
        GLsizei   offset;
        GLuint    texture;
    };
}
