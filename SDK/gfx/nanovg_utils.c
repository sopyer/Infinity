#include <gfx/nanovg.h>

void nvguLine(NVGcontext* ctx, float x0, float y0, float x1, float y1)
{
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, x0, y0);
    nvgLineTo(ctx, x1, y1);
    nvgStroke(ctx);
}

void nvguRect(NVGcontext* ctx, float x, float y, float w, float h)
{
    nvgBeginPath(ctx);
    nvgRect(ctx, x, y, w, h);
    nvgFill(ctx);
}
