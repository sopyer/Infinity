//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include "imgui.h"
#include <gfx/gfx.h>

// Some math headers don't have PI defined.
static const float PI = 3.14159265f;

inline unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return (r) | (g << 8) | (b << 16) | (a << 24);
}

static void drawPolygon(const float* coords, unsigned numCoords, float r, unsigned int col)
{
    nvgBeginPath(vg::ctx);
    nvgFillColor(vg::ctx, nvgRGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff));

    nvgMoveTo(vg::ctx, coords[0], coords[1]);	

	for (unsigned i = 1; i < numCoords; ++i)
	{
		nvgLineTo(vg::ctx, coords[i*2], coords[i*2+1]);
	}
	
    nvgFill(vg::ctx);
}

static void drawRect(float x, float y, float w, float h, float fth, unsigned int col)
{
    nvgBeginPath(vg::ctx);
    nvgFillColor(vg::ctx, nvgRGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff));
    nvgRect(vg::ctx, x, y, w, h);
    nvgFill(vg::ctx);
}


static void drawRoundedRect(float x, float y, float w, float h, float r, float fth, unsigned int col)
{
    nvgBeginPath(vg::ctx);
    nvgFillColor(vg::ctx, nvgRGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff));
    nvgRoundedRect(vg::ctx, x, y, w, h, r);
    nvgFill(vg::ctx);
}


static void drawLine(float x0, float y0, float x1, float y1, float r, float fth, unsigned int col)
{
    nvgBeginPath(vg::ctx);
    nvgFillColor(vg::ctx, nvgRGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff));
    nvgMoveTo(vg::ctx, x0, y0);	
	nvgLineTo(vg::ctx, x1, y1);
    nvgFill(vg::ctx);
}

int font = -1;

bool imguiRenderNVGInit(const char* fontPath)
{
    memory_t fileData;
    if (mem_file(&fileData, fontPath))
    {
        //fontNormal = nvgAddFont(vg, "sans", "../example/FiraSans-Regular.ttf");
        font = nvgCreateFontMem(vg::ctx, "imguifont", fileData.buffer, fileData.size, 0);
    	return true;
    }

    return false;
}

void imguiRenderNVGDestroy()
{
}

static void drawText(float x, float y, const char *text, int align, unsigned int col)
{
    if (font<0)
        return;

    int vgalign = NVG_ALIGN_MIDDLE;

	if (align == IMGUI_ALIGN_CENTER)
   	    vgalign |= NVG_ALIGN_CENTER;
	else if (align == IMGUI_ALIGN_RIGHT)
		vgalign |= NVG_ALIGN_RIGHT;
    else
        vgalign |= IMGUI_ALIGN_LEFT;
	
    nvgFontSize(vg::ctx, 14);
    nvgFontFace(vg::ctx, "imguifont");
    nvgTextAlign(vg::ctx, vgalign);
    nvgFillColor(vg::ctx, nvgRGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff));
    nvgText(vg::ctx, x, y, text, NULL);
}

void imguiRenderNVGDraw()
{
	const imguiGfxCmd* q = imguiGetRenderQueue();
	int nq = imguiGetRenderQueueSize();

	const float s = 1.0f/8.0f;

    nvgResetScissor(vg::ctx);
	for (int i = 0; i < nq; ++i)
	{
		const imguiGfxCmd& cmd = q[i];
		if (cmd.type == IMGUI_GFXCMD_RECT)
		{
			if (cmd.rect.r == 0)
			{
				drawRect((float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
						 (float)cmd.rect.w*s-1, (float)cmd.rect.h*s-1,
						 1.0f, cmd.col);
			}
			else
			{
				drawRoundedRect((float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
								(float)cmd.rect.w*s-1, (float)cmd.rect.h*s-1,
								(float)cmd.rect.r*s, 1.0f, cmd.col);
			}
		}
		else if (cmd.type == IMGUI_GFXCMD_LINE)
		{
			drawLine(cmd.line.x0*s, cmd.line.y0*s, cmd.line.x1*s, cmd.line.y1*s, cmd.line.r*s, 1.0f, cmd.col);
		}
		else if (cmd.type == IMGUI_GFXCMD_TRIANGLE)
		{
			if (cmd.flags == 1)
			{
				const float verts[3*2] =
				{
					(float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
					(float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s-1, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s/2-0.5f,
					(float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
				};
				drawPolygon(verts, 3, 1.0f, cmd.col);
			}
			if (cmd.flags == 2)
			{
				const float verts[3*2] =
				{
					(float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
					(float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s/2-0.5f, (float)cmd.rect.y*s+0.5f,
					(float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s-1, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
				};
				drawPolygon(verts, 3, 1.0f, cmd.col);
			}
		}
		else if (cmd.type == IMGUI_GFXCMD_TEXT)
		{
			drawText(cmd.text.x, cmd.text.y, cmd.text.text, cmd.text.align, cmd.col);
		}
		else if (cmd.type == IMGUI_GFXCMD_SCISSOR)
		{
			if (cmd.flags)
			{
                nvgScissor(vg::ctx, cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h);
			}
			else
			{
                nvgResetScissor(vg::ctx);
			}
		}
	}
    nvgResetScissor(vg::ctx);
}
