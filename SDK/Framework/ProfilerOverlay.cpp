#include <windows.h>
#include <opengl.h>
#include <vg/vg.h>

#include "ProfilerOverlay.h"
#include "profiler.h"

#include "ui.h"
#include <vg/vg.h>

#include <map>

static const uint32_t profilerIntervalMask = 0xFEDC0000;

void ProfilerOverlay::initialise(int w, int h)
{
    mWidth = (float)w; mHeight=(float)h;

    float w1=mWidth-60.0f, h1=mHeight-60.0f;/*60=30+30*/

    mSelection = -1;
    mScale = 1.0f;
    mDoDrag = false;
    mOffsetX = 0.0f;
}

void ProfilerOverlay::loadProfilerData()
{
    static const size_t MAX_STACK_DEPTH = 8;

    size_t                  numEvents;
    const profiler_event_t* events;

    profilerGetData(&numEvents, &events);

    drawData.resize(numEvents * 4);

    __int64 minTime = events[0].timestamp,
            maxTime = events[0].timestamp;

    for (size_t i = 1; i < numEvents; ++i)
    {
        minTime = min(minTime, events[i].timestamp);
        maxTime = max(maxTime, events[i].timestamp);
    }

    int     stackTop = -1;
    struct
    {
        size_t  id;
        __int64 sliceBegin;
    } stack[MAX_STACK_DEPTH];

    float scale = (mWidth-70.0f) / float(maxTime-minTime);

    std::map<size_t, uint32_t> colorMap;
    size_t colorIndex = 0;

    drawData.clear();
    intervals.clear();
    colors.clear();
    ids.clear();

    for (size_t i=0; i<numEvents; ++i)
    {
        profiler_event_t event = events[i];

        if (event.phase==PROF_EVENT_PHASE_BEGIN)
        {
            //Add event to stack
            ++stackTop;
            stack[stackTop].id         = event.id;
            stack[stackTop].sliceBegin = event.timestamp;
        }
        else if (event.phase==PROF_EVENT_PHASE_END)
        {
            //remove item from stack and add entry
            assert(stackTop>=0);
            assert(stackTop<MAX_STACK_DEPTH);

            assert (stack[stackTop].id == event.id); //implement backtracking end fixing later
           
            float xstart = (stack[stackTop].sliceBegin-minTime) * scale + 5.0f;
            float xend   = (event.timestamp-minTime) * scale + 5.0f;
            float ystart = 20.0f+stackTop*20;
            float yend   = 20.0f+stackTop*20+15;

            uint32_t color = colorMap[event.id];
            
            if (color == 0)
            {
                color = colorIndex>=ui::RAINBOW_TABLE_L_SIZE ? 0xFFFFFFFF : ui::rainbowTableL[colorIndex];
                if  (colorIndex<ui::RAINBOW_TABLE_L_SIZE) ++colorIndex;
                colorMap[event.id] = color;
            }

            colors.push_back(color);
            colors.push_back(color);
            colors.push_back(color);
            colors.push_back(color);

            ids.push_back(intervals.size()|profilerIntervalMask);
            ids.push_back(intervals.size()|profilerIntervalMask);
            ids.push_back(intervals.size()|profilerIntervalMask);
            ids.push_back(intervals.size()|profilerIntervalMask);

            Vertex vtx[4] = 
            {
                {xstart, ystart},
                {xend,   ystart},
                {xend,   yend  },
                {xstart, yend  },
            };

            drawData.push_back(vtx[0]);
            drawData.push_back(vtx[1]);
            drawData.push_back(vtx[2]);
            drawData.push_back(vtx[3]);

            Interval inter = 
            {
                (const char*) event.id,
                (event.timestamp-stack[stackTop].sliceBegin) / 1000.0f,
                xstart, ystart,
                xend, yend
            };

            intervals.push_back(inter);

            --stackTop;
        }
    }
}

void ProfilerOverlay::drawBars(uint32_t* colorArray)
{
    float w1=mWidth-80.0f, h1=mHeight-80.0f;

    glPushMatrix();
    glTranslatef(30.0f+mOffsetX, 30.0f, 0.0f);
    glScalef(mScale, 1.0f, 1.0f);

    glEnable(GL_SCISSOR_TEST);
    glScissor(30, 30, (int)mWidth-60, (int)mHeight-60);

    glUseProgram(0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &drawData[0].x);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(uint32_t), colorArray);

    glDrawArrays(GL_QUADS, 0, drawData.size());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

void ProfilerOverlay::updateUI()
{
    int offx;

    static const float scaleUp   = 1.25;
    static const float scaleDown = 1.0f / 1.25f;

    if (!drawData.empty())
        drawBars(&ids[0]);

    uint32_t id = ui::mouseOverID();
    if (((id & profilerIntervalMask) == profilerIntervalMask) && ui::mouseWasReleased(SDL_BUTTON_LEFT))
    {
        mSelection = id & 0xFFFF;
    }

    if (ui::keyWasReleased(SDL_SCANCODE_EQUALS) || ui::mouseWasWheelUp())
    {
        ui::mouseAbsOffset(&offx, NULL);
        mScale*=scaleUp;
        offx -= 30;
        mOffsetX = (1.0f-scaleUp) * offx + scaleUp * mOffsetX;
    }
    if (ui::keyWasReleased(SDL_SCANCODE_MINUS) || ui::mouseWasWheelDown())
    {
        ui::mouseAbsOffset(&offx, NULL);
        mScale*=scaleDown;
        offx -= 30;
        mOffsetX = (1.0f-scaleDown) * offx + scaleDown * mOffsetX;
    }

    if (ui::mouseWasPressed(SDL_BUTTON_LEFT))
    {
        mDoDrag = true;
    }
    if (mDoDrag)
    {
        ui::mouseRelOffset(&offx, NULL);
        mOffsetX += offx;
    }
    if (ui::mouseWasReleased(SDL_BUTTON_LEFT) && mDoDrag)
    {
        mDoDrag = false;
    }
}

void ProfilerOverlay::renderFullscreen()
{
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, mWidth, 0, mHeight, 0, 500);
    glTranslatef(0, mHeight, 0);
    glScalef(1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);

    float w1=mWidth-80.0f, h1=mHeight-80.0f;

    glUseProgram(0);
    
    // Background
    glColor4f(0.1f, 0.1f, 0.1f, 0.93f);
    glBegin(GL_QUADS);
    glVertex3f(30.0f, 30.0f, 0.0f);
    glVertex3f(mWidth-30.0f, 30.0f, 0.0f);
    glVertex3f(mWidth-30.0f, mHeight-30.0f, 0.0f);
    glVertex3f(30.0f, mHeight-30.0f, 0.0f);
    glEnd();

    if (!drawData.empty())
        drawBars(&colors[0]);

    if (mSelection >= 0 && mSelection < intervals.size())
    {
        char str[256];

        glColor3f(1.0f, 1.0f, 1.0f);

        _snprintf(str, 256, "name     : %s", intervals[mSelection].name);
        vg::drawString(ui::defaultFont, 50, 150, str, strlen(str));

        _snprintf(str, 256, "duration : %.2f", intervals[mSelection].duration);
        vg::drawString(ui::defaultFont, 50, 165, str, strlen(str));
    }

    glPopAttrib();
}
