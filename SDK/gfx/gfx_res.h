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
    extern GLuint prgNanoVG;
    extern GLuint prgNanoVGAA;

    extern etlsf_arena_t vgGArena;
    extern GLuint        buffer;

    void init();
    void fini();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// NanoVG backend declarations


// Create flags

// Flag indicating if geoemtry based anti-aliasing is used (may not be needed when using MSAA).
#define NVG_ANTIALIAS 1 	

// Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
// slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
#define NVG_STENCIL_STROKES 2

NVGcontext* nvgCreateGL3(int flags);
void nvgDeleteGL3(NVGcontext* ctx);
