#include <gfx/gfx.h>
#include "gfx_res.h"
#include "Path.h"
#include "DefaultFontData.h"

//TODO: API change: vgBeginDraw vgEndDraw for common state setting
//TODO: make stencil configurable - make it possible to allocate bits using mask

//Path rendering use 0 as base refence value - it should work fine with GL_*_WRAP stencil operations
//Path rendering avoids explicit stencil clear - cover/paint operation resets stencil

#include <stdio.h>
#include <memory.h>
#include "nanovg/nanovg_gl.h"

NVGcontext* nvgCreateGL3(int flags);
void nvgDeleteGL3(NVGcontext* ctx);

namespace vg
{
    NVGcontext*  ctx;
    Font         defaultFont;

    void initFontSubsystem();
    void shutdownFontSubsystem();

    void init()
    {
        ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        initFontSubsystem();
        defaultFont = createFont(anonymousProBTTF, sizeof(anonymousProBTTF), 16);
    }

    void fini()
    {
        destroyFont(defaultFont);
        shutdownFontSubsystem();
        nvgDeleteGL3(ctx);
    }

    void drawQuad(float xmin, float ymin, float xmax, float ymax, float offset)
    {
        GLuint baseVertex;

        glBindVertexArray(vf::p2_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p2_vertex_t));
        vf::p2_vertex_t* v = gfx::frameAllocVertices<vf::p2_vertex_t>(4, &baseVertex);
        v[0].x = xmin-offset; v[0].y = ymin-offset;
        v[1].x = xmin-offset; v[1].y = ymax+offset;
        v[2].x = xmax+offset; v[2].y = ymin-offset;
        v[3].x = xmax+offset; v[3].y = ymax+offset;
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 4);
    }

    void clearStencil(float xmin, float ymin, float xmax, float ymax, float offset)
    {
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_ALWAYS, 0x00, 0xFF);
        glStencilMask(0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        gfx::setStdProgram(0);

        drawQuad(xmin, ymin, xmax, ymax, offset);
    }

    void setStencilRasterStates(bool useNonZero)
    {
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0xFF); //TODO: make mask configurable
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, useNonZero?GL_INCR_WRAP:GL_INVERT);
        glStencilOpSeparate(GL_BACK,  GL_KEEP, GL_KEEP, useNonZero?GL_DECR_WRAP:GL_INVERT);
    }

    void setStencilFillStates()
    {
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    void drawPath(Path path, uint32_t color, bool useNonZero)
    {
        glEnable(GL_STENCIL_TEST);

        setStencilRasterStates(useNonZero);
        stencilPath(path, 0);

        setStencilFillStates();
        glUseProgram(gfx_res::prgPaintSolid);
        gfx::setMVP();

        GLuint offset;
        v128*  colorf = (v128*)gfx::dynbufAllocMem(sizeof(v128), gfx::caps.uboAlignment, &offset);
        *colorf = vi_cvt_ubyte4_to_vec4(color);
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, gfx::dynBuffer, offset, sizeof(v128));

        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);

        glDisable(GL_STENCIL_TEST);
    }

    void drawPath(Path path, Paint paint, bool useNonZero, bool useAA)
    {
        glEnable(GL_STENCIL_TEST);

        setStencilRasterStates(useNonZero);
        if (useAA) glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        stencilPath(path, useAA);
        if (useAA) glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

        applyPaintAsGLProgram(paint);
        setStencilFillStates();
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);

        glDisable(GL_STENCIL_TEST);
    }

    void drawPathAA(Path path, Paint paint)
    {
        //Clear alpha
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
        glDisable(GL_BLEND);
        glUseProgram(gfx_res::prgPaintSolid);

        GLuint offset;
        v128*  colorf = (v128*)gfx::dynbufAllocMem(sizeof(v128), gfx::caps.uboAlignment, &offset);
        *colorf = vi_set_0000();

        gfx::setMVP();
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, gfx::dynBuffer, offset, sizeof(v128));
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0.0f);

        //stencil
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        stencilPath(path, true);

        //fill
        applyPaintAsGLProgram(paint);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}