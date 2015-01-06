#pragma once

#include <gfx/nanovg.h>

#ifdef __cplusplus
extern "C" {
#endif

void nvguLine(NVGcontext* ctx, float x0, float y0, float x1, float y1);
void nvguRect(NVGcontext* ctx, float x, float y, float w, float h);

#ifdef __cplusplus
}
#endif