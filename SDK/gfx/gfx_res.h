#pragma once

#include <gfx/gfx.h>

namespace gfx
{
    extern mspace_t  memArena;
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
    extern GLuint prgRect;

    extern GLuint vaoRect;

    extern etlsf_arena_t vgGArena;
    extern GLuint        buffer;

    void init();
    void fini();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// NanoVG backend declarations


// Create flags

enum NVGcreateFlags {
	// Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
	NVG_ANTIALIAS 		= 1<<0,
	// Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
	// slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
	NVG_STENCIL_STROKES	= 1<<1,
	// Flag indicating that additional debug checks are done.
	NVG_DEBUG 			= 1<<2,
};

NVGcontext* nvgCreateGL3(int flags);
void nvgDeleteGL3(NVGcontext* ctx);
