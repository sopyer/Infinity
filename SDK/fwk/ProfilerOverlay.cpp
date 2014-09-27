#include <gfx/gfx.h>
#include <fwk/ui.h>

#include "ProfilerOverlay.h"

static const uint32_t profilerIntervalMask = 0xFEDC0000;

const char vsQuadSource[] = 
    "#version 430                                                           \n"
    "                                                                       \n"
    "layout(std140, column_major, binding = 0) uniform uniMVP               \n"
    "{                                                                      \n"
    "    mat4  uMVP;                                                        \n"
    "};                                                                     \n"
    "                                                                       \n"
    "layout(location=0) in vec4  vaRect;                                    \n"
    "layout(location=1) in vec4  vaColor;                                   \n"
    "                                                                       \n"
    "out vec4 faColor;                                                      \n"
    "                                                                       \n"
    "void main()                                                            \n"
    "{                                                                      \n"
    "   float x = ((gl_VertexID & 1) != 0) ? vaRect.x : vaRect.z;           \n"
    "   float y = ((gl_VertexID & 2) != 0) ? vaRect.y : vaRect.w;           \n"
    "                                                                       \n"
    "   gl_Position = uMVP * vec4(x, y, 0, 1);                              \n"
    "   faColor     = vaColor;                                              \n"
    "}                                                                      \n";

const char fsQuadSource[] = 
    "#version 430                                                           \n"
    "                                                                       \n"
    "in vec4 faColor;                                                       \n"
    "                                                                       \n"
    "layout(location = 0, index = 0) out vec4 rt0;                          \n"
    "                                                                       \n"
    "void main()                                                            \n"
    "{                                                                      \n"
    "   rt0 = faColor;                                                      \n"
    "}                                                                      \n";

GLuint prgQuad;

#define INVALID_SELECTION 0xFFFFFFFF

void ProfilerOverlay::init(int w, int h)
{
    mWidth = (float)w; mHeight=(float)h;

    float w1=mWidth-60.0f, h1=mHeight-60.0f;/*60=30+30*/

    mSelection = INVALID_SELECTION;
    mScale = 1.0f;
    mDoDrag = false;
    mOffsetX = 0.0f;

    prgQuad = glCreateProgram();
    res::compileAndAttachShader(prgQuad, GL_VERTEX_SHADER,   sizeof(vsQuadSource)-1, vsQuadSource);
    res::compileAndAttachShader(prgQuad, GL_FRAGMENT_SHADER, sizeof(fsQuadSource)-1, fsQuadSource);
    glLinkProgram(prgQuad);

    const size_t LOG_STR_LEN = 1024;
    char         infoLog[LOG_STR_LEN] = {0};
    GLsizei      length;

    glGetProgramInfoLog(prgQuad, LOG_STR_LEN-1, &length, infoLog);
    infoLog[length] = 0;

    if (infoLog[0])
    {
        fprintf(stderr, "%s\n", infoLog);
    }

    uniMVP = glGetUniformLocation(prgQuad, "uMVP");

    gfx::vertex_element_t ve[2] = {
        {0, 0, 0, GL_FLOAT,         4, GL_FALSE, GL_FALSE},
        {1, 0, 1, GL_UNSIGNED_BYTE, 4, GL_FALSE, GL_TRUE },
    };

    GLuint divs[2] = {1, 1};

    vao = gfx::createVAO(2, ve, 2, divs);
}

void ProfilerOverlay::fini()
{
    glDeleteVertexArrays(1, &vao);
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

    uint64_t minTime = events[0].timestamp,
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
        uint64_t sliceBegin;
    } stack[MAX_THREAD_COUNT][MAX_STACK_DEPTH];

    float scale = (mWidth-70.0f) / float(maxTime-minTime);

    core::index_t<uint32_t, ui::RAINBOW_TABLE_L_SIZE> colorMap  = {0};
    core::index_t<uint64_t, 32>                       threadMap = {0};

    rectData.clear();
    intervals.clear();
    colors.clear();
    ids.clear();

    for (size_t i=0; i<numEvents; ++i)
    {
        profiler_event_t event = events[i];
        uint32_t         threadIdx;

        threadIdx = core::index_lookup_or_add(&threadMap, event.threadID);

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

            float xstart = (stack[threadIdx][top].sliceBegin-minTime) * scale + 5.0f;
            float xend   = (event.timestamp-minTime) * scale + 5.0f;
            float ystart = 20.0f+top*20;
            float yend   = 20.0f+top*20+15;

            ystart += threadIdx * 200;
            yend   += threadIdx * 200;

            uint32_t colorIdx;
            uint32_t color;

            colorIdx = core::index_lookup_or_add(&colorMap, event.id);
            color    = ui::rainbowTableL[colorIdx];

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
}

void ProfilerOverlay::drawBars(uint32_t* colorArray)
{
    PROFILER_CPU_TIMESLICE("ProfilerOverlay::drawBars");
    float w1=mWidth-80.0f, h1=mHeight-80.0f;

    v128 mv[4] = {
        vi_set(  mScale,       0.0f, 0.0f, 0.0f),
        vi_set(    0.0f,       1.0f, 0.0f, 0.0f),
        vi_set(    0.0f,       0.0f, 1.0f, 0.0f),
        vi_set(30.0f+mOffsetX, 30.0f, 0.0f, 1.0f),
    };

    gfx::setModelViewMatrix(mv);

    glEnable(GL_SCISSOR_TEST);
    glScissor(30, 30, (int)mWidth-60, (int)mHeight-60);

    {
        PROFILER_CPU_TIMESLICE("ModernGL");
        glUseProgram(prgQuad);
        gfx::setMVP();

        GLuint rectOffset, colOffset;
        size_t count = colors.size();

        float*   rectDst = (float*)  gfx::dynbufAllocMem(sizeof(float)*4*count,   0, &rectOffset);
        uint8_t* colDst  = (uint8_t*)gfx::dynbufAllocMem(sizeof(uint8_t)*4*count, 0, &colOffset);

        {
            PROFILER_CPU_TIMESLICE("Data copy");
            memcpy(rectDst, &rectData[0], sizeof(float)*4*count);
            memcpy(colDst,  colorArray,   sizeof(uint8_t)*4*count);
        }

        glBindVertexArray(vao);

        glBindVertexBuffer(0, gfx::dynBuffer, rectOffset, sizeof(float)*4);
        glBindVertexBuffer(1, gfx::dynBuffer, colOffset,  sizeof(uint8_t)*4);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        glBindVertexArray(0);
    }

    glDisable(GL_SCISSOR_TEST);
    ml::make_identity_mat4(mv);
    gfx::setModelViewMatrix(mv);
}

void ProfilerOverlay::updateUI(float delta)
{
    int offx;

    float keybScaleSpeed  = 0.50f; // (increase/s)
    float mouseScaleSpeed = 0.10f; // (increase/s)

    int x, y;

    ui::mouseAbsOffset(&x, &y);

    point_t pt = {(x-30.0f-mOffsetX)/mScale, y-30.0f};

    if (ui::mouseWasReleased(SDL_BUTTON_LEFT))
    for (size_t i = 0; i < rectData.size(); i +=4)
    {
        rect_t rect = {rectData[i], rectData[i+1], rectData[i+2]-rectData[i], rectData[i+3]-rectData[i+1]};

        if (testPtInRect(pt, rect))
        {
            mSelection = i / 4;
            break;
        }
    }

    if (ui::keyIsPressed(SDL_SCANCODE_EQUALS) || ui::mouseWasWheelUp())
    {
        float scaleUp = 1.0f + (ui::mouseWasWheelUp() ? mouseScaleSpeed : keybScaleSpeed * delta);

        ui::mouseAbsOffset(&offx, NULL);
        mScale *= scaleUp;
        offx -= 30;
        mOffsetX = (1.0f-scaleUp) * offx + scaleUp * mOffsetX;
    }
    if (ui::keyIsPressed(SDL_SCANCODE_MINUS) || ui::mouseWasWheelDown())
    {
        float scaleDown  = 1.0f / (1.0f + (ui::mouseWasWheelDown() ? mouseScaleSpeed : keybScaleSpeed * delta));

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
    PROFILER_CPU_TIMESLICE("ProfilerOverlay::renderFullscreen");
    glViewport(0, 0, mWidth, mHeight);

    float w1=mWidth-80.0f, h1=mHeight-80.0f;

    // Background
    vg::drawRect(30.0f, 30.0f, mWidth-30.0f, mHeight-30.0f, 0xF01A1A1A, 0xF01A1A1A);

    if (!rectData.empty())
        drawBars(&colors[0]);

    if (mSelection != INVALID_SELECTION && mSelection < intervals.size())
    {
        char str[256];

        _snprintf(str, 256, "name     : %s", intervals[mSelection].name);
        vg::drawString(vg::defaultFont, 50, 550, 0xFFFFFFFF, str, strlen(str));

        _snprintf(str, 256, "duration : %.4f", intervals[mSelection].duration);
        vg::drawString(vg::defaultFont, 50, 565, 0xFFFFFFFF, str, strlen(str));
    }
}
