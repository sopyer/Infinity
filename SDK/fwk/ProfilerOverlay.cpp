#include <gfx/gfx.h>
#include <fwk/ui.h>

#include "ProfilerOverlay.h"

static const uint32_t profilerIntervalMask = 0xFEDC0000;

GLuint prgQuad;

#define INVALID_SELECTION 0xFFFFFFFF

static const int    MAX_STACK_DEPTH  = 8;
static const int    MAX_THREAD_COUNT = 8;
static const int    firstThreadRow = 3;
static const float  overlayPadding = 25.0f;
static const float  viewMargin = 5.0f;
static const float  tickSize = 3;

void ProfilerOverlay::init()
{
    sx = sy =  1.0f;
    dx = 0.0f;
    mDoDrag = false;
    ui::mouseAbsOffset(&mbx, &mby);
}

void ProfilerOverlay::resize(int w, int h)
{
    width = w; height = h;
    layoutUI(w, h);
}

void ProfilerOverlay::layoutUI(int w, int h)
{
    rect_t mainArea = {overlayPadding, overlayPadding, w - 2.0f * overlayPadding, h - 2.0f * overlayPadding};
    graphArea = {mainArea.x+viewMargin, mainArea.y+viewMargin, mainArea.w - 300 - 4*viewMargin, mainArea.h - 2*viewMargin};
    helpArea  = {graphArea.x + graphArea.w + 2*viewMargin, mainArea.y+viewMargin, 300, 100};
    statArea  = {graphArea.x + graphArea.w + 2*viewMargin, helpArea.y + helpArea.h + 2*viewMargin, 300, mainArea.h - helpArea.h - 4*viewMargin};
}

void ProfilerOverlay::fini()
{
    glDeleteProgram(prgQuad);
}

void ProfilerOverlay::addInterval(
    const char* name, uint32_t colorID, 
    uint16_t trackID, uint32_t start, 
    uint32_t duration, int depth
)
{
    float xstart   = (float)start;
    float ystart   = depth + 0.1f + trackID * (MAX_STACK_DEPTH + 2) + firstThreadRow;

    uint32_t color = ui::rainbowTableL[colorID];
    colors.push_back(color);

    rectData.emplace_back(rect_t{xstart, ystart, (float)duration, 0.9f});

    Interval inter = 
    {
        name,
        xstart / 1000.0f,
        duration / 1000.0f
    };

    intervals.push_back(inter);
}

inline uint32_t convertToMs(uint64_t ms, uint64_t freq, int quantShift)
{
    return (uint32_t)(ms * (1ull<<quantShift) * 1000000 / freq);
}

void ProfilerOverlay::loadProfilerData()
{
    event_capture_t* capture = profilerGetData();
    const char**     names   = profilerGetNames();

    size_t                  numEvents = capture->numEvents;
    const profiler_event_t* events = capture->events;

    rectData.resize(numEvents * 4);

    int     stackTop[MAX_THREAD_COUNT] = {-1, -1, -1, -1, -1, -1, -1, -1};
    struct
    {
        size_t   id;
        uint32_t sliceBegin;
    } stack[MAX_THREAD_COUNT][MAX_STACK_DEPTH];

    core::index_t<uint16_t, ui::RAINBOW_TABLE_L_SIZE> colorMap  = {0};
    core::index_t<uint16_t, 32>                       threadMap = {0};

    rectData.clear();
    intervals.clear();
    colors.clear();

    numThreads = 0;

    minTime = convertToMs(events[0].timestamp, capture->freq, capture->quantShift);
    maxTime = convertToMs(events[0].timestamp, capture->freq, capture->quantShift);

    for (size_t i=0; i<numEvents; ++i)
    {
        profiler_event_t event = events[i];
        uint32_t         threadIdx;
        uint32_t         ts = convertToMs(events[i].timestamp, capture->freq, capture->quantShift);
        threadIdx = core::index_lookup_or_add(&threadMap, event.tid);

        minTime = core::min(minTime, ts);
        maxTime = core::max(maxTime, ts);

        numThreads = core::max(numThreads, threadIdx);

        int& top = stackTop[threadIdx];

        //ignore intervals exceeding stack depth
        if (top >= MAX_STACK_DEPTH-1) continue;

        if (event.phase==PROF_EVENT_PHASE_BEGIN)
        {
            //Add event to stack
            ++top;
            stack[threadIdx][top].id         = event.id;
            stack[threadIdx][top].sliceBegin = ts;
        }
        else if (event.phase==PROF_EVENT_PHASE_END)
        {
            //ignore not opened interval
            if (top < 0 || stack[threadIdx][top].id != event.id)
                continue;

            //remove item from stack and add entry
            uint32_t colorID = core::index_lookup_or_add(&colorMap, event.id);
            uint32_t start   = stack[threadIdx][top].sliceBegin;
            addInterval(names[event.id], colorID, threadIdx, start, ts - start, top);

            --top;
        }
    }

    //TODO: close all opened intervals
    uint32_t endTime = capture->endTime;
    for (size_t i = 0; i < MAX_THREAD_COUNT; ++i)
    {
        int top = stackTop[i];
        for (;top>=0; --top)
        {
            uint16_t id      = stack[i][top].id;
            uint32_t colorID = core::index_lookup_or_add(&colorMap, id);
            uint32_t start   = stack[i][top].sliceBegin;
            addInterval(names[id], colorID, i, start, endTime - start, top);
        }
    }

    ++numThreads;

    prevMouseMoveTime = 0;
    showTooltip = false;

    sx = graphArea.w/(maxTime - minTime);
    sy = 20.0f;
    dx = -sx*maxTime;
}

uint32_t roundInterval(uint32_t interval)
{
    uint32_t roundedInterval = 100;
    for (; roundedInterval < 100000000; roundedInterval *= 10)
    {
        if (roundedInterval>interval)
            break;
    }

    if (interval < roundedInterval/2)
    {
        if (interval > roundedInterval/5)
            roundedInterval /= 2;
        else if (interval > roundedInterval/10)
            roundedInterval /= 5;
    }

    return roundedInterval;
}

void selectUnits(uint32_t interval, uint32_t* unitScale, const char** unitFormat)
{
    assert(unitScale);
    assert(unitFormat);

    *unitScale  = interval;
    *unitFormat = "%dus";
    if (interval>=1000)
    {
        *unitFormat = "%dms";
        *unitScale  = interval/1000;
    }
    else if (interval>=1000000)
    {
        *unitFormat = "%ds";
        *unitScale  = interval / 1000000;
    }
}

size_t ProfilerOverlay::elementUnderCursor(int x, int y)
{
    point_t pt = {(x-graphArea.x-dx)/sx, (y-graphArea.y)/sy};

    for (size_t i = 0; i < rectData.size(); ++i)
    {
        if (testPtInRect(pt, rectData[i]))
        {
            return i;
        }
    }

    return INVALID_SELECTION;
}

void ProfilerOverlay::updateUI(float delta)
{
    float keybScaleSpeed  = 0.50f; // (increase/s)
    float mouseScaleSpeed = 0.10f; // (increase)

    int x, y;
    ui::mouseAbsOffset(&x, &y);

    static const int MOUSE_MOVE_OFFSET_THRESHOLD = 5;
    static const int MOUSE_IDLE_TIME_THRESHOLD = 700*1000; //0.7s

    if (abs(mbx-x)+abs(mby-y)>MOUSE_MOVE_OFFSET_THRESHOLD || ui::mouseIsPressed(SDL_BUTTON_LEFT) || ui::mouseIsPressed(SDL_BUTTON_RIGHT))
    {
        mbx = x;
        mby = y;
        prevMouseMoveTime = timerAbsoluteTime();
        showTooltip = false;
    }

    if (timerAbsoluteTime()-prevMouseMoveTime>MOUSE_IDLE_TIME_THRESHOLD && !showTooltip)
    {
        showTooltip = true;
    }

    if (ui::mouseWasReleased(SDL_BUTTON_LEFT))
    {
        uint32_t interval = elementUnderCursor(x, y);
        selection.clear();
        if (interval != INVALID_SELECTION)
            selection.push_back(interval);
    }

    float scale = 1.0f;

    if (ui::keyIsPressed(SDL_SCANCODE_EQUALS) ||
       (ui::keyIsPressed(SDL_SCANCODE_LCTRL) && ui::mouseWasWheelUp()))
    {
        scale = 1.0f + (ui::mouseWasWheelUp() ? mouseScaleSpeed : keybScaleSpeed * delta);
    }
    if (ui::keyIsPressed(SDL_SCANCODE_MINUS) ||
       (ui::keyIsPressed(SDL_SCANCODE_LCTRL) && ui::mouseWasWheelDown()))
    {
        scale  = 1.0f / (1.0f + (ui::mouseWasWheelDown() ? mouseScaleSpeed : keybScaleSpeed * delta));
    }

    float scaleMin = graphArea.w/(maxTime - minTime);
    float scaleMax = 1.0f;
    scale = ml::clamp(scale, scaleMin/sx, scaleMax/sx);

    sx *= scale;
    dx = (1.0f-scale) * (x-this->graphArea.x) + scale * dx;

    if (ui::mouseWasPressed(SDL_BUTTON_RIGHT))
    {
        mDoDrag = true;
    }
    if (mDoDrag)
    {
        int offx;
        ui::mouseRelOffset(&offx, NULL);
        dx += offx;
    }
    if (ui::mouseWasReleased(SDL_BUTTON_RIGHT) && mDoDrag)
    {
        mDoDrag = false;
    }

    float    minMarkerDist = 80.0f;
    uint32_t interval      = roundInterval(uint32_t(minMarkerDist/sx));

    startInterval = minTime / interval;
    endInterval   = (maxTime + interval - 1) / interval;

    pxIntervalStep = interval * sx;
    dx = ml::clamp(dx, graphArea.w-(endInterval*pxIntervalStep), -(startInterval*pxIntervalStep));

    startInterval = (int)ml::floor(-dx/sx) / interval;
    endInterval   = ((int)ml::ceil((graphArea.w-dx)/sx) + interval - 1) / interval;

    selectUnits(interval, &unitScale, &unitFormat);
}

void ProfilerOverlay::renderFullscreen()
{
    PROFILER_CPU_TIMESLICE("ProfilerOverlay::renderFullscreen");
    glViewport(0, 0, width, height);

    char  strBuf[128];
    float tx = graphArea.x+dx;

    nvgBeginFrame(vg::ctx, gfx::width, gfx::height, (float)gfx::width/gfx::height);
    // Background
    nvgFillColor(vg::ctx, nvgRGBA(0x1A,0x1A,0x1A,0xD0));
    nvguRect(vg::ctx, 0.0f, 0.0f, (float)width, (float)height);

    if (rectData.empty())
    {
        nvgEndFrame(vg::ctx);
        return;
    }

    {
        PROFILER_CPU_TIMESLICE("NanoVG");

        float basey = graphArea.y+sy;
        //Bars rendering
        nvgScissor(vg::ctx, graphArea.x, graphArea.y, graphArea.w, graphArea.h);
        for (uint32_t i = startInterval; i < endInterval; ++i)
        {
            nvgFillColor(vg::ctx, (i%2) ? nvgRGB(32, 32, 32) : nvgRGB(8, 8, 8));
            nvguRect(vg::ctx, i*pxIntervalStep + tx, basey, pxIntervalStep, graphArea.h-sy);
        }
        nvgResetScissor(vg::ctx);

        nvgFontSize(vg::ctx, 14.0f);
        nvgFontFace(vg::ctx, "default");
        nvgTextAlign(vg::ctx, NVG_ALIGN_CENTER|NVG_ALIGN_BOTTOM);
        nvgFillColor(vg::ctx, nvgRGB(255,255,255));
        nvgStrokeColor(vg::ctx, nvgRGB(192,192,192));
        nvgStrokeWidth(vg::ctx, 2.0f);
        //Labels and ticks rendering
        for (uint32_t i = startInterval; i <= endInterval; ++i)
        {
            float x = i*pxIntervalStep + tx;
            if (x<graphArea.x || x>graphArea.x+graphArea.w)
                continue;
            nvguLine(vg::ctx, x, basey, x, basey+tickSize);
            sprintf_s(strBuf, unitFormat, i*unitScale);
            nvgText(vg::ctx, x, basey-tickSize, strBuf, 0);
        }
        nvguLine(vg::ctx, graphArea.x, basey, graphArea.x+graphArea.w, basey);

        nvgFontSize(vg::ctx, 14.0f);
        nvgFontFace(vg::ctx, "default");
        nvgTextAlign(vg::ctx, NVG_ALIGN_LEFT|NVG_ALIGN_BOTTOM);
        nvgFillColor(vg::ctx, nvgRGB(128,128,128));
        nvgStrokeColor(vg::ctx, nvgRGB(128,128,128));
        nvgStrokeWidth(vg::ctx, 3.0f);

        float dy = (MAX_STACK_DEPTH+2)*sy;
        float y = graphArea.y + firstThreadRow*sy-2;
        for (size_t i = 0; i<numThreads; ++i, y+=dy)
        {
            sprintf_s(strBuf, "Thread%d", i);
            nvgText(vg::ctx, graphArea.x+10.0f, y, strBuf, 0);
            nvguLine(vg::ctx, graphArea.x+5.0f, y, graphArea.x+graphArea.w-10.0f, y);
        }
    }

    {
        PROFILER_CPU_TIMESLICE("Render Bars");

        nvgFontSize(vg::ctx, 14.0f);
        nvgFontFace(vg::ctx, "default");
        nvgTextAlign(vg::ctx, NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);

        size_t numRects = colors.size();
        for (size_t i=0; i<numRects; ++i)
        {
            float x0 = rectData[i].x;
            float y0 = rectData[i].y;
            float x1 = x0 + rectData[i].w;
            float y1 = y0 + rectData[i].h;

            x0 = x0*sx+tx;
            y0 = y0*sy+graphArea.y;
            x1 = x1*sx+tx;
            y1 = y1*sy+graphArea.y;

            if (x0>graphArea.x+graphArea.w || x1<graphArea.x || x1-x0<1.0f)
                continue;

            float x0c = ml::clamp(x0, graphArea.x, graphArea.x+graphArea.w);
            float x1c = ml::clamp(x1, graphArea.x, graphArea.x+graphArea.w);
            float y0c = ml::clamp(y0, graphArea.y, graphArea.y+graphArea.h);
            float y1c = ml::clamp(y1, graphArea.y, graphArea.y+graphArea.h);

            uint32_t c = colors[i];
            v128 cf = vi_cvt_u8x4_to_v128(c);
            nvgFillColor(vg::ctx, nvgRGBAf(cf.m128_f32[0], cf.m128_f32[1], cf.m128_f32[2], cf.m128_f32[3]));
            nvguRect(vg::ctx, x0c, y0c, x1c-x0c, y1c-y0c);
            nvgScissor(vg::ctx, x0c, y0c, x1c-x0c, y1c-y0c);
            nvgFillColor(vg::ctx, nvgRGB(255, 255, 255));
            nvgText(vg::ctx, x0+5, 0.5f*(y0+y1), intervals[i].name, 0);
            nvgResetScissor(vg::ctx);
        }
    }

    cstr1024 buffer = "Ctrl+Wheel - zoom timeline\n"
                      "RMB - horizontal pan\n"
                      "Point at bar to see info tooltip\n"
                      "Click to see more info";

    //Help rendring
    nvgFillColor(vg::ctx, nvgRGB(16, 16, 16));
    nvguRect(vg::ctx, helpArea.x, helpArea.y, helpArea.w, helpArea.h);
    nvguRect(vg::ctx, statArea.x, statArea.y, statArea.w, statArea.h);

    nvgFontSize(vg::ctx, 14.0f);
    nvgFillColor(vg::ctx, nvgRGB(255, 255, 255));
    nvgFontFace(vg::ctx, "default");
    nvgTextAlign(vg::ctx, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgTextBox(vg::ctx, helpArea.x+20, helpArea.y+20, FLT_MAX, buffer, buffer+cstr_len(buffer));

    //Tooltip rendering
    if (showTooltip)
    {
        size_t selected  = elementUnderCursor(mbx, mby);

        if (selected != INVALID_SELECTION)
        {
            Interval& interval = intervals[selected];

            sprintf_s(
                buffer,
                "name     : %s\n"
                "duration : %.4f\n"
                "start    : %.4f\n"
                "end      : %.4f",
                interval.name,
                interval.duration,
                interval.start,
                interval.start+interval.duration
            );

            static const float mg = 10.0f;
            static const float lineH = 15.0f;

            float bx = mbx+mg+8;
            float by = mby+32;

            float bounds[4];

            nvgFontSize(vg::ctx, 14.0f);
            nvgFontFace(vg::ctx, "default");
            nvgTextAlign(vg::ctx, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);

            nvgTextBoxBounds(vg::ctx, bx, by, FLT_MAX, buffer, buffer+cstr_len(buffer), bounds);

            nvgFillColor(vg::ctx, nvgRGBA(0, 0, 67, 128));
            nvgStrokeColor(vg::ctx, nvgRGB(0, 89, 225));
            nvgStrokeWidth(vg::ctx, 1.0f);
            nvgBeginPath(vg::ctx);
            nvgRoundedRect(vg::ctx, bounds[0]-mg, bounds[1]-mg, bounds[2]-bounds[0]+2*mg, bounds[3]-bounds[1]+2*mg, 3);
            nvgFill(vg::ctx);
            nvgStroke(vg::ctx);

            nvgFillColor(vg::ctx, nvgRGB(255, 255, 255));

            nvgTextBox(vg::ctx, bx, by, FLT_MAX, buffer, buffer+cstr_len(buffer));
        }
    }

    //Draw selection
    nvgFontSize(vg::ctx, 14.0f);
    nvgFontFace(vg::ctx, "default");
    nvgTextAlign(vg::ctx, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgFillColor(vg::ctx, nvgRGB(255, 255, 255));

    float asc, desc, lineh;
    nvgTextMetrics(vg::ctx, &asc, &desc, &lineh);
    
    nvgScissor(vg::ctx, statArea.x, statArea.y, statArea.w, statArea.h);
    float y = statArea.y + 20;
    for (size_t i = 0; i < selection.size(); ++i)
    {
        uint32_t  idx = selection[i];
        Interval& interval = intervals[idx];

        sprintf_s(
            buffer,
            "name     : %s\n"
            "duration : %.4f\n"
            "start    : %.4f\n"
            "end      : %.4f",
            interval.name,
            interval.duration,
            interval.start,
            interval.start+interval.duration
        );

        nvgTextBox(vg::ctx, statArea.x+20, y, FLT_MAX, buffer, buffer+cstr_len(buffer));
    }
    nvgResetScissor(vg::ctx);

    nvgEndFrame(vg::ctx);
}
