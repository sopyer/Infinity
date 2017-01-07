#pragma once

#include <gfx/gfx.h>

namespace vg
{
    struct PaintOpaque
    {
        GLuint        program;
        etlsf_alloc_t allocUniforms;
        GLsizei       size;
        GLsizei       offset;
        GLuint        texture;
    };
}
