#define _CRT_SECURE_NO_WARNINGS

#include <fwk/fwk.h>
#include <stb_image_write.h>

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
        unsigned int    state;
    };
}

struct WindowState
{
    int visible:1;
    int minimized:1;
};

static const size_t MAX_UI_ELEMENTS = 64;

size_t usedElements;
uint16_t parent[MAX_UI_ELEMENTS];
ml::vec2 rpos[MAX_UI_ELEMENTS];
ml::vec2 pos[MAX_UI_ELEMENTS];
ml::vec2 dim[MAX_UI_ELEMENTS];

size_t activeAreaCount;
uint16_t activeArea[MAX_UI_ELEMENTS];

void resetHierData()
{
    usedElements = 0;
}

void updateHierData()
{
    if (usedElements < 1)
        return;

    assert(parent[0]==0xFFFF);
    for (size_t i = 0; i < usedElements; ++i)
    {
        size_t p = parent[i];
        if (p != 0xFFFF)
        {
            pos[i].x = pos[p].x+rpos[i].x;
            pos[i].y = pos[p].y+rpos[i].y;
        }
        else
        {
            pos[i].x = rpos[i].x;
            pos[i].y = rpos[i].y;
        }
    }
}

enum PGID
{
    PGID_WINDOW,
    PGID_CAPTION,
    PGID_MINMAXBTN,
    PGID_CLOSEBTN,
    PGID_CAT1,
    PGID_PROP1,
    PGID_PROP2,
    PGID_CAT2,
    PGID_PROP3,
    PGID_PROP4,
};

void layoutPropGrid(WindowState* ws, struct NVGcontext* vg, float x, float y, float w, float h)
{
    float lineh;
    float ascHdrFont, descHdrFont;
    float ascCatFont, descCatFont;

    float cornerRadius;
    float wndMargin, btnMargin, catMargin;
    float textHhrMargin, textCatMargin;
    float headerWidth, headerHeight;
    float categoryWidth, categoryHeight;
    float buttonSize;

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextMetrics(vg, &ascHdrFont, &descHdrFont, &lineh);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextMetrics(vg, &ascCatFont, &descCatFont, &lineh);

    cornerRadius  = 5.0f;
    wndMargin     = cornerRadius + 3.0f;
    btnMargin     = 7.0f;
    catMargin     = 3.0f;
    textHhrMargin = 7.0f;
    textCatMargin = 3.0f;

    headerHeight  = ascHdrFont - descHdrFont + textHhrMargin*2.0f;
    buttonSize  = headerHeight - btnMargin;
    headerWidth   = w - (buttonSize + btnMargin + wndMargin) * 2.0f;

    categoryWidth  = w - (wndMargin+catMargin) * 2.0f;
    categoryHeight = ascCatFont - descCatFont + textCatMargin*2.0f;

    resetHierData();

    activeAreaCount = 0;

    if (!ws->visible) return;

    if (ws->minimized) h = headerHeight + wndMargin;

    usedElements = PGID_CLOSEBTN+1;

    parent[PGID_WINDOW] = 0xFFFF;
    rpos[PGID_WINDOW].x = x;
    rpos[PGID_WINDOW].y = y;
    dim[PGID_WINDOW].x = w;
    dim[PGID_WINDOW].y = h;

    parent[PGID_CAPTION] = PGID_WINDOW;
    rpos[PGID_CAPTION].x = wndMargin;
    rpos[PGID_CAPTION].y = 0;
    dim[PGID_CAPTION].x = headerWidth;
    dim[PGID_CAPTION].y = headerHeight;

    parent[PGID_MINMAXBTN] = PGID_WINDOW;
    rpos[PGID_MINMAXBTN].x = wndMargin+headerWidth+btnMargin;
    rpos[PGID_MINMAXBTN].y = 0;
    dim[PGID_MINMAXBTN].x = buttonSize;
    dim[PGID_MINMAXBTN].y = buttonSize;

    parent[PGID_CLOSEBTN] = PGID_WINDOW;
    rpos[PGID_CLOSEBTN].x = wndMargin+headerWidth+btnMargin+buttonSize;
    rpos[PGID_CLOSEBTN].y = 0;
    dim[PGID_CLOSEBTN].x = buttonSize;
    dim[PGID_CLOSEBTN].y = buttonSize;

    activeArea[activeAreaCount++] = PGID_CAPTION;
    activeArea[activeAreaCount++] = PGID_MINMAXBTN;
    activeArea[activeAreaCount++] = PGID_CLOSEBTN;

    if (!ws->minimized)
    {
        usedElements = PGID_CAT1+1;

        parent[PGID_CAT1] = PGID_WINDOW;
        rpos[PGID_CAT1].x = wndMargin+catMargin;
        rpos[PGID_CAT1].y = headerHeight+catMargin;
        dim[PGID_CAT1].x = categoryWidth;
        dim[PGID_CAT1].y = categoryHeight;

        activeArea[activeAreaCount++] = PGID_CAT1;
    }

    updateHierData();
}

enum GraphrenderStyle {
    GRAPH_RENDER_FPS,
    GRAPH_RENDER_MS,
};

#define GRAPH_HISTORY_COUNT 100
struct PerfGraph {
    int style;
    char name[32];
    float values[GRAPH_HISTORY_COUNT];
    int head;
};
typedef struct PerfGraph PerfGraph;

void initGraph(PerfGraph* fps, int style, const char* name);
void updateGraph(PerfGraph* fps, float frameTime);
void renderGraph(NVGcontext* vg, float x, float y, PerfGraph* fps);
float getGraphAverage(PerfGraph* fps);

void initGraph(PerfGraph* fps, int style, const char* name)
{
    memset(fps, 0, sizeof(PerfGraph));
    fps->style = style;
    strncpy(fps->name, name, sizeof(fps->name));
    fps->name[sizeof(fps->name)-1] = '\0';
}

void updateGraph(PerfGraph* fps, float frameTime)
{
    fps->head = (fps->head+1) % GRAPH_HISTORY_COUNT;
    fps->values[fps->head] = frameTime;
}

float getGraphAverage(PerfGraph* fps)
{
    int i;
    float avg = 0;
    for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
        avg += fps->values[i];
    }
    return avg / (float)GRAPH_HISTORY_COUNT;
}

void renderGraph(NVGcontext* vg, float x, float y, PerfGraph* fps)
{
    int i;
    float avg, w, h;
    char str[64];

    avg = getGraphAverage(fps);

    w = 200;
    h = 35;

    nvgBeginPath(vg);
    nvgRect(vg, x,y, w,h);
    nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgMoveTo(vg, x, y+h);
    if (fps->style == GRAPH_RENDER_FPS) {
        for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
            float v = 1.0f / (0.00001f + fps->values[(fps->head+i) % GRAPH_HISTORY_COUNT]);
            float vx, vy;
            if (v > 80.0f) v = 80.0f;
            vx = x + ((float)i/(GRAPH_HISTORY_COUNT-1)) * w;
            vy = y + h - ((v / 80.0f) * h);
            nvgLineTo(vg, vx, vy);
        }
    } else {
        for (i = 0; i < GRAPH_HISTORY_COUNT; i++) {
            float v = fps->values[(fps->head+i) % GRAPH_HISTORY_COUNT] * 1000.0f;
            float vx, vy;
            if (v > 20.0f) v = 20.0f;
            vx = x + ((float)i/(GRAPH_HISTORY_COUNT-1)) * w;
            vy = y + h - ((v / 20.0f) * h);
            nvgLineTo(vg, vx, vy);
        }
    }
    nvgLineTo(vg, x+w, y+h);
    nvgFillColor(vg, nvgRGBA(255,192,0,128));
    nvgFill(vg);

    nvgFontFace(vg, "sans");

    if (fps->name[0] != '\0') {
        nvgFontSize(vg, 14.0f);
        nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(240,240,240,192));
        nvgText(vg, x+3,y+1, fps->name, NULL);
    }

    if (fps->style == GRAPH_RENDER_FPS) {
        nvgFontSize(vg, 18.0f);
        nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(240,240,240,255));
        sprintf(str, "%.2f FPS", 1.0f / avg);
        nvgText(vg, x+w-3,y+1, str, NULL);

        nvgFontSize(vg, 15.0f);
        nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_BOTTOM);
        nvgFillColor(vg, nvgRGBA(240,240,240,160));
        sprintf(str, "%.2f ms", avg * 1000.0f);
        nvgText(vg, x+w-3,y+h-1, str, NULL);
    } else {
        nvgFontSize(vg, 18.0f);
        nvgTextAlign(vg,NVG_ALIGN_RIGHT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(240,240,240,255));
        sprintf(str, "%.2f ms", avg * 1000.0f);
        nvgText(vg, x+w-3,y+1, str, NULL);
    }
}

struct DemoData {
    int fontNormal, fontBold, fontIcons; 
    int images[12];
};
typedef struct DemoData DemoData;

int loadDemoData(NVGcontext* vg, DemoData* data);
void freeDemoData(NVGcontext* vg, DemoData* data);
void renderDemo(NVGcontext* vg, float mx, float my, float width, float height, float t, int blowup, DemoData* data);

void saveScreenShot(int w, int h, int premult, const char* name);


#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729

//static float minf(float a, float b) { return a < b ? a : b; }
static float maxf(float a, float b) { return a > b ? a : b; }
//static float absf(float a) { return a >= 0.0f ? a : -a; }
static float clampf(float a, float mn, float mx) { return a < mn ? mn : (a > mx ? mx : a); }

// Returns 1 if col.rgba is 0.0f,0.0f,0.0f,0.0f, 0 otherwise
int isBlack(NVGcolor col)
{
    if( col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f )
    {
        return 1;
    }
    return 0;
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


void drawWindow(NVGcontext* vg, const char* title, float x, float y, float w, float h)
{
    float cornerRadius = 3.0f;
    NVGpaint shadowPaint;
    NVGpaint headerPaint;

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

void drawSearchBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
    NVGpaint bg;
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

void drawDropDown(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
    NVGpaint bg;
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

void drawLabel(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
    NVG_NOTUSED(w);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,128));

    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x,y+h*0.5f,text, NULL);
}

void drawEditBoxBase(NVGcontext* vg, float x, float y, float w, float h)
{
    NVGpaint bg;
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

void drawEditBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{

    drawEditBoxBase(vg, x,y, w,h);

    nvgFontSize(vg, 20.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255,255,255,64));
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+h*0.3f,y+h*0.5f,text, NULL);
}

void drawEditBoxNum(NVGcontext* vg,
    const char* text, const char* units, float x, float y, float w, float h)
{
    float uw;

    drawEditBoxBase(vg, x,y, w,h);

    uw = nvgTextBounds(vg, 0,0, units, NULL, NULL);

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

void drawCheckBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
    NVGpaint bg;
    char icon[8];
    NVG_NOTUSED(w);

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

    nvgFontSize(vg, 40);
    nvgFontFace(vg, "icons");
    nvgFillColor(vg, nvgRGBA(255,255,255,128));
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+9+2, y+h*0.5f, cpToUTF8(ICON_CHECK,icon), NULL);
}

void drawButton(NVGcontext* vg, int preicon, const char* text, float x, float y, float w, float h, NVGcolor col)
{
    NVGpaint bg;
    char icon[8];
    float cornerRadius = 4.0f;
    float tw = 0, iw = 0;

    bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,isBlack(col)?16:32), nvgRGBA(0,0,0,isBlack(col)?16:32));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
    if (!isBlack(col)) {
        nvgFillColor(vg, col);
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
    tw = nvgTextBounds(vg, 0,0, text, NULL, NULL);
    if (preicon != 0) {
        nvgFontSize(vg, h*1.3f);
        nvgFontFace(vg, "icons");
        iw = nvgTextBounds(vg, 0,0, cpToUTF8(preicon,icon), NULL, NULL);
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

void drawSlider(NVGcontext* vg, float pos, float x, float y, float w, float h)
{
    NVGpaint bg, knob;
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

    // Knob
    knob = nvgLinearGradient(vg, x,cy-kr,x,cy+kr, nvgRGBA(255,255,255,16), nvgRGBA(0,0,0,16));
    nvgBeginPath(vg);
    nvgCircle(vg, x+(int)(pos*w),cy, kr-1);
    nvgFillColor(vg, nvgRGBA(40,43,48,255));
    nvgFill(vg);
    nvgFillPaint(vg, knob);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgCircle(vg, x+(int)(pos*w),cy, kr-0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,92));
    nvgStroke(vg);

    nvgRestore(vg);
}

void drawEyes(NVGcontext* vg, float x, float y, float w, float h, float mx, float my, float t)
{
    NVGpaint gloss, bg;
    float ex = w *0.23f;
    float ey = h * 0.5f;
    float lx = x + ex;
    float ly = y + ey;
    float rx = x + w - ex;
    float ry = y + ey;
    float dx,dy,d;
    float br = (ex < ey ? ex : ey) * 0.5f;
    float blink = 1 - ml::pow(ml::sin(t*0.5f),200)*0.8f;

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
    d = ml::sqrt(dx*dx+dy*dy);
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
    d = ml::sqrt(dx*dx+dy*dy);
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

void drawGraph(NVGcontext* vg, float x, float y, float w, float h, float t)
{
    NVGpaint bg;
    float samples[6];
    float sx[6], sy[6];
    float dx = w/5.0f;
    int i;

    samples[0] = (1+ml::sin(t*1.2345f +ml::cos(t*0.33457f)*0.44f))*0.5f;
    samples[1] = (1+ml::sin(t*0.68363f+ml::cos(t*1.3f    )*1.55f))*0.5f;
    samples[2] = (1+ml::sin(t*1.1642f +ml::cos(t*0.33457f)*1.24f))*0.5f;
    samples[3] = (1+ml::sin(t*0.56345f+ml::cos(t*1.63f   )*0.14f))*0.5f;
    samples[4] = (1+ml::sin(t*1.6245f +ml::cos(t*0.254f  )*0.3f ))*0.5f;
    samples[5] = (1+ml::sin(t*0.345f  +ml::cos(t*0.03f   )*0.6f ))*0.5f;

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

void drawSpinner(NVGcontext* vg, float cx, float cy, float r, float t)
{
    float a0 = 0.0f + t*6;
    float a1 = NVG_PI + t*6;
    float r0 = r;
    float r1 = r * 0.75f;
    float ax,ay, bx,by;
    NVGpaint paint;

    nvgSave(vg);

    nvgBeginPath(vg);
    nvgArc(vg, cx,cy, r0, a0, a1, NVG_CW);
    nvgArc(vg, cx,cy, r1, a1, a0, NVG_CCW);
    nvgClosePath(vg);
    ax = cx + ml::cos(a0) * (r0+r1)*0.5f;
    ay = cy + ml::sin(a0) * (r0+r1)*0.5f;
    bx = cx + ml::cos(a1) * (r0+r1)*0.5f;
    by = cy + ml::sin(a1) * (r0+r1)*0.5f;
    paint = nvgLinearGradient(vg, ax,ay, bx,by, nvgRGBA(0,0,0,0), nvgRGBA(0,0,0,128));
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    nvgRestore(vg);
}

void drawThumbnails(NVGcontext* vg, float x, float y, float w, float h, const int* images, int nimages, float t)
{
    float cornerRadius = 3.0f;
    NVGpaint shadowPaint, imgPaint, fadePaint;
    float ix,iy,iw,ih;
    float thumb = 60.0f;
    float arry = 30.5f;
    int imgw, imgh;
    float stackh = (nimages/2) * (thumb+10) + 10;
    int i;
    float u  = (1+ml::cos(t*0.5f))*0.5f;
    float u2 = (1-ml::cos(t*0.2f))*0.5f;
    float scrollh, dv;

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

    dv = 1.0f / (float)(nimages-1);

    for (i = 0; i < nimages; i++) {
        float tx, ty, v, a;
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

        v = i * dv;
        a = clampf((u2-v) / dv, 0, 1);

        if (a < 1.0f)
            drawSpinner(vg, tx+thumb/2,ty+thumb/2, thumb*0.25f, t);

        imgPaint = nvgImagePattern(vg, tx+ix, ty+iy, iw,ih, 0.0f/180.0f*NVG_PI, images[i], NVG_NOREPEAT, a);
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

    scrollh = (h/stackh) * (h-8);
    shadowPaint = nvgBoxGradient(vg, x+w-12-1,y+4+(h-8-scrollh)*u-1, 8,scrollh, 3,4, nvgRGBA(220,220,220,255), nvgRGBA(128,128,128,255));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+w-12+1,y+4+1 + (h-8-scrollh)*u, 8-2,scrollh-2, 2);
    nvgFillPaint(vg, shadowPaint);
    //	nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgFill(vg);

    nvgRestore(vg);
}

void drawColorwheel(NVGcontext* vg, float x, float y, float w, float h, float t)
{
    int i;
    float r0, r1, ax,ay, bx,by, cx,cy, aeps, r;
    float hue = ml::sin(t * 0.12f);
    NVGpaint paint;

    nvgSave(vg);

    /*	nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
    nvgFillColor(vg, nvgRGBA(255,0,0,128));
    nvgFill(vg);*/

    cx = x + w*0.5f;
    cy = y + h*0.5f;
    r1 = (w < h ? w : h) * 0.5f - 5.0f;
    r0 = r1 - 20.0f;
    aeps = 0.5f / r1;   // half a pixel arc length in radians (2pi cancels out).

    for (i = 0; i < 6; i++) {
        float a0 = (float)i / 6.0f * NVG_PI * 2.0f - aeps;
        float a1 = (float)(i+1.0f) / 6.0f * NVG_PI * 2.0f + aeps;
        nvgBeginPath(vg);
        nvgArc(vg, cx,cy, r0, a0, a1, NVG_CW);
        nvgArc(vg, cx,cy, r1, a1, a0, NVG_CCW);
        nvgClosePath(vg);
        ax = cx + ml::cos(a0) * (r0+r1)*0.5f;
        ay = cy + ml::sin(a0) * (r0+r1)*0.5f;
        bx = cx + ml::cos(a1) * (r0+r1)*0.5f;
        by = cy + ml::sin(a1) * (r0+r1)*0.5f;
        paint = nvgLinearGradient(vg, ax,ay, bx,by, nvgHSLA(a0/(NVG_PI*2),1.0f,0.55f,255), nvgHSLA(a1/(NVG_PI*2),1.0f,0.55f,255));
        nvgFillPaint(vg, paint);
        nvgFill(vg);
    }

    nvgBeginPath(vg);
    nvgCircle(vg, cx,cy, r0-0.5f);
    nvgCircle(vg, cx,cy, r1+0.5f);
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
    nvgRect(vg, r0-2-10,-4-10,r1-r0+4+20,8+20);
    nvgRect(vg, r0-2,-4,r1-r0+4,8);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    // Center triangle
    r = r0 - 6;
    ax = ml::cos( 120.0f/180.0f*NVG_PI) * r;
    ay = ml::sin( 120.0f/180.0f*NVG_PI) * r;
    bx = ml::cos(-120.0f/180.0f*NVG_PI) * r;
    by = ml::sin(-120.0f/180.0f*NVG_PI) * r;
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
    ax = ml::cos(120.0f/180.0f*NVG_PI) * r*0.3f;
    ay = ml::sin(120.0f/180.0f*NVG_PI) * r*0.4f;
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

void drawLines(NVGcontext* vg, float x, float y, float w, float h, float t)
{
    int i, j;
    float pad = 5.0f, s = w/9.0f - pad*2;
    float pts[4*2], fx, fy;
    int joins[3] = {NVG_MITER, NVG_ROUND, NVG_BEVEL};
    int caps[3] = {NVG_BUTT, NVG_ROUND, NVG_SQUARE};
    NVG_NOTUSED(h);

    nvgSave(vg);
    pts[0] = -s*0.25f + ml::cos(t*0.3f) * s*0.5f;
    pts[1] = ml::sin(t*0.3f) * s*0.5f;
    pts[2] = -s*0.25;
    pts[3] = 0;
    pts[4] = s*0.25f;
    pts[5] = 0;
    pts[6] = s*0.25f + ml::cos(-t*0.3f) * s*0.5f;
    pts[7] = ml::sin(-t*0.3f) * s*0.5f;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            fx = x + s*0.5f + (i*3+j)/9.0f*w + pad;
            fy = y - s*0.5f + pad;

            nvgLineCap(vg, caps[i]);
            nvgLineJoin(vg, joins[j]);

            nvgStrokeWidth(vg, s*0.3f);
            nvgStrokeColor(vg, nvgRGBA(0,0,0,160));
            nvgBeginPath(vg);
            nvgMoveTo(vg, fx+pts[0], fy+pts[1]);
            nvgLineTo(vg, fx+pts[2], fy+pts[3]);
            nvgLineTo(vg, fx+pts[4], fy+pts[5]);
            nvgLineTo(vg, fx+pts[6], fy+pts[7]);
            nvgStroke(vg);

            nvgLineCap(vg, NVG_BUTT);
            nvgLineJoin(vg, NVG_BEVEL);

            nvgStrokeWidth(vg, 1.0f);
            nvgStrokeColor(vg, nvgRGBA(0,192,255,255));
            nvgBeginPath(vg);
            nvgMoveTo(vg, fx+pts[0], fy+pts[1]);
            nvgLineTo(vg, fx+pts[2], fy+pts[3]);
            nvgLineTo(vg, fx+pts[4], fy+pts[5]);
            nvgLineTo(vg, fx+pts[6], fy+pts[7]);
            nvgStroke(vg);
        }
    }


    nvgRestore(vg);
}

int loadDemoData(NVGcontext* vg, DemoData* data)
{
    int i;

    if (vg == NULL)
        return -1;

    memory_t f1, f2, f3;
    memory_t fileData;

    for (size_t i = 0; i < 12; i++)
    {
        char file[128];
        _snprintf(file, 128, "uidemo/images/image%d.jpg", i+1);
        if (mem_file(&fileData, file))
        {
            data->images[i] = nvgCreateImageMem(vg::ctx, 0, fileData.buffer, fileData.size);
            mem_free(&fileData);
        }
        if (data->images[i] == 0)
        {
            printf("Could not load %s.\n", file);
        }
    }

    if (mem_file(&f1, "uidemo/entypo.ttf"))
    {
        data->fontIcons = nvgCreateFontMem(vg::ctx, "icons", f1.buffer, f1.size, 0);
    }
    if (data->fontIcons == -1)
    {
        printf("Could not add font icons.\n");
    }
    if (mem_file(&f2, "uidemo/Roboto-Regular.ttf"))
    {
        //fontNormal = nvgAddFont(vg, "sans", "../example/FiraSans-Regular.ttf");
        data->fontNormal = nvgCreateFontMem(vg::ctx, "sans", f2.buffer, f2.size, 0);
    }
    if (data->fontNormal == -1)
    {
        printf("Could not add font italic.\n");
    }
    if (mem_file(&f3, "uidemo/Roboto-Bold.ttf"))
    {
        //fontBold = nvgAddFont(vg, "sans-bold", "../example/FiraSans-Bold.ttf");
        data->fontBold = nvgCreateFontMem(vg::ctx, "sans-bold", f3.buffer, f3.size, 0);
    }
    if (data->fontBold == -1)
    {
        printf("Could not add font bold.\n");
    }
    return 0;
}

void freeDemoData(NVGcontext* vg, DemoData* data)
{
    int i;

    if (vg == NULL)
        return;

    for (i = 0; i < 12; i++)
        nvgDeleteImage(vg, data->images[i]);
}

void drawParagraph(NVGcontext* vg, float x, float y, float width, float height, float mx, float my)
{
    NVGtextRow rows[3];
    NVGglyphPosition glyphs[100];
    const char* text = "This is longer chunk of text.\n  \n  Would have used lorem ipsum but she    was busy jumping over the lazy dog with the fox and all the men who came to the aid of the party.";
    const char* start;
    const char* end;
    int nrows, i, nglyphs, j, lnum = 0;
    float lineh;
    float caretx, px;
    float bounds[4];
    float a;
    float gx,gy;
    int gutter = 0;
    NVG_NOTUSED(height);

    nvgSave(vg);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans");
    nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgTextMetrics(vg, NULL, NULL, &lineh);

    // The text break API can be used to fill a large buffer of rows,
    // or to iterate over the text just few lines (or just one) at a time.
    // The "next" variable of the last returned item tells where to continue.
    start = text;
    end = text + strlen(text);
    while ((nrows = nvgTextBreakLines(vg, start, end, width, rows, 3))) {
        for (i = 0; i < nrows; i++) {
            NVGtextRow* row = &rows[i];
            int hit = mx > x && mx < (x+width) && my >= y && my < (y+lineh);

            nvgBeginPath(vg);
            nvgFillColor(vg, nvgRGBA(255,255,255,hit?64:16));
            nvgRect(vg, x, y, row->width, lineh);
            nvgFill(vg);

            nvgFillColor(vg, nvgRGBA(255,255,255,255));
            nvgText(vg, x, y, row->start, row->end);

            if (hit) {
                caretx = (mx < x+row->width/2) ? x : x+row->width;
                px = x;
                nglyphs = nvgTextGlyphPositions(vg, x, y, row->start, row->end, glyphs, 100);
                for (j = 0; j < nglyphs; j++) {
                    float x0 = glyphs[j].x;
                    float x1 = (j+1 < nglyphs) ? glyphs[j+1].x : x+row->width;
                    float gx = x0 * 0.3f + x1 * 0.7f;
                    if (mx >= px && mx < gx)
                        caretx = glyphs[j].x;
                    px = gx;
                }
                nvgBeginPath(vg);
                nvgFillColor(vg, nvgRGBA(255,192,0,255));
                nvgRect(vg, caretx, y, 1, lineh);
                nvgFill(vg);

                gutter = lnum+1;
                gx = x - 10;
                gy = y + lineh/2;
            }
            lnum++;
            y += lineh;
        }
        // Keep going...
        start = rows[nrows-1].next;
    }

    if (gutter) {
        char txt[16];
        snprintf(txt, sizeof(txt), "%d", gutter);
        nvgFontSize(vg, 13.0f);
        nvgTextAlign(vg, NVG_ALIGN_RIGHT|NVG_ALIGN_MIDDLE);

        nvgTextBounds(vg, gx,gy, txt, NULL, bounds);

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGBA(255,192,0,255));
        nvgRoundedRect(vg, (int)bounds[0]-4,(int)bounds[1]-2, (int)(bounds[2]-bounds[0])+8, (int)(bounds[3]-bounds[1])+4, ((int)(bounds[3]-bounds[1])+4)/2-1);
        nvgFill(vg);

        nvgFillColor(vg, nvgRGBA(32,32,32,255));
        nvgText(vg, gx,gy, txt, NULL);
    }

    y += 20.0f;

    nvgFontSize(vg, 13.0f);
    nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgTextLineHeight(vg, 1.2f);

    nvgTextBoxBounds(vg, x,y, 150, "Hover your mouse over the text to see calculated caret position.", NULL, bounds);

    // Fade the tooltip out when close to it.
    gx = ml::abs((mx - (bounds[0]+bounds[2])*0.5f) / (bounds[0] - bounds[2]));
    gy = ml::abs((my - (bounds[1]+bounds[3])*0.5f) / (bounds[1] - bounds[3]));
    a = maxf(gx, gy) - 0.5f;
    a = clampf(a, 0, 1);
    nvgGlobalAlpha(vg, a);

    nvgBeginPath(vg);
    nvgFillColor(vg, nvgRGBA(220,220,220,255));
    nvgRoundedRect(vg, bounds[0]-2,bounds[1]-2, (int)(bounds[2]-bounds[0])+4, (int)(bounds[3]-bounds[1])+4, 3);
    px = (int)((bounds[2]+bounds[0])/2);
    nvgMoveTo(vg, px,bounds[1] - 10);
    nvgLineTo(vg, px+7,bounds[1]+1);
    nvgLineTo(vg, px-7,bounds[1]+1);
    nvgFill(vg);

    nvgFillColor(vg, nvgRGBA(0,0,0,220));
    nvgTextBox(vg, x,y, 150, "Hover your mouse over the text to see calculated caret position.", NULL);

    nvgRestore(vg);
}

void drawWidths(NVGcontext* vg, float x, float y, float width)
{
    int i;

    nvgSave(vg);

    nvgStrokeColor(vg, nvgRGBA(0,0,0,255));

    for (i = 0; i < 20; i++) {
        float w = (i+0.5f)*0.1f;
        nvgStrokeWidth(vg, w);
        nvgBeginPath(vg);
        nvgMoveTo(vg, x,y);
        nvgLineTo(vg, x+width,y+width*0.3f);
        nvgStroke(vg);
        y += 10;
    }

    nvgRestore(vg);
}

void drawCaps(NVGcontext* vg, float x, float y, float width)
{
    int i;
    int caps[3] = {NVG_BUTT, NVG_ROUND, NVG_SQUARE};
    float lineWidth = 8.0f;

    nvgSave(vg);

    nvgBeginPath(vg);
    nvgRect(vg, x-lineWidth/2, y, width+lineWidth, 40);
    nvgFillColor(vg, nvgRGBA(255,255,255,32));
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRect(vg, x, y, width, 40);
    nvgFillColor(vg, nvgRGBA(255,255,255,32));
    nvgFill(vg);

    nvgStrokeWidth(vg, lineWidth);
    for (i = 0; i < 3; i++) {
        nvgLineCap(vg, caps[i]);
        nvgStrokeColor(vg, nvgRGBA(0,0,0,255));
        nvgBeginPath(vg);
        nvgMoveTo(vg, x, y + i*10 + 5);
        nvgLineTo(vg, x+width, y + i*10 + 5);
        nvgStroke(vg);
    }

    nvgRestore(vg);
}

void drawScissor(NVGcontext* vg, float x, float y, float t)
{
    nvgSave(vg);

    // Draw first rect and set scissor to it's area.
    nvgTranslate(vg, x, y);
    nvgRotate(vg, nvgDegToRad(5));
    nvgBeginPath(vg);
    nvgRect(vg, -20,-20,60,40);
    nvgFillColor(vg, nvgRGBA(255,0,0,255));
    nvgFill(vg);
    nvgScissor(vg, -20,-20,60,40);

    // Draw second rectangle with offset and rotation.
    nvgTranslate(vg, 40,0);
    nvgRotate(vg, t);

    // Draw the intended second rectangle without any scissoring.
    nvgSave(vg);
    nvgResetScissor(vg);
    nvgBeginPath(vg);
    nvgRect(vg, -20,-10,60,30);
    nvgFillColor(vg, nvgRGBA(255,128,0,64));
    nvgFill(vg);
    nvgRestore(vg);

    // Draw second rectangle with combined scissoring.
    nvgIntersectScissor(vg, -20,-10,60,30);
    nvgBeginPath(vg);
    nvgRect(vg, -20,-10,60,30);
    nvgFillColor(vg, nvgRGBA(255,128,0,255));
    nvgFill(vg);

    nvgRestore(vg);
}

void renderDemo(NVGcontext* vg, float mx, float my, float width, float height,
    float t, int blowup, DemoData* data)
{
    float x,y,popy;

    drawEyes(vg, width - 250, 50, 150, 100, mx, my, t);
    drawParagraph(vg, width - 450, 50, 150, 100, mx, my);
    drawGraph(vg, 0, height/2, width, height/2, t);
    drawColorwheel(vg, width - 300, height - 300, 250.0f, 250.0f, t);

    // Line joints
    drawLines(vg, 120, height-50, 600, 50, t);

    // Line caps
    drawWidths(vg, 10, 50, 30);

    // Line caps
    drawCaps(vg, 10, 300, 30);

    drawScissor(vg, 50, height-80, t);

    nvgSave(vg);
    if (blowup) {
        nvgRotate(vg, ml::sin(t*0.3f)*5.0f/180.0f*NVG_PI);
        nvgScale(vg, 2.0f, 2.0f);
    }

    // Widgets
    drawWindow(vg, "Widgets `n Stuff", 50, 50, 300, 400);
    x = 60; y = 95;
    drawSearchBox(vg, "Search", x,y,280,25);
    y += 40;
    drawDropDown(vg, "Effects", x,y,280,28);
    popy = y + 14;
    y += 45;

    // Form
    drawLabel(vg, "Login", x,y, 280,20);
    y += 25;
    drawEditBox(vg, "Email",  x,y, 280,28);
    y += 35;
    drawEditBox(vg, "Password", x,y, 280,28);
    y += 38;
    drawCheckBox(vg, "Remember me", x,y, 140,28);
    drawButton(vg, ICON_LOGIN, "Sign in", x+138, y, 140, 28, nvgRGBA(0,96,128,255));
    y += 45;

    // Slider
    drawLabel(vg, "Diameter", x,y, 280,20);
    y += 25;
    drawEditBoxNum(vg, "123.00", "px", x+180,y, 100,28);
    drawSlider(vg, 0.4f, x,y, 170,28);
    y += 55;

    drawButton(vg, ICON_TRASH, "Delete", x, y, 160, 28, nvgRGBA(128,16,8,255));
    drawButton(vg, 0, "Cancel", x+170, y, 110, 28, nvgRGBA(0,0,0,0));

    // Thumbnails box
    drawThumbnails(vg, 365, popy-30, 160, 300, data->images, 12, t);

    nvgRestore(vg);
}

static int mini(int a, int b) { return a < b ? a : b; }

static void unpremultiplyAlpha(unsigned char* image, int w, int h, int stride)
{
    int x,y;

    // Unpremultiply
    for (y = 0; y < h; y++) {
        unsigned char *row = &image[y*stride];
        for (x = 0; x < w; x++) {
            int r = row[0], g = row[1], b = row[2], a = row[3];
            if (a != 0) {
                row[0] = (int)mini(r*255/a, 255);
                row[1] = (int)mini(g*255/a, 255);
                row[2] = (int)mini(b*255/a, 255);
            }
            row += 4;
        }
    }

    // Defringe
    for (y = 0; y < h; y++) {
        unsigned char *row = &image[y*stride];
        for (x = 0; x < w; x++) {
            int r = 0, g = 0, b = 0, a = row[3], n = 0;
            if (a == 0) {
                if (x-1 > 0 && row[-1] != 0) {
                    r += row[-4];
                    g += row[-3];
                    b += row[-2];
                    n++;
                }
                if (x+1 < w && row[7] != 0) {
                    r += row[4];
                    g += row[5];
                    b += row[6];
                    n++;
                }
                if (y-1 > 0 && row[-stride+3] != 0) {
                    r += row[-stride];
                    g += row[-stride+1];
                    b += row[-stride+2];
                    n++;
                }
                if (y+1 < h && row[stride+3] != 0) {
                    r += row[stride];
                    g += row[stride+1];
                    b += row[stride+2];
                    n++;
                }
                if (n > 0) {
                    row[0] = r/n;
                    row[1] = g/n;
                    row[2] = b/n;
                }
            }
            row += 4;
        }
    }
}

static void setAlpha(unsigned char* image, int w, int h, int stride, unsigned char a)
{
    int x, y;
    for (y = 0; y < h; y++) {
        unsigned char* row = &image[y*stride];
        for (x = 0; x < w; x++)
            row[x*4+3] = a;
    }
}

static void flipHorizontal(unsigned char* image, int w, int h, int stride)
{
    int i = 0, j = h-1, k;
    while (i < j) {
        unsigned char* ri = &image[i * stride];		
        unsigned char* rj = &image[j * stride];		
        for (k = 0; k < w*4; k++) {
            unsigned char t = ri[k];
            ri[k] = rj[k];
            rj[k] = t;
        }
        i++;
        j--;
    }
}

void saveScreenShot(int w, int h, int premult, const char* name)
{
    unsigned char* image = (unsigned char*)malloc(w*h*4);
    if (image == NULL)
        return;
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image);
    if (premult)
        unpremultiplyAlpha(image, w, h, w*4);
    else
        setAlpha(image, w, h, w*4, 255);
    flipHorizontal(image, w, h, w*4);
    stbi_write_png(name, w, h, 4, image, w*4);
    free(image);
}
uint64_t timerAbsoluteTime();

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

    NVGcolor color0;
    NVGcolor color1;

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
    tw = nvgTextBounds(vg, 0, 0, text, NULL, NULL);
    if (preicon != 0) {
        nvgFontSize(vg, h*1.3f);
        nvgFontFace(vg, "icons");
        iw = nvgTextBounds(vg, 0, 0, cpToUTF8(preicon,icon), NULL, NULL);
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

ui::AreaDesc loginButton  = {ICON_LOGIN, "Sign in", BUTTON_ACTION};
ui::AreaDesc deleteButton = {ICON_TRASH, "Delete",  BUTTON_ALERT};
ui::AreaDesc cancelButton = {         0, "Cancel",  0};

enum CONTROL_ID
{
    ID_LOGIN_BUTTON,
    ID_DELETE_BUTTON,
    ID_CANCEL_BUTTON,
    ID_CHECKBOX,
    ID_WINDOW_HEADER,
    ID_TRACKBAR,
    ID_COMBOBOX,
    CONTROL_COUNT
};

rect_t controlArea[CONTROL_COUNT] = 
{
    {0.0f, 0.0f, 140.0f, 28.0f},
    {0.0f, 0.0f, 160.0f, 28.0f},
    {0.0f, 0.0f, 110.0f, 28.0f},
    {0.0f, 0.0f, 125.0f, 28.0f},
    {0.0f, 0.0f, 300.0f - 2.0f, 30.0f},
    {0.0f, 0.0f,  12.0f, 12.0f},
    {0.0f, 0.0f, 280.0f, 28.0f}
};

namespace app
{
    DemoData data;

    bool showEffects;
    bool checkboxValue;
    float wx1, wy1, ww, wh;
    float wx2, wy2;

    float dt = 0;

    int blowup = 0;
    int screenshot = 0;
    int premult = 0;

    PerfGraph fps, cpuGraph, gpuGraph;

    WindowState ws;

    int dsx, dsy;
    int sliderPos;

    ui::refined_events_t events;

    cpu_timer_t        cpuTimer;
    gfx::gpu_timer_t   gpuTimer;

    void init()
    {
        memory_t fileData;

        if (loadDemoData(vg::ctx, &data) == -1)
            return;

        initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");
        initGraph(&cpuGraph, GRAPH_RENDER_MS, "CPU Time");
        initGraph(&gpuGraph, GRAPH_RENDER_MS, "GPU Time");

        showEffects   = false;
        checkboxValue = false;

        wx1 = 51.0f;
        wy1 = 51.0f;
        ww  = 300.0f - 2.0f;
        wh = 30.0f;

        wx2 = 550;
        wy2 = 30;

        sliderPos = 0;

        ws.visible = 1;
        ws.minimized = 0;

        events.activeArea = ui::INVALID_ID;
        events.eventMask  = 0;

        gfx::gpu_timer_init(&gpuTimer);
    }

    void fini()
    {
        gfx::gpu_timer_fini(&gpuTimer);

        freeDemoData(vg::ctx, &data);
    }

    void drawButton(uint32_t id, struct NVGcontext* vg, ui::AreaDesc* area, const rect_t& rect)
    {
        area->state &= ~3;
        if (ui::isAreaActive(&events, id))
            area->state |= BUTTON_PRESSED;
        if (ui::isAreaHighlighted(&events, id))
            area->state |= BUTTON_FOCUSED;
        ::drawButton(vg, area->preicon, area->text, rect.x, rect.y, rect.w, rect.h, area->state);
    }

    void render()
    {
        double mx, my;
        int width, height, xx, yy;

        glPushAttrib(GL_ALL_ATTRIB_BITS);

        ui::mouseAbsOffset(&xx, &yy);

        mx = xx; my = yy;

        //glfwGetFramebufferSize(window, &width, &height);
        width  = gfx::width;
        height = gfx::height;

        // Update and render
        glViewport(0, 0, width, height);
        if (premult)
            glClearColor(0,0,0,0);
        else
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
        //float x,y,popy;

        //drawEyes(vg::ctx, width - 250, 50, 150, 100, mx, my, t);
        //drawGraph(vg::ctx, 0, height/2, width, height/2, t);
        //drawColorwheel(vg::ctx, width - 300, height - 300, 250.0f, 250.0f, t);

        //// Widgets
        //drawWindow(vg::ctx, "Widgets `n Stuff", wx1, wy1, 300, 400);
        //x = wx1 + 10; y = wy1 + 45;
        //drawSearchBox(vg::ctx, "Search", x,y,280,25);
        //y += 40;
        //drawDropDown(vg::ctx, "Effects", x,y,280,28);
        //popy = y + 14;
        //y += 45;

        //// Form
        //drawLabel(vg::ctx, "Login", x,y, 280,20);
        //y += 25;
        //drawEditBox(vg::ctx, "Email",  x,y, 280,28);
        //y += 35;
        //drawEditBox(vg::ctx, "Password", x,y, 280,28);
        //y += 38;
        //drawCheckBox(vg::ctx, "Remember me", x,y, 140,28, checkboxValue);
        //drawButton(0, vg::ctx, &loginButton, controlArea[ID_LOGIN_BUTTON]);
        //y += 45;

        //// Slider
        //drawLabel(vg::ctx, "Diameter", x,y, 280,20);
        //y += 25;
        //drawEditBoxNum(vg::ctx, "123.00", "px", x+180,y, 100,28);
        //drawSlider(vg::ctx, sliderPos, x,y, 170,28);
        //y += 55;

        //drawButton(1, vg::ctx, &deleteButton, controlArea[ID_DELETE_BUTTON]);
        //drawButton(2, vg::ctx, &cancelButton, controlArea[ID_CANCEL_BUTTON]);

        //if (showEffects)
        //{
        //    // Thumbnails box
        //    drawThumbnails(vg::ctx, wx1 + 315, popy-30, 160, 300, images, 12, t);
        //}

        //drawWindow2(&ws, vg::ctx, "Widgets `n Stuff", wx2, wy2, 300, 660);

        //nvgRestore(vg::ctx);

        nvgBeginFrame(vg::ctx, width, height, 1.0f);

        cpu_timer_start(&cpuTimer);
        gpu_timer_start(&gpuTimer);

        renderDemo(vg::ctx, mx, my, width, height, t, blowup, &data);

        renderGraph(vg::ctx, 5,5, &fps);
        renderGraph(vg::ctx, 5+200+5,5, &cpuGraph);
        renderGraph(vg::ctx, 5+200+5+200+5,5, &gpuGraph);

        nvgEndFrame(vg::ctx);

        gpu_timer_stop(&gpuTimer);
        cpu_timer_stop(&cpuTimer);

        updateGraph(&fps, dt);
        updateGraph(&cpuGraph, cpu_timer_measured(&cpuTimer) / 1000000.0f);
        updateGraph(&gpuGraph, gfx::gpu_timer_measured(&gpuTimer) / 1000000.0f);

        if (screenshot) {
            screenshot = 0;
            saveScreenShot(width, height, premult, "dump.png");
        }

        glEnable(GL_DEPTH_TEST);
        glPopAttrib();
    }

    void update(float dt)
    {
        app::dt = dt;

        if (ui::keyWasPressed(SDL_SCANCODE_SPACE))
            blowup = !blowup;
        if (ui::keyWasPressed(SDL_SCANCODE_S))
            screenshot = 1;
        if (ui::keyWasPressed(SDL_SCANCODE_P))
            premult = !premult;

        //int mx, my;

        //ui::mouseAbsOffset(&mx, &my);

        //point_t pt = {mx, my};

        //uint32_t activeArea = ui::INVALID_ID;

        //for (size_t i = 0; i < CONTROL_COUNT; ++i)
        //{
        //    if (testPtInRect(pt, controlArea[i]))
        //    {
        //        activeArea = i;
        //    }
        //}

        //for (size_t i = 0; i < activeAreaCount; ++i)
        //{
        //    uint16_t id = ::activeArea[i];
        //    if (testPtInRect(pt.x, pt.y, pos[id].x, pos[id].y, dim[id].x, dim[id].y))
        //    {
        //        activeArea = id+CONTROL_COUNT;
        //    }
        //}

        //ui::refineEvents(&events, activeArea);

        //switch (ui::getEventArea(&events, ui::EVENT_LEFT_CLICK))
        //{
        //case ID_LOGIN_BUTTON:
        //    break;
        //case ID_DELETE_BUTTON:
        //    break;
        //case ID_CANCEL_BUTTON:
        //    break;
        //case ID_CHECKBOX:
        //    checkboxValue = !checkboxValue;
        //    break;
        //case ID_COMBOBOX:
        //    showEffects = true;
        //    break;
        //case CONTROL_COUNT+PGID_CLOSEBTN:
        //    ws.visible = 0;
        //    break;
        //case CONTROL_COUNT+PGID_MINMAXBTN:
        //    ws.minimized = !ws.minimized;
        //    break;
        //}

        ////Event activated outside
        //if (showEffects && events.eventMask&ui::EVENT_ACTIVATED && !testPtInRect(mx-wx1, my-wy1, 315, 69, 160, 300))
        //{
        //    showEffects = false;
        //}

        //switch (ui::getEventArea(&events, ui::EVENT_ACTIVATED))
        //{
        //case CONTROL_COUNT+PGID_CAPTION:
        //    dsx = wx2 - mx;
        //    dsy = wy2 - my;
        //    break;
        //case ID_WINDOW_HEADER:
        //    dsx = wx1 - mx;
        //    dsy = wy1 - my;
        //    break;
        //case ID_TRACKBAR:
        //    dsx = sliderPos - mx;
        //    break;
        //}

        //switch (ui::getEventArea(&events, ui::EVENT_ACTIVE))
        //{
        //case CONTROL_COUNT+PGID_CAPTION:
        //    wx2 = float(mx + dsx);
        //    wy2 = float(my + dsy);
        //    break;
        //case ID_WINDOW_HEADER:
        //    wx1 = float(mx + dsx);
        //    wy1 = float(my + dsy);
        //    break;
        //case ID_TRACKBAR:
        //    sliderPos = float(mx + dsx);
        //    sliderPos = core::max(sliderPos,   0);
        //    sliderPos = core::min(sliderPos, 170);
        //    break;
        //}

        //float x,y,popy;
        //x = wx1 + 10; y = wy1 + 45;
        //y += 40;
        //controlArea[ID_COMBOBOX].x = x;
        //controlArea[ID_COMBOBOX].y = y;
        //popy = y + 14;
        //y += 45;
        //y += 25;
        //y += 35;
        //y += 38;

        //controlArea[ID_CHECKBOX].x = x;
        //controlArea[ID_CHECKBOX].y = y;

        //controlArea[ID_LOGIN_BUTTON].x = x + 138;
        //controlArea[ID_LOGIN_BUTTON].y = y;

        ////drawButton(vg, ICON_LOGIN, "Sign in", x+138, y,    140, 28, BUTTON_ACTION);

        //y += 45;
        //y += 25;

        //controlArea[ID_TRACKBAR].x = x+sliderPos-6;
        //controlArea[ID_TRACKBAR].y = y+14-6;

        //y += 55;

        //controlArea[ID_DELETE_BUTTON].x = x;
        //controlArea[ID_DELETE_BUTTON].y = y;

        //controlArea[ID_CANCEL_BUTTON].x = x + 170;
        //controlArea[ID_CANCEL_BUTTON].y = y;

        ////drawButton(vg, ICON_TRASH, "Delete", x, y, 160, 28, BUTTON_ALERT);
        ////drawButton(vg, 0, "Cancel", x+170, y, 110, 28, 0);

        //controlArea[ID_WINDOW_HEADER].x = wx1;
        //controlArea[ID_WINDOW_HEADER].y = wy1;
        //controlArea[ID_WINDOW_HEADER].w = ww;
        //controlArea[ID_WINDOW_HEADER].h = wh;
    }

    void recompilePrograms() {}

    void resize(int width, int height) {}
}
