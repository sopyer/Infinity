#include <windows.h>
#include <opengl.h>
#include <vg/vg.h>

#include "ProfilerOverlay.h"
#include "profiler.h"

#include "ui.h"

#include <map>

//HACK: already defined in shader overlay!!!!
void SetAStyle(Editor& ed, int style, Colour fore, Colour back=0xFFFFFFFF, int size=-1, const char *face=0);
//{
//	ed.Command(SCI_STYLESETFORE, style, fore);
//	ed.Command(SCI_STYLESETBACK, style, back);
//	if (size >= 1)
//		ed.Command(SCI_STYLESETSIZE, style, size);
//	if (face) 
//		ed.Command(SCI_STYLESETFONT, style, reinterpret_cast<sptr_t>(face));
//}

void ProfilerOverlay::initialiseView() {
    mProfilerView.Command(SCI_SETSTYLEBITS, 7);

    // Set up the global default style. These attributes are used wherever no explicit choices are made.
    SetAStyle(mProfilerView, STYLE_DEFAULT, 0xFF00FF00, 0xD0000000, 16, "c:/windows/fonts/cour.ttf");
    mProfilerView.Command(SCI_STYLECLEARALL);	// Copies global style to all others
    mProfilerView.Command(SCI_SETMARGINWIDTHN, 1, 0);//Calculate correct width

    mProfilerView.Command(SCI_SETUSETABS, 1);
    mProfilerView.Command(SCI_SETTABWIDTH, 4);

    mProfilerView.Command(SCI_SETSELBACK,            1, 0xD0CC9966);
    mProfilerView.Command(SCI_SETCARETFORE,          0x00FFFFFF, 0);
    mProfilerView.Command(SCI_SETCARETLINEVISIBLE,   1);
    mProfilerView.Command(SCI_SETCARETLINEBACK,      0xFFFFFFFF);
    mProfilerView.Command(SCI_SETCARETLINEBACKALPHA, 0x20);

    mProfilerView.Command(SCI_SETFOCUS, true);
}

void ProfilerOverlay::initialise(int w, int h)
{
    initialiseView();

    mWidth = (float)w; mHeight=(float)h;

    float w1=mWidth-60.0f, h1=mHeight-60.0f;/*60=30+30*/

    mProfilerView.SetSize(w1, h1);
}

void ProfilerOverlay::update()
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

            Vertex vtx[4] = 
            {
                {xstart, ystart, color},
                {xend,   ystart, color},
                {xend,   yend,   color},
                {xstart, yend,   color},
            };

            drawData.push_back(vtx[0]);
            drawData.push_back(vtx[1]);
            drawData.push_back(vtx[2]);
            drawData.push_back(vtx[3]);

            --stackTop;
        }
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

    //mProfilerView.Paint();

    glUseProgram(0);
    
    // Background
    glColor4f(0.1f, 0.1f, 0.1f, 0.93f);
    glBegin(GL_QUADS);
    glVertex3f(30.0f, 30.0f, 0.0f);
    glVertex3f(mWidth-30.0f, 30.0f, 0.0f);
    glVertex3f(mWidth-30.0f, mHeight-30.0f, 0.0f);
    glVertex3f(30.0f, mHeight-30.0f, 0.0f);
    glEnd();

    glTranslatef(30.0f, 30.0f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    for (size_t i = 0; i < drawData.size(); ++i)
    {
        glColor4ubv((GLubyte*)&drawData[i].color);
        glVertex2fv(&drawData[i].x);
    }
    glEnd();

    glPopAttrib();
}

void ProfilerOverlay::handleKeyDown(SDL_KeyboardEvent& event)
{
    switch (event.keysym.sym)
    {
    case SDLK_UP:
        mProfilerView.Command(SCI_LINEUP);
        break;
    case SDLK_DOWN:
        mProfilerView.Command(SCI_LINEDOWN);
        break;
    }
}
