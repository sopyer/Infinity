#include <opengl.h>

#include "VG.h"
#include "impl/SharedResources.h"
#include "Path.h"

//TODO: API change: vgBeginDraw vgEndDraw for common state setting
//TODO: make stencil configurable - make it possible to allocate bits using mask

//Path rendering use 0 as base refence value - it should work fine with GL_*_WRAP stencil operations
//Path rendering avoids explicit stencil clear - cover/paint operation resets stencil

#define GLNANOVG_IMPLEMENTATION
#include <stdio.h>
#include <memory.h>
#include "glnanovg.h"

namespace vg
{
    struct NVGcontext* ctx;

    void initFontSubsystem();
    void shutdownFontSubsystem();

    void init()
    {
        ctx = glnvgCreate(512,512);
        impl::allocResources();
        initFontSubsystem();
    }

    void fini()
    {
        shutdownFontSubsystem();
        impl::freeResources();
        glnvgDelete(ctx);
    }

    void drawQuad(float xmin, float ymin, float xmax, float ymax, float offset)
    {
        glBegin(GL_QUADS);
        glVertex2f(xmin-offset, ymin-offset);
        glVertex2f(xmin-offset, ymax+offset);
        glVertex2f(xmax+offset, ymax+offset);
        glVertex2f(xmax+offset, ymin-offset);
        glEnd();
    }

    void clearStencil(float xmin, float ymin, float xmax, float ymax, float offset)
    {
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_ALWAYS, 0x00, 0xFF);
        glStencilMask(0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glUseProgram(0);

        drawQuad(xmin, ymin, xmax, ymax, offset);
    }

    void drawPath(Path path, VGubyte red, VGubyte green, VGubyte blue, VGubyte alpha)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_STENCIL_TEST);
        stencilPath(path, 0, 0);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glUseProgram(0);
        glColor4ub(red, green, blue, alpha);
        glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);
        glPopAttrib();
    }

    void drawPathNZ(Path path, VGubyte red, VGubyte green, VGubyte blue, VGubyte alpha)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_STENCIL_TEST);
        stencilPath(path, 0, 1);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glUseProgram(0);
        glColor4ub(red, green, blue, alpha);
        glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);
        glPopAttrib();
    }

    void drawPath(Path path, Paint paint)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_STENCIL_TEST);
        stencilPath(path, 0, 0);

        applyPaintAsGLProgram(paint);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);
        glPopAttrib();
    }

    void drawPathA2C(Path path, Paint paint)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_STENCIL_TEST);
        stencilPath(path, 1, 0);

        applyPaintAsGLProgram(paint);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);
        glPopAttrib();
    }

    void drawPathNZA2C(Path path, Paint paint)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_STENCIL_TEST);
        stencilPath(path, 1, 1);

        applyPaintAsGLProgram(paint);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);
        glPopAttrib();
    }

    void drawPathNZ(Path path, Paint paint)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_STENCIL_TEST);
        stencilPath(path, 0, 1);

        applyPaintAsGLProgram(paint);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);
        glPopAttrib();
    }

    void clearAlpha(float xmin, float ymin, float xmax, float ymax, float offset)
    {
        glUseProgram(0);

        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
        glColor4ub(0, 0, 0, 0);
        drawQuad(xmin, ymin, xmax, ymax, offset);

        glPopAttrib();
    }

    void drawPathAA(Path path, Paint paint)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glEnable(GL_STENCIL_TEST);
        clearAlpha(path->xmin, path->ymin, path->xmax, path->ymax, 0);
        rasterizeEvenOddAA(path);

        applyPaintAsGLProgram(paint);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
        drawQuad(path->xmin, path->ymin, path->xmax, path->ymax, 0);

        glPopAttrib();
    }
}