#include <gfx/gfx.h>
#include <fwk/ui.h>

#include "ProfilerOverlay.h"

static const uint32_t profilerIntervalMask = 0xFEDC0000;

GLuint prgQuad;

#define INVALID_SELECTION 0xFFFFFFFF

static const size_t MAX_STACK_DEPTH  = 8;
static const size_t MAX_THREAD_COUNT = 8;
static const size_t firstThreadRow = 3;
static const float  graphMargin = 25.0f;
static const float  tickSize = 3;

void ProfilerOverlay::init()
{
    mSelection = INVALID_SELECTION;
    sx = sy =  1.0f;
    dx = 0.0f;
    mDoDrag = false;
}

void ProfilerOverlay::resize(int w, int h)
{
    width = w; height = h;

    graphArea = {graphMargin, graphMargin, w - 2.0f * graphMargin, h - 2.0f * graphMargin};
}

void ProfilerOverlay::fini()
{
    glDeleteProgram(prgQuad);
}

void ProfilerOverlay::loadProfilerData()
{
    size_t                  numEvents;
    const profiler_event_t* events;
    const char**            names;
    profilerGetData(&numEvents, &events, &names);

    rectData.resize(numEvents * 4);

    minTime = events[0].timestamp,
    maxTime = events[0].timestamp;

    for (size_t i = 1; i < numEvents; ++i)
    {
        minTime = core::min(minTime, events[i].timestamp);
        maxTime = core::max(maxTime, events[i].timestamp);
    }

    int     stackTop[MAX_THREAD_COUNT] = {-1, -1, -1, -1, -1, -1, -1, -1};
    struct
    {
        size_t   id;
        uint32_t sliceBegin;
    } stack[MAX_THREAD_COUNT][MAX_STACK_DEPTH];

    sx = graphArea.w/(maxTime - minTime);
    sy = 20.0f;
    dx = -sx*maxTime;

    core::index_t<uint16_t, ui::RAINBOW_TABLE_L_SIZE> colorMap  = {0};
    core::index_t<uint16_t, 32>                       threadMap = {0};

    rectData.clear();
    intervals.clear();
    colors.clear();

    numThreads = 0;

    for (size_t i=0; i<numEvents; ++i)
    {
        profiler_event_t event = events[i];
        uint32_t         threadIdx;

        threadIdx = core::index_lookup_or_add(&threadMap, event.tid);

        numThreads = core::max(numThreads, threadIdx);

        int& top = stackTop[threadIdx];

        if (event.phase==PROF_EVENT_PHASE_BEGIN)
        {
            //Add event to stack
            ++top;
            stack[threadIdx][top].id         = event.id;
            stack[threadIdx][top].sliceBegin = event.timestamp;
        }
        else if (event.phase==PROF_EVENT_PHASE_END)
        {
            //remove item from stack and add entry
            assert(top>=0);
            assert(top<MAX_STACK_DEPTH);

            assert (stack[threadIdx][top].id == event.id); //implement backtracking end fixing later

            float xstart = (float)stack[threadIdx][top].sliceBegin;
            float xend   = (float)event.timestamp;
            float ystart = top+0.1f;
            float yend   = top+0.9f;

            ystart += threadIdx * (MAX_STACK_DEPTH + 2) + firstThreadRow;
            yend   += threadIdx * (MAX_STACK_DEPTH + 2) + firstThreadRow;

            uint32_t colorIdx;
            uint32_t color;

            colorIdx = core::index_lookup_or_add(&colorMap, event.id);
            color    = ui::rainbowTableL[colorIdx];

            colors.push_back(color);

            rectData.push_back(xstart);
            rectData.push_back(ystart);
            rectData.push_back(xend);
            rectData.push_back(yend);

            Interval inter = 
            {
                names[event.id],
                xstart / 1000.0f,
                (xend-xstart) / 1000.0f
            };

            intervals.push_back(inter);

            --top;
        }
    }

    ++numThreads;

    prevMouseMoveTime = 0;
    showTooltip = false;
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

void ProfilerOverlay::drawBars(uint32_t* colorArray)
{
    PROFILER_CPU_TIMESLICE("ProfilerOverlay::drawBars");

    char  strBuf[128];
    float tx = graphArea.x+dx;

    nvgBeginFrame(vg::ctx, gfx::width, gfx::height, (float)gfx::width/gfx::height);

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

        size_t numRects = colors.size();
        for (size_t i=0; i<numRects; ++i)
        {
            float x0 = rectData[i*4];
            float y0 = rectData[i*4+1];
            float x1 = rectData[i*4+2];
            float y1 = rectData[i*4+3];

            x0 = x0*sx+tx;
            y0 = y0*sy+graphArea.y;
            x1 = x1*sx+tx;
            y1 = y1*sy+graphArea.y;

            if (x0>graphArea.x+graphArea.w || x1<graphArea.x || x1-x0<1.0f)
                continue;

            x0 = ml::clamp(x0, graphArea.x, graphArea.x+graphArea.w);
            x1 = ml::clamp(x1, graphArea.x, graphArea.x+graphArea.w);
            y0 = ml::clamp(y0, graphArea.y, graphArea.y+graphArea.h);
            y1 = ml::clamp(y1, graphArea.y, graphArea.y+graphArea.h);

            uint32_t c = colors[i];
            v128 cf = vi_cvt_ubyte4_to_vec4(c);
            nvgFillColor(vg::ctx, nvgRGBAf(cf.m128_f32[0], cf.m128_f32[1], cf.m128_f32[2], cf.m128_f32[3]));
            nvguRect(vg::ctx, x0, y0, x1-x0, y1-y0);
        }
    }

    int x, y;
    ui::mouseAbsOffset(&x, &y);

    if (showTooltip)
    {
        size_t selected  = elementUnderCursor(x, y);

        if (selected != INVALID_SELECTION)
        {
            Interval& interval = intervals[selected];

            cstr1024 buffer;
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

            float bx = x+mg+8;
            float by = y+32;

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

    nvgEndFrame(vg::ctx);

    gfx::set2DStates();
}

size_t ProfilerOverlay::elementUnderCursor(int x, int y)
{
    point_t pt = {(x-graphArea.x-dx)/sx, (y-graphArea.y)/sy};

    for (size_t i = 0; i < rectData.size(); i +=4)
    {
        rect_t rect = {rectData[i], rectData[i+1], rectData[i+2]-rectData[i], rectData[i+3]-rectData[i+1]};

        if (testPtInRect(pt, rect))
        {
            return i / 4;
        }
    }

    return INVALID_SELECTION;
}

void ProfilerOverlay::updateUI(float delta)
{
    float keybScaleSpeed  = 0.50f; // (increase/s)
    float mouseScaleSpeed = 0.10f; // (increase)

    int x, y;
    int mdx, mdy;

    ui::mouseAbsOffset(&x, &y);
    ui::mouseRelOffset(&mdx, &mdy);

    static const int MOUSE_MOVE_OFFSET_THRESHOLD = 5;
    static const int MOUSE_IDLE_TIME_THRESHOLD = 1*1000*1000; //2s

    if (abs(mdx)+abs(mdy)>MOUSE_MOVE_OFFSET_THRESHOLD || ui::mouseIsPressed(SDL_BUTTON_LEFT) || ui::mouseIsPressed(SDL_BUTTON_RIGHT))
    {
        prevMouseMoveTime = timerAbsoluteTime();
        showTooltip = false;
    }

    if (timerAbsoluteTime()-prevMouseMoveTime>MOUSE_IDLE_TIME_THRESHOLD && !showTooltip)
    {
        showTooltip = true;
    }

    if (ui::mouseWasReleased(SDL_BUTTON_LEFT))
        mSelection = elementUnderCursor(x, y);

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

    // Background
    vg::drawRect(0.0f, 0.0f, (float)width, (float)height, 0xF01A1A1A, 0xF01A1A1A);

    if (!rectData.empty())
        drawBars(&colors[0]);

    if (mSelection != INVALID_SELECTION && mSelection < intervals.size())
    {
        char str[256];

        Interval& interval = intervals[mSelection];

        sprintf_s(str, "name     : %s", interval.name);
        vg::drawString(vg::defaultFont, 50, 550, 0xFFFFFFFF, str, strlen(str));

        sprintf_s(str, "duration : %.4f", interval.duration);
        vg::drawString(vg::defaultFont, 50, 565, 0xFFFFFFFF, str, strlen(str));

        sprintf_s(str, "start    : %.4f", interval.start);
        vg::drawString(vg::defaultFont, 50, 580, 0xFFFFFFFF, str, strlen(str));

        sprintf_s(str, "end      : %.4f", interval.start+interval.duration);
        vg::drawString(vg::defaultFont, 50, 595, 0xFFFFFFFF, str, strlen(str));
    }
}
