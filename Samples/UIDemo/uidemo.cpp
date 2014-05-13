#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <utils.h>
#include <SpectatorCamera.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729

namespace ui
{
    struct AreaDesc
    {
        int             preicon;
        const char*     text;
        float           x;
        float           y;
        float           w;
        float           h;
        unsigned int    state;
    };
}

static char* cpToUTF8(int cp, char* str)
{
    int n = 0;
    if (cp < 0x80) n = 1;
    else if (cp < 0x800) n = 2;
    else if (cp < 0x10000) n = 3;
    else if (cp < 0x200000) n = 4;
    else if (cp < 0x4000000) n = 5;
    else if (cp <= 0x7fffffff) n = 6;
    str[n] = '\0';
    switch (n) {
    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
    case 1: str[0] = cp;
    }
    return str;
}


void drawWindow(struct NVGcontext* vg, const char* title, float x, float y, float w, float h)
{
    float cornerRadius = 3.0f;
    struct NVGpaint shadowPaint;
    struct NVGpaint headerPaint;

    nvgSave(vg);
    //	nvgClearState(vg);

    // Window
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgFillColor(vg, nvgRGBA(28,30,34,192));
    //	nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgFill(vg);

    // Drop shadow
    shadowPaint = nvgBoxGradient(vg, x,y+2, w,h, cornerRadius*2, 10, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, x-10,y-10, w+20,h+30);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, shadowPaint);
    nvgFill(vg);

    // Header
    headerPaint = nvgLinearGradient(vg, x,y,x,y+15, nvgRGBA(255,255,255,8), nvgRGBA(0,0,0,16));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,30, cornerRadius-1);
    nvgFillPaint(vg, headerPaint);
    nvgFill(vg);
    nvgBeginPath(vg);
    nvgMoveTo(vg, x+0.5f, y+0.5f+30);
    nvgLineTo(vg, x+0.5f+w-1, y+0.5f+30);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
    nvgStroke(vg);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

    nvgFontBlur(vg,2);
    nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgText(vg, x+w/2,y+16+1, title, NULL);

    nvgFontBlur(vg,0);
    nvgFillColor(vg, nvgRGBA(220,220,220,160));
    nvgText(vg, x+w/2,y+16, title, NULL);

    nvgRestore(vg);
}

void drawSearchBox(struct NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
    struct NVGpaint bg;
    char icon[8];
    float cornerRadius = h/2-1;

    // Edit
    bg = nvgBoxGradient(vg, x,y+1.5f, w,h, h/2,5, nvgRGBA(0,0,0,16), nvgRGBA(0,0,0,92));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    /*	nvgBeginPath(vg);
    nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
    nvgStroke(vg);*/

    nvgFontSize(vg, h*1.3f);
    nvgFontFace(vg, "icons");
    nvgFillColor(vg, nvgRGBA(255,255,255,64));
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+h*0.55f, y+h*0.55f, cpToUTF8(ICON_SEARCH,icon), NULL);

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,32));

    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+h*1.05f,y+h*0.5f,text, NULL);

    nvgFontSize(vg, h*1.3f);
    nvgFontFace(vg, "icons");
    nvgFillColor(vg, nvgRGBA(255,255,255,32));
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+w-h*0.55f, y+h*0.55f, cpToUTF8(ICON_CIRCLED_CROSS,icon), NULL);
}

void drawDropDown(struct NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
    struct NVGpaint bg;
    char icon[8];
    float cornerRadius = 4.0f;

    bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,16), nvgRGBA(0,0,0,16));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
    nvgStroke(vg);

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,160));
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+h*0.3f,y+h*0.5f,text, NULL);

    nvgFontSize(vg, h*1.3f);
    nvgFontFace(vg, "icons");
    nvgFillColor(vg, nvgRGBA(255,255,255,64));
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+w-h*0.5f, y+h*0.5f, cpToUTF8(ICON_CHEVRON_RIGHT,icon), NULL);
}

void drawLabel(struct NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,128));

    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x,y+h*0.5f,text, NULL);
}

void drawEditBoxBase(struct NVGcontext* vg, float x, float y, float w, float h)
{
    struct NVGpaint bg;
    // Edit
    bg = nvgBoxGradient(vg, x+1,y+1+1.5f, w-2,h-2, 3,4, nvgRGBA(255,255,255,32), nvgRGBA(32,32,32,32));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,h-2, 4-1);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, 4-0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
    nvgStroke(vg);
}

void drawEditBox(struct NVGcontext* vg, const char* text, float x, float y, float w, float h)
{

    drawEditBoxBase(vg, x,y, w,h);

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,64));
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+h*0.3f,y+h*0.5f,text, NULL);
}

void drawEditBoxNum(struct NVGcontext* vg,
    const char* text, const char* units, float x, float y, float w, float h)
{
    float uw;

    drawEditBoxBase(vg, x,y, w,h);

    uw = nvgTextBounds(vg, units, NULL, NULL);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,64));
    nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+w-h*0.3f,y+h*0.5f,units, NULL);

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,128));
    nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+w-uw-h*0.5f,y+h*0.5f,text, NULL);
}

void drawCheckBox(struct NVGcontext* vg, const char* text, float x, float y, float w, float h, bool checked)
{
    struct NVGpaint bg;
    char icon[8];

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,160));

    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+28,y+h*0.5f,text, NULL);

    bg = nvgBoxGradient(vg, x+1,y+(int)(h*0.5f)-9+1, 18,18, 3,3, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,92));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+(int)(h*0.5f)-9, 18,18, 3);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    if (checked)
    {
        nvgFontSize(vg, 40);
        nvgFontFace(vg, "icons");
        nvgFillColor(vg, nvgRGBA(255,255,255,128));
        nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
        nvgText(vg, x+9+2, y+h*0.5f, cpToUTF8(ICON_CHECK,icon), NULL);
    }
}

enum
{
        BUTTON_FOCUSED = 1 << 0,
        BUTTON_PRESSED = 1 << 1,
        BUTTON_DEFAULT = 0 << 2,
        BUTTON_ACTION  = 1 << 2,
        BUTTON_ALERT   = 2 << 2,
};

void drawButton(struct NVGcontext* vg, int preicon, const char* text, float x, float y, float w, float h, unsigned int state)
{
    struct NVGpaint bg;
    char icon[8];
    float cornerRadius = 4.0f;
    float tw = 0, iw = 0;
    
    unsigned int color0;
    unsigned int color1;


    if (state&BUTTON_FOCUSED || state&BUTTON_PRESSED)
    {
        color0 = nvgRGBA( 64,  64,  64, 48);
        color1 = nvgRGBA(255, 255, 255, 48);
    }
    else
    {
        color0 = nvgRGBA(  0,   0,   0, 32);
        color1 = nvgRGBA(255, 255, 255, 32);
    }

    if (state&BUTTON_PRESSED)
        bg = nvgLinearGradient(vg, x,y,x,y+h, color0, color1);
    else
        bg = nvgLinearGradient(vg, x,y,x,y+h, color1, color0);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);

    if (state&BUTTON_ACTION)
    {
        nvgFillColor(vg, nvgRGBA(0,96,128,255));
        nvgFill(vg);
    }
    else if (state&BUTTON_ALERT)
    {
        nvgFillColor(vg, nvgRGBA(128,16,8,255));
        nvgFill(vg);
    }

    nvgFillPaint(vg, bg);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
    nvgStroke(vg);

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans-bold");
    tw = nvgTextBounds(vg, text, NULL, NULL);
    if (preicon != 0) {
        nvgFontSize(vg, h*1.3f);
        nvgFontFace(vg, "icons");
        iw = nvgTextBounds(vg, cpToUTF8(preicon,icon), NULL, NULL);
        iw += h*0.15f;
    }

    if (preicon != 0) {
        nvgFontSize(vg, h*1.3f);
        nvgFontFace(vg, "icons");
        nvgFillColor(vg, nvgRGBA(255,255,255,96));
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
        nvgText(vg, x+w*0.5f-tw*0.5f-iw*0.75f, y+h*0.5f, cpToUTF8(preicon,icon), NULL);
    }

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, nvgRGBA(0,0,0,160));
    nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f-1,text, NULL);
    nvgFillColor(vg, nvgRGBA(255,255,255,160));
    nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f,text, NULL);
}

void drawSlider(struct NVGcontext* vg, float pos, float x, float y, float w, float h)
{
    struct NVGpaint bg, knob;
    float cy = y+(int)(h*0.5f);
    float kr = (int)(h*0.25f);

    nvgSave(vg);
    //	nvgClearState(vg);

    // Slot
    bg = nvgBoxGradient(vg, x,cy-2+1, w,4, 2,2, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,128));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x,cy-2, w,4, 2);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    // Knob Shadow
    bg = nvgRadialGradient(vg, x+(int)(pos*w),cy+1, kr-3,kr+3, nvgRGBA(0,0,0,64), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, x+(int)(pos*w)-kr-5,cy-kr-5,kr*2+5+5,kr*2+5+5+3);
    nvgCircle(vg, x+(int)(pos*w),cy, kr);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    pos = std::max(pos, 0.0f);
    pos = std::min(pos, w);

    // Knob
    knob = nvgLinearGradient(vg, x,cy-kr,x,cy+kr, nvgRGBA(255,255,255,16), nvgRGBA(0,0,0,16));
    nvgBeginPath(vg);
    nvgCircle(vg, x+(int)pos,cy, kr-1);
    nvgFillColor(vg, nvgRGBA(40,43,48,255));
    nvgFill(vg);
    nvgFillPaint(vg, knob);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgCircle(vg, x+(int)pos,cy, kr-0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,92));
    nvgStroke(vg);

    nvgRestore(vg);
}

void drawEyes(struct NVGcontext* vg, float x, float y, float w, float h, float mx, float my, float t)
{
    struct NVGpaint gloss, bg;
    float ex = w *0.23f;
    float ey = h * 0.5f;
    float lx = x + ex;
    float ly = y + ey;
    float rx = x + w - ex;
    float ry = y + ey;
    float dx,dy,d;
    float br = (ex < ey ? ex : ey) * 0.5f;
    float blink = 1 - pow(sinf(t*0.5f),200)*0.8f;

    bg = nvgLinearGradient(vg, x,y+h*0.5f,x+w*0.1f,y+h, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,16));
    nvgBeginPath(vg);
    nvgEllipse(vg, lx+3.0f,ly+16.0f, ex,ey);
    nvgEllipse(vg, rx+3.0f,ry+16.0f, ex,ey);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    bg = nvgLinearGradient(vg, x,y+h*0.25f,x+w*0.1f,y+h, nvgRGBA(220,220,220,255), nvgRGBA(128,128,128,255));
    nvgBeginPath(vg);
    nvgEllipse(vg, lx,ly, ex,ey);
    nvgEllipse(vg, rx,ry, ex,ey);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    dx = (mx - rx) / (ex * 10);
    dy = (my - ry) / (ey * 10);
    d = sqrtf(dx*dx+dy*dy);
    if (d > 1.0f) {
        dx /= d; dy /= d;
    }
    dx *= ex*0.4f;
    dy *= ey*0.5f;
    nvgBeginPath(vg);
    nvgEllipse(vg, lx+dx,ly+dy+ey*0.25f*(1-blink), br,br*blink);
    nvgFillColor(vg, nvgRGBA(32,32,32,255));
    nvgFill(vg);

    dx = (mx - rx) / (ex * 10);
    dy = (my - ry) / (ey * 10);
    d = sqrtf(dx*dx+dy*dy);
    if (d > 1.0f) {
        dx /= d; dy /= d;
    }
    dx *= ex*0.4f;
    dy *= ey*0.5f;
    nvgBeginPath(vg);
    nvgEllipse(vg, rx+dx,ry+dy+ey*0.25f*(1-blink), br,br*blink);
    nvgFillColor(vg, nvgRGBA(32,32,32,255));
    nvgFill(vg);

    gloss = nvgRadialGradient(vg, lx-ex*0.25f,ly-ey*0.5f, ex*0.1f,ex*0.75f, nvgRGBA(255,255,255,128), nvgRGBA(255,255,255,0));
    nvgBeginPath(vg);
    nvgEllipse(vg, lx,ly, ex,ey);
    nvgFillPaint(vg, gloss);
    nvgFill(vg);

    gloss = nvgRadialGradient(vg, rx-ex*0.25f,ry-ey*0.5f, ex*0.1f,ex*0.75f, nvgRGBA(255,255,255,128), nvgRGBA(255,255,255,0));
    nvgBeginPath(vg);
    nvgEllipse(vg, rx,ry, ex,ey);
    nvgFillPaint(vg, gloss);
    nvgFill(vg);
}

void drawGraph(struct NVGcontext* vg, float x, float y, float w, float h, float t)
{
    struct NVGpaint bg;
    float samples[6];
    float sx[6], sy[6];
    float dx = w/5.0f;
    int i;

    samples[0] = (1+sinf(t*1.23450f+cosf(t*0.33457f)*0.44f))*0.5f;
    samples[1] = (1+sinf(t*0.68363f+cosf(t*1.30000f)*1.55f))*0.5f;
    samples[2] = (1+sinf(t*1.16420f+cosf(t*0.33457f)*1.24f))*0.5f;
    samples[3] = (1+sinf(t*0.56345f+cosf(t*1.63000f)*0.14f))*0.5f;
    samples[4] = (1+sinf(t*1.62450f+cosf(t*0.25400f)*0.30f))*0.5f;
    samples[5] = (1+sinf(t*0.34500f+cosf(t*0.03000f)*0.60f))*0.5f;

    for (i = 0; i < 6; i++) {
        sx[i] = x+i*dx;
        sy[i] = y+h*samples[i]*0.8f;
    }

    // Graph background
    bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(0,160,192,0), nvgRGBA(0,160,192,64));
    nvgBeginPath(vg);
    nvgMoveTo(vg, sx[0], sy[0]);
    for (i = 1; i < 6; i++)
        nvgBezierTo(vg, sx[i-1]+dx*0.5f,sy[i-1], sx[i]-dx*0.5f,sy[i], sx[i],sy[i]);
    nvgLineTo(vg, x+w, y+h);
    nvgLineTo(vg, x, y+h);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    // Graph line
    nvgBeginPath(vg);
    nvgMoveTo(vg, sx[0], sy[0]+2);
    for (i = 1; i < 6; i++)
        nvgBezierTo(vg, sx[i-1]+dx*0.5f,sy[i-1]+2, sx[i]-dx*0.5f,sy[i]+2, sx[i],sy[i]+2);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
    nvgStrokeWidth(vg, 3.0f);
    nvgStroke(vg);

    nvgBeginPath(vg);
    nvgMoveTo(vg, sx[0], sy[0]);
    for (i = 1; i < 6; i++)
        nvgBezierTo(vg, sx[i-1]+dx*0.5f,sy[i-1], sx[i]-dx*0.5f,sy[i], sx[i],sy[i]);
    nvgStrokeColor(vg, nvgRGBA(0,160,192,255));
    nvgStrokeWidth(vg, 3.0f);
    nvgStroke(vg);

    // Graph sample pos
    for (i = 0; i < 6; i++) {
        bg = nvgRadialGradient(vg, sx[i],sy[i]+2, 3.0f,8.0f, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,0));
        nvgBeginPath(vg);
        nvgRect(vg, sx[i]-10, sy[i]-10+2, 20,20);
        nvgFillPaint(vg, bg);
        nvgFill(vg);
    }

    nvgBeginPath(vg);
    for (i = 0; i < 6; i++)
        nvgCircle(vg, sx[i], sy[i], 4.0f);
    nvgFillColor(vg, nvgRGBA(0,160,192,255));
    nvgFill(vg);
    nvgBeginPath(vg);
    for (i = 0; i < 6; i++)
        nvgCircle(vg, sx[i], sy[i], 2.0f);
    nvgFillColor(vg, nvgRGBA(220,220,220,255));
    nvgFill(vg);

    nvgStrokeWidth(vg, 1.0f);
}

void drawThumbnails(struct NVGcontext* vg, float x, float y, float w, float h, const int* images, int nimages, float t)
{
    float cornerRadius = 3.0f;
    struct NVGpaint shadowPaint, imgPaint, fadePaint;
    float ix,iy,iw,ih;
    float thumb = 60.0f;
    float arry = 30.5f;
    int imgw, imgh;
    float stackh = (nimages/2) * (thumb+10) + 10;
    int i;
    float u = (1+cosf(t*0.5f))*0.5f;

    nvgSave(vg);
    //	nvgClearState(vg);

    // Drop shadow
    shadowPaint = nvgBoxGradient(vg, x,y+4, w,h, cornerRadius*2, 20, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, x-10,y-10, w+20,h+30);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, shadowPaint);
    nvgFill(vg);

    // Window
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgMoveTo(vg, x-10,y+arry);
    nvgLineTo(vg, x+1,y+arry-11);
    nvgLineTo(vg, x+1,y+arry+11);
    nvgFillColor(vg, nvgRGBA(200,200,200,255));
    nvgFill(vg);

    nvgSave(vg);
    nvgScissor(vg, x,y,w,h);
    nvgTranslate(vg, 0, -(stackh - h)*u);

    for (i = 0; i < nimages; i++) {
        float tx, ty;
        tx = x+10;
        ty = y+10;
        tx += (i%2) * (thumb+10);
        ty += (i/2) * (thumb+10);
        nvgImageSize(vg, images[i], &imgw, &imgh);
        if (imgw < imgh) {
            iw = thumb;
            ih = iw * (float)imgh/(float)imgw;
            ix = 0;
            iy = -(ih-thumb)*0.5f;
        } else {
            ih = thumb;
            iw = ih * (float)imgw/(float)imgh;
            ix = -(iw-thumb)*0.5f;
            iy = 0;
        }
        imgPaint = nvgImagePattern(vg, tx+ix, ty+iy, iw,ih, 0.0f/180.0f*NVG_PI, images[i], 0);
        nvgBeginPath(vg);
        nvgRoundedRect(vg, tx,ty, thumb,thumb, 5);
        nvgFillPaint(vg, imgPaint);
        nvgFill(vg);

        shadowPaint = nvgBoxGradient(vg, tx-1,ty, thumb+2,thumb+2, 5, 3, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
        nvgBeginPath(vg);
        nvgRect(vg, tx-5,ty-5, thumb+10,thumb+10);
        nvgRoundedRect(vg, tx,ty, thumb,thumb, 6);
        nvgPathWinding(vg, NVG_HOLE);
        nvgFillPaint(vg, shadowPaint);
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgRoundedRect(vg, tx+0.5f,ty+0.5f, thumb-1,thumb-1, 4-0.5f);
        nvgStrokeWidth(vg,1.0f);
        nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
        nvgStroke(vg);
    }
    nvgRestore(vg);

    // Hide fades
    fadePaint = nvgLinearGradient(vg, x,y,x,y+6, nvgRGBA(200,200,200,255), nvgRGBA(200,200,200,0));
    nvgBeginPath(vg);
    nvgRect(vg, x+4,y,w-8,6);
    nvgFillPaint(vg, fadePaint);
    nvgFill(vg);

    fadePaint = nvgLinearGradient(vg, x,y+h,x,y+h-6, nvgRGBA(200,200,200,255), nvgRGBA(200,200,200,0));
    nvgBeginPath(vg);
    nvgRect(vg, x+4,y+h-6,w-8,6);
    nvgFillPaint(vg, fadePaint);
    nvgFill(vg);

    // Scroll bar
    shadowPaint = nvgBoxGradient(vg, x+w-12+1,y+4+1, 8,h-8, 3,4, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,92));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+w-12,y+4, 8,h-8, 3);
    nvgFillPaint(vg, shadowPaint);
    //	nvgFillColor(vg, nvgRGBA(255,0,0,128));
    nvgFill(vg);

    float scrollh = (h/stackh) * (h-8);
    shadowPaint = nvgBoxGradient(vg, x+w-12-1,y+4+(h-8-scrollh)*u-1, 8,scrollh, 3,4, nvgRGBA(220,220,220,255), nvgRGBA(128,128,128,255));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+w-12+1,y+4+1 + (h-8-scrollh)*u, 8-2,scrollh-2, 2);
    nvgFillPaint(vg, shadowPaint);
    //	nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgFill(vg);

    nvgRestore(vg);
}

void drawColorwheel(struct NVGcontext* vg, float x, float y, float w, float h, float t)
{
    int i;
    float r0, r1, ax,ay, bx,by, cx,cy, aeps, r;
    float hue = sinf(t * 0.12f);
    struct NVGpaint paint;

    nvgSave(vg);

    /*	nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
    nvgFillColor(vg, nvgRGBA(255,0,0,128));
    nvgFill(vg);*/

    cx = x + w*0.5f;
    cy = y + h*0.5f;
    r1 = (w < h ? w : h) * 0.5f - 5.0f;
    r0 = r1 - 20.0f;
    aeps = 0.5f / r1;	// half a pixel arc length in radians (2pi cancels out).

    for (i = 0; i < 6; i++) {
        float a0 = (float)i / 6.0f * NVG_PI * 2.0f - aeps;
        float a1 = (float)(i+1.0f) / 6.0f * NVG_PI * 2.0f + aeps;
        nvgBeginPath(vg);
        nvgArc(vg, cx,cy, r0, a0, a1, NVG_CW);
        nvgArc(vg, cx,cy, r1, a1, a0, NVG_CCW);
        nvgClosePath(vg);
        ax = cx + cosf(a0) * (r0+r1)*0.5f;
        ay = cy + sinf(a0) * (r0+r1)*0.5f;
        bx = cx + cosf(a1) * (r0+r1)*0.5f;
        by = cy + sinf(a1) * (r0+r1)*0.5f;
        paint = nvgLinearGradient(vg, ax,ay, bx,by, nvgHSLA(a0/(NVG_PI*2),1.0f,0.55f,255), nvgHSLA(a1/(NVG_PI*2),1.0f,0.55f,255));
        nvgFillPaint(vg, paint);
        nvgFill(vg);
    }

    nvgBeginPath(vg);
    nvgCircle(vg, cx,cy, r0-1);
    nvgCircle(vg, cx,cy, r1+1);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,64));
    nvgStrokeWidth(vg, 1.0f);
    nvgStroke(vg);

    // Selector
    nvgSave(vg);
    nvgTranslate(vg, cx,cy);
    nvgRotate(vg, hue*NVG_PI*2);

    // Marker on
    nvgStrokeWidth(vg, 2.0f);
    nvgBeginPath(vg);
    nvgRect(vg, r0-1,-3,r1-r0+2,6);
    nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
    nvgStroke(vg);

    paint = nvgBoxGradient(vg, r0-3,-5,r1-r0+6,10, 2,4, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, r0-3-10,-5-10,r1-r0+6+20,10+20);
    nvgRect(vg, r0-3,-5,r1-r0+6,10);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    // Center triangle
    r = r0 - 6;
    ax = cosf(120.0f/180.0f*NVG_PI) * r;
    ay = sinf(120.0f/180.0f*NVG_PI) * r;
    bx = cosf(-120.0f/180.0f*NVG_PI) * r;
    by = sinf(-120.0f/180.0f*NVG_PI) * r;
    nvgBeginPath(vg);
    nvgMoveTo(vg, r,0);
    nvgLineTo(vg, ax,ay);
    nvgLineTo(vg, bx,by);
    nvgClosePath(vg);
    paint = nvgLinearGradient(vg, r,0, ax,ay, nvgHSLA(hue,1.0f,0.5f,255), nvgRGBA(255,255,255,255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    paint = nvgLinearGradient(vg, (r+ax)*0.5f,(0+ay)*0.5f, bx,by, nvgRGBA(0,0,0,0), nvgRGBA(0,0,0,255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,64));
    nvgStroke(vg);

    // Select circle on triangle
    ax = cosf(120.0f/180.0f*NVG_PI) * r*0.3f;
    ay = sinf(120.0f/180.0f*NVG_PI) * r*0.4f;
    nvgStrokeWidth(vg, 2.0f);
    nvgBeginPath(vg);
    nvgCircle(vg, ax,ay,5);
    nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
    nvgStroke(vg);

    paint = nvgRadialGradient(vg, ax,ay, 7,9, nvgRGBA(0,0,0,64), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, ax-20,ay-20,40,40);
    nvgCircle(vg, ax,ay,7);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    nvgRestore(vg);

    nvgRestore(vg);
}

uint64_t timerAbsoluteTime();

void drag(uint32_t areaID, int* x, int* y, int dx, int dy, int* dragInProgress)
{
    assert(x);
    assert(y);
    assert(dragInProgress);

    uint32_t id = ui::mouseOverID();

    if (ui::mouseWasPressed(SDL_BUTTON_LEFT) && id==areaID)
    {
        int csx, csy;

        ui::mouseAbsOffset(&csx, &csy);
        dx = *x - csx;
        dy = *y - csy;

        *dragInProgress = true;
    }
    else if (ui::mouseIsPressed(SDL_BUTTON_LEFT) && *dragInProgress)
    {
        int csx, csy;

        ui::mouseAbsOffset(&csx, &csy);
        *x = csx + dx;
        *y = csy + dy;

        if (ui::mouseWasReleased(SDL_BUTTON_LEFT))
        {
            *dragInProgress = false;
        }
    }
}

void drawButton(struct NVGcontext* vg, ui::AreaDesc* area)
{
    drawButton(vg, area->preicon, area->text, area->x, area->y, area->w, area->h, area->state);
}

ui::AreaDesc loginButton  = {ICON_LOGIN, "Sign in", 0, 0, 140, 28, BUTTON_ACTION};
ui::AreaDesc deleteButton = {ICON_TRASH, "Delete",  0, 0, 160, 28, BUTTON_ALERT};
ui::AreaDesc cancelButton = {         0, "Cancel",  0, 0, 110, 28, 0};


class UIDemo: public ui::Stage
{
private:
    SpectatorCamera     camera;
    glm::mat4	mProj;
    int images[12];
    int fontNormal, fontBold, fontIcons; 
    memory_t f1, f2, f3;

public:
    UIDemo()
    {
        memory_t fileData;

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        for (size_t i = 0; i < 12; i++)
        {
            char file[128];
            _snprintf(file, 128, "uidemo/images/image%d.jpg", i+1);
            if (mopen(&fileData, file))
            {
                images[i] = nvgCreateImageMem(vg::ctx, fileData.buffer, fileData.allocated, 0);
                mfree(&fileData);
            }
            if (images[i] == 0)
            {
                printf("Could not load %s.\n", file);
            }
        }

        if (mopen(&f1, "uidemo/entypo.ttf"))
        {
            fontIcons = nvgCreateFontMem(vg::ctx, "icons", f1.buffer, f1.allocated, 0);
        }
        if (fontIcons == -1)
        {
            printf("Could not add font icons.\n");
        }
        if (mopen(&f2, "uidemo/Roboto-Regular.ttf"))
        {
            //fontNormal = nvgAddFont(vg, "sans", "../example/FiraSans-Regular.ttf");
            fontNormal = nvgCreateFontMem(vg::ctx, "sans", f2.buffer, f2.allocated, 0);
        }
        if (fontNormal == -1)
        {
            printf("Could not add font italic.\n");
        }
        if (mopen(&f3, "uidemo/Roboto-Bold.ttf"))
        {
            //fontBold = nvgAddFont(vg, "sans-bold", "../example/FiraSans-Bold.ttf");
            fontBold = nvgCreateFontMem(vg::ctx, "sans-bold", f3.buffer, f3.allocated, 0);
        }
        if (fontBold == -1)
        {
            printf("Could not add font bold.\n");
        }

        activeView    = ViewMain;
        showEffects   = false;
        checkboxValue = false;

        wx1 = 51.0f;
        wy1 = 51.0f;
        ww  = 300.0f - 2.0f;
        wh = 30.0f;

        sliderPos = 0;
    }

    ~UIDemo()
    {
        for (size_t i = 0; i < 12; i++)
            nvgDeleteImage(vg::ctx, images[i]);

        mfree(&f1);
        mfree(&f2);
        mfree(&f3);
    }

protected:
    enum EnumView
    {
        ViewMain,
        ViewThumbnails,
        ViewDragWindow,
        ViewDragSlider,
        ViewWaitForRelease
    };

    EnumView activeView;
    bool showEffects;
    bool checkboxValue;
    float wx1, wy1, ww, wh;

    void onPaint()
    {
        double mx, my;
        int width, height, xx, yy;

        glPushAttrib(GL_ALL_ATTRIB_BITS);

        ui::mouseAbsOffset(&xx, &yy);

        mx = xx; my = yy;

        //glfwGetFramebufferSize(window, &width, &height);
        width  = mWidth;
        height = mHeight;

        // Update and render
        glViewport(0, 0, width, height);
        glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_2D);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,width,height,0,-1,1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);
        glColor4ub(255,255,255,255);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        float t = timerAbsoluteTime() / 1000000.0f;
        float x,y,popy;

        nvgBeginFrame(vg::ctx);

        drawEyes(vg::ctx, width - 250, 50, 150, 100, mx, my, t);
        drawGraph(vg::ctx, 0, height/2, width, height/2, t);
        drawColorwheel(vg::ctx, width - 300, height - 300, 250.0f, 250.0f, t);

        nvgSave(vg::ctx);

        // Widgets
        drawWindow(vg::ctx, "Widgets `n Stuff", wx1, wy1, 300, 400);
        x = wx1 + 10; y = wy1 + 45;
        drawSearchBox(vg::ctx, "Search", x,y,280,25);
        y += 40;
        drawDropDown(vg::ctx, "Effects", x,y,280,28);
        popy = y + 14;
        y += 45;

        // Form
        drawLabel(vg::ctx, "Login", x,y, 280,20);
        y += 25;
        drawEditBox(vg::ctx, "Email",  x,y, 280,28);
        y += 35;
        drawEditBox(vg::ctx, "Password", x,y, 280,28);
        y += 38;
        drawCheckBox(vg::ctx, "Remember me", x,y, 140,28, checkboxValue);
        drawButton(vg::ctx, &loginButton);
        y += 45;

        // Slider
        drawLabel(vg::ctx, "Diameter", x,y, 280,20);
        y += 25;
        drawEditBoxNum(vg::ctx, "123.00", "px", x+180,y, 100,28);
        drawSlider(vg::ctx, sliderPos, x,y, 170,28);
        y += 55;

        drawButton(vg::ctx, &deleteButton);
        drawButton(vg::ctx, &cancelButton);

        if (showEffects)
        {
            // Thumbnails box
            drawThumbnails(vg::ctx, wx1 + 315, popy-30, 160, 300, images, 12, t);
        }

        nvgRestore(vg::ctx);

        glEnable(GL_DEPTH_TEST);
        glPopAttrib();

        CHECK_GL_ERROR();
    }

    int dsx, dsy;
    int sliderPos;

    bool hitTest(float x, float y, float x0, float y0, float x1, float y1)
    {
        return (x0<=x) && (x<=x1) && (y0<=y) && (y<=y1);
    }

    bool buttonLogic(float mx, float my, ui::AreaDesc* area)
    {
        area->state &= ~3;
        if (hitTest(mx, my, area->x, area->y, area->x+area->w, area->y+area->h))
        {
            if (ui::mouseIsPressed(SDL_BUTTON_LEFT))
                area->state |= BUTTON_PRESSED;
            else
                area->state |= BUTTON_FOCUSED;

            if (ui::mouseWasReleased(SDL_BUTTON_LEFT))
                return true;
        }

        return false;
    }

    void onUpdate(float dt)
    {
        int mx, my;

        ui::mouseAbsOffset(&mx, &my);

        if (activeView == ViewThumbnails)
        {
            if (ui::mouseWasPressed(SDL_BUTTON_LEFT) && !hitTest(mx-wx1, my-wy1, 315, 69, 475, 369))
            {
                showEffects = false;
                activeView = ViewWaitForRelease;
            }
            else
            {
                //draw selection
            }
        }
        else if (activeView==ViewWaitForRelease)
        {
            activeView = ui::mouseWasReleased(SDL_BUTTON_LEFT) ? ViewMain : ViewWaitForRelease;
        }
        else if (activeView == ViewDragWindow)
        {
            int csx, csy;

            ui::mouseAbsOffset(&csx, &csy);
            wx1 = float(csx + dsx);
            wy1 = float(csy + dsy);

            if (ui::mouseWasReleased(SDL_BUTTON_LEFT))
            {
                activeView = ViewMain;
            }
        }
        else if (activeView == ViewDragSlider)
        {
            int csx, csy;

            ui::mouseAbsOffset(&csx, &csy);
            sliderPos = float(csx + dsx);

            if (ui::mouseWasReleased(SDL_BUTTON_LEFT))
            {
                sliderPos = std::max(sliderPos,   0);
                sliderPos = std::min(sliderPos, 170);

                activeView = ViewMain;
            }
        }
        else if (activeView == ViewMain)
        {
            if (ui::mouseWasReleased(SDL_BUTTON_LEFT) && hitTest(mx-wx1, my-wy1, 10, 85, 290, 113))
            {
                activeView  = ViewThumbnails;
                showEffects = true;
            }

            if (ui::mouseWasReleased(SDL_BUTTON_LEFT) && hitTest(mx-wx1, my-wy1, 10, 228, 135, 256))
            {
                checkboxValue = !checkboxValue;
            }

            buttonLogic(mx, my, &loginButton);
            buttonLogic(mx, my, &deleteButton);
            buttonLogic(mx, my, &cancelButton);

            if (ui::mouseWasPressed(SDL_BUTTON_LEFT) && hitTest(mx-wx1, my-wy1, 0, 0, ww, wh))
            {
                dsx = wx1 - mx;
                dsy = wy1 - my;

                activeView = ViewDragWindow;
            }

            if (ui::mouseWasPressed(SDL_BUTTON_LEFT) && hitTest(mx-wx1, my-wy1, 4+sliderPos, 306, 16+sliderPos, 318))
            {
                int csx, csy;

                ui::mouseAbsOffset(&csx, &csy);
                dsx = sliderPos - csx;

                activeView = ViewDragSlider;
            }

        }
        else assert(0);
 
        float x,y,popy;
        x = wx1 + 10; y = wy1 + 45;
        y += 40;
        popy = y + 14;
        y += 45;
        y += 25;
        y += 35;
        y += 38;
        loginButton.x = x + 138;
        loginButton.y = y;
        //drawButton(vg, ICON_LOGIN, "Sign in", x+138, y,    140, 28, BUTTON_ACTION);
        y += 45;
        y += 25;
        y += 55;
        deleteButton.x = x;
        deleteButton.y = y;
        cancelButton.x = x+170;
        cancelButton.y = y;

        //drawButton(vg, ICON_TRASH, "Delete", x, y, 160, 28, BUTTON_ALERT);
        //drawButton(vg, 0, "Cancel", x+170, y, 110, 28, 0);
   }
};

int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    {
        UIDemo app;
        app.run();
    }

    fwk::cleanup();

    return 0;
}