#include <windows.h>
#include <opengl.h>
#include <vg/vg.h>

#include "ProfilerOverlay.h"
#include "profiler.h"
#include <ResourceHelpers.h>
#include <Framework.h>

#include "ui.h"
#include <vg/vg.h>
#include <utils.h>

static const uint32_t profilerIntervalMask = 0xFEDC0000;

const char vsQuadSource[] = 
    "layout(location=0) in vec4  aRect;                         \n"
    "layout(location=1) in vec4  aColor;                        \n"
    "                                                           \n"
    "uniform mat4 uMVP;                                         \n"
    "                                                           \n"
    "out vec4 vColor;                                           \n"
    "                                                           \n"
    "void main()                                                \n"
    "{                                                          \n"
    "vec4 vertex;                                               \n"
    "vertex.x = ((gl_VertexID & 1) != 0) ? aRect.x : aRect.z;   \n"
    "vertex.y = ((gl_VertexID & 2) != 0) ? aRect.y : aRect.w;   \n"
    "vertex.z = 0.0;                                            \n"
    "vertex.w = 1.0;                                            \n"
    "   gl_Position = gl_ModelViewProjectionMatrix * vertex;    \n"
    "   vColor      = aColor;                                   \n"
    "}                                                          \n";

const char fsQuadSource[] = 
    "in vec4 vColor;                                            \n"
    "                                                           \n"
    "layout(location = 0, index = 0) out vec4 outColor;         \n"
    "                                                           \n"
    "void main()                                                \n"
    "{                                                          \n"
    "   outColor = vColor;                                      \n"
    "}                                                          \n";

GLuint prgQuad;

void ProfilerOverlay::init(int w, int h)
{
    mWidth = (float)w; mHeight=(float)h;

    float w1=mWidth-60.0f, h1=mHeight-60.0f;/*60=30+30*/

    mSelection = -1;
    mScale = 1.0f;
    mDoDrag = false;
    mOffsetX = 0.0f;

	prgQuad = glCreateProgram();
	resources::compileAndAttachShader(prgQuad, GL_VERTEX_SHADER,   sizeof(vsQuadSource)-1, vsQuadSource);
	resources::compileAndAttachShader(prgQuad, GL_FRAGMENT_SHADER, sizeof(fsQuadSource)-1, fsQuadSource);
    glLinkProgram(prgQuad);

    const size_t	LOG_STR_LEN = 1024;
    char			infoLog[LOG_STR_LEN] = {0};
    GLsizei			length;

    glGetProgramInfoLog(prgQuad, LOG_STR_LEN-1, &length, infoLog);
    infoLog[length] = 0;

    if (infoLog[0])
    {
        logging::message("%s\n", infoLog);
    }

    uniMVP = glGetUniformLocation(prgQuad, "uMVP");
}

void ProfilerOverlay::fini()
{
    glDeleteProgram(prgQuad);
}

void ProfilerOverlay::loadProfilerData()
{
    static const size_t MAX_STACK_DEPTH  = 8;
    static const size_t MAX_THREAD_COUNT = 8;

    size_t                  numEvents;
    const profiler_event_t* events;

    profilerGetData(&numEvents, &events);

    rectData.resize(numEvents * 4);

    __int64 minTime = events[0].timestamp,
            maxTime = events[0].timestamp;

    for (size_t i = 1; i < numEvents; ++i)
    {
        minTime = std::min(minTime, events[i].timestamp);
        maxTime = std::max(maxTime, events[i].timestamp);
    }

    int     stackTop[MAX_THREAD_COUNT] = {-1, -1, -1, -1, -1, -1, -1, -1};
    struct
    {
        size_t  id;
        __int64 sliceBegin;
    } stack[MAX_THREAD_COUNT][MAX_STACK_DEPTH];

    float scale = (mWidth-70.0f) / float(maxTime-minTime);

    ut::vindex32_t colorMap;
    ut::vindex32_t threadMap;
    
    ut::vindex_create(&colorMap, ui::RAINBOW_TABLE_L_SIZE);
    ut::vindex_create(&threadMap, 32);

    rectData.clear();
    intervals.clear();
    colors.clear();
    ids.clear();

    for (size_t i=0; i<numEvents; ++i)
    {
        profiler_event_t event = events[i];
        uint32_t         threadIdx;

        ut::vindex_get(&threadMap, event.threadID, &threadIdx);

        int&   top       = stackTop[threadIdx];

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

            float xstart = (stack[threadIdx][top].sliceBegin-minTime) * scale + 5.0f;
            float xend   = (event.timestamp-minTime) * scale + 5.0f;
            float ystart = 20.0f+top*20;
            float yend   = 20.0f+top*20+15;

            ystart += threadIdx * 200;
            yend   += threadIdx * 200;

            uint32_t colorIndex;
            uint32_t color;

            ut::vindex_get(&colorMap, event.id, &colorIndex);
            color = ui::rainbowTableL[colorIndex];

            colors.push_back(color);

            ids.push_back(intervals.size()|profilerIntervalMask);

            rectData.push_back(xstart);
            rectData.push_back(ystart);
            rectData.push_back(xend);
            rectData.push_back(yend);

            Interval inter = 
            {
                (const char*) event.id,
                (event.timestamp-stack[threadIdx][top].sliceBegin) / 1000.0f
            };

            intervals.push_back(inter);

            --top;
        }
    }

    ut::vindex_destroy(colorMap);
    ut::vindex_destroy(threadMap);
}

void drawRects(GLsizei count, float* rects, uint32_t* colors)
{
    glUseProgram(prgQuad);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(0, 4, GL_FLOAT,         GL_FALSE, 0, rects);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,  0, colors);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void ProfilerOverlay::drawBars(uint32_t* colorArray)
{
    float w1=mWidth-80.0f, h1=mHeight-80.0f;

    glPushMatrix();
    glTranslatef(30.0f+mOffsetX, 30.0f, 0.0f);
    glScalef(mScale, 1.0f, 1.0f);

    glEnable(GL_SCISSOR_TEST);
    glScissor(30, 30, (int)mWidth-60, (int)mHeight-60);

    drawRects(rectData.size()/4, &rectData[0], colorArray);

    glUseProgram(0);

    glPopMatrix();
}

void ProfilerOverlay::updateUI()
{
    int offx;

    static const float scaleUp   = 1.25;
    static const float scaleDown = 1.0f / 1.25f;

    if (!rectData.empty())
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

    if (!rectData.empty())
        drawBars(&colors[0]);

    if (mSelection >= 0 && mSelection < intervals.size())
    {
        char str[256];

        glColor3f(1.0f, 1.0f, 1.0f);

        _snprintf(str, 256, "name     : %s", intervals[mSelection].name);
        vg::drawString(ui::defaultFont, 50, 550, str, strlen(str));

        _snprintf(str, 256, "duration : %.2f", intervals[mSelection].duration);
        vg::drawString(ui::defaultFont, 50, 565, str, strlen(str));
    }

    glPopAttrib();
}
