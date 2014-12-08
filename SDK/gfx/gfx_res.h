#pragma once

#include <gfx/gfx.h>

namespace gfx
{
    extern mem::arena_t  memArena;
}

namespace gfx_res
{
    static const size_t STD_PROGRAM_COUNT = 1<<gfx::STD_FEATURE_COUNT;

    extern GLuint stdPrograms[STD_PROGRAM_COUNT];

    extern GLuint prgUI;
    extern GLuint prgRasterCubic;
    extern GLuint prgRasterCubicAA;
    extern GLuint prgPaintSolid;
    extern GLuint prgPaintLinGradient;
    extern GLuint prgLine;
    extern GLuint prgPoint;

    extern etlsf_arena_t vgGArena;
    extern GLuint        buffer;

    void init();
    void fini();
}
