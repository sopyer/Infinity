#define GLEW_STATIC
#include <windows.h>
#include <cstdlib>
#include <opengl.h>
#include <SOIL.h> // remove later
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <utils.h>
#include <vg/vg.h>
#include <vg/impl/SharedResources.h>
#include <ShaderEditOverlay.h>
#include <ProfilerOverlay.h>
#include <profiler.h>
#include "UI.h"
#include "Framework.h"
#include "Timer.h"
#include "Stage.h"
#include "graphics.h"

void Platform_Initialise(HWND hWnd);
void Platform_Finalise();

enum
{
    STATE_DEFAULT,
    STATE_SHADER_EDIT,
    STATE_PROFILER
};

namespace ui
{
    Stage::Stage():
        mState(STATE_DEFAULT),
        doFrameCapture(false),
        doTimesliceCapture(false),
        mRunLoop(true)
#if defined(DEBUG) || defined(_DEBUG)
        ,dumpPickImage(0)
#endif
    {
        ////!!!! Refactor
        mFullscreen = false;
        mWidth = 1280;
        mHeight = 720;

        setProjection(glm::perspectiveGTX(60.0f, mWidth/mHeight, 0.1f, 600.0f));

        SDL_SysWMinfo info = {0, SDL_SYSWM_UNKNOWN, {0}};
        SDL_GetWindowWMInfo(fwk::window, &info);
        Platform_Initialise(info.info.win.window);

        mShaderEditOverlay = new ShaderEditOverlay;
        mShaderEditOverlay->initialise(mWidth, mHeight);

        mProfilerOverlay = new ProfilerOverlay;
        mProfilerOverlay->init(mWidth, mHeight);

        GLuint programs[] = {
            impl::simpleUIProgram, impl::stencilArcAreaProgram, impl::stencilQuadAreaProgram,
            impl::stencilCubicAreaAAProgram, impl::stencilCubicAreaProgram, impl::linGradProgram
        };

        mShaderEditOverlay->addPrograms(ARRAY_SIZE(programs), programs);

        mPrevTime = timerAbsoluteTime();

        //TODO: HACK!!!!!
        SDL_StartTextInput();
    }

    Stage::~Stage()
    {
        delete mShaderEditOverlay;
        mProfilerOverlay->fini();
        delete mProfilerOverlay;
        Platform_Finalise();
    }

    void Stage::addPrograms(size_t count, GLuint* programs)
    {
        mShaderEditOverlay->addPrograms(count, programs);
    }

    void onSDLEvent(SDL_Event& event);

    void Stage::handleInput()
    {
        PROFILER_CPU_TIMESLICE("handleInput");

        if (ui::keyIsPressed(SDL_SCANCODE_GRAVE) && ui::keyWasReleased(SDL_SCANCODE_T))
        {
            doTimesliceCapture = !doTimesliceCapture;
        }
        else if (ui::keyIsPressed(SDL_SCANCODE_GRAVE) && ui::keyWasReleased(SDL_SCANCODE_F))
        {
            doFrameCapture = !doTimesliceCapture;
        }

#if defined(DEBUG) || defined(_DEBUG)
        if (ui::keyIsPressed(SDL_SCANCODE_GRAVE) && ui::keyWasReleased(SDL_SCANCODE_P))
        {
            dumpPickImage = true;
        }
#endif
        SDL_Event	E;
        while (SDL_PollEvent(&E))
        {
            switch(E.type)
            {
            case SDL_QUIT:
                close();
                break;
            case SDL_TEXTINPUT:
                 if (mState==STATE_SHADER_EDIT) mShaderEditOverlay->inputText(E.text.text);
                break;

            case SDL_KEYDOWN:
                switch (mState)
                {
                    case STATE_SHADER_EDIT:
                        mShaderEditOverlay->handleKeyDown(E.key);
                        break;
                }
                break;
            case SDL_KEYUP:		
                if (E.key.keysym.sym==SDLK_ESCAPE)
                {
                    close();
                }
                else if (E.key.keysym.sym==SDLK_F5)
                {
                    mState = mState==STATE_SHADER_EDIT?STATE_DEFAULT:STATE_SHADER_EDIT;
                    if (mState==STATE_SHADER_EDIT) mShaderEditOverlay->reset();
                }
                else if (E.key.keysym.sym==SDLK_F4)
                {
                    mState = mState==STATE_PROFILER?STATE_DEFAULT:STATE_PROFILER;
                }
                break;
            default:
                ui::onSDLEvent(E);
            }
        }
    }

    void Stage::close()
    {
        mRunLoop = false;
    }

    void Stage::setCaption(const char* caption)
    {
        SDL_SetWindowTitle(fwk::window, caption);
    }

    void Stage::run()
    {
        while (mRunLoop)
        {
            static const char* strFrame = "Frame";

            if (doFrameCapture && !profilerIsCaptureInProgress())
            {
                profilerBeginDataCapture();
            }
            else if (doFrameCapture && profilerIsCaptureInProgress())
            {
                profilerEndDataCapture();
                mProfilerOverlay->loadProfilerData();
                doFrameCapture = false;
            }
            else if (doTimesliceCapture && !profilerIsCaptureInProgress())
            {
                profilerBeginDataCapture();
            }
            else if (!doTimesliceCapture && profilerIsCaptureInProgress())
            {
                profilerEndDataCapture();
                mProfilerOverlay->loadProfilerData();
            }
            PROFILER_CPU_TIMESLICE("Frame");
        
            {
                PROFILER_CPU_TIMESLICE("Frame sync");
                graphics::beginFrame();
            }

            glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);

            //TODO: Merge all three
            handleInput();
            ui::update();
            outlineActors();
        
            if (mShaderEditOverlay->requireReset())
            {
                onShaderRecompile();
            }

            renderActors();

            if (mState==STATE_PROFILER)
            {
                PROFILER_CPU_TIMESLICE("mProfilerOverlay->renderFullscreen");
                mProfilerOverlay->renderFullscreen();
            }
            if (mState==STATE_SHADER_EDIT)
            {
                PROFILER_CPU_TIMESLICE("mShaderEditOverlay->renderFullscreen");
                mShaderEditOverlay->renderFullscreen();
            }
        
            graphics::endFrame();

            {
                PROFILER_CPU_TIMESLICE("SDL_GL_SwapBuffers");
                SDL_GL_SwapWindow(fwk::window);
            }
        }
    }

    Stage& Stage::setProjection(float* mat4x4)
    {
        mProjection = glm::mat4(
            mat4x4[0],  mat4x4[1],  mat4x4[2],  mat4x4[3], 
            mat4x4[4],  mat4x4[5],  mat4x4[6],  mat4x4[7], 
            mat4x4[8],  mat4x4[9],  mat4x4[10], mat4x4[11], 
            mat4x4[12], mat4x4[13], mat4x4[14], mat4x4[15]
        );
        return *this;
    }
    
    //!!!!!!!!!!!Refactor
    void setupUIViewMatrix(const glm::mat4& proj, float width, float height)
    {
        glm::mat4 inv = glm::inverseGTX(proj);
        glm::vec4 ur = inv*glm::vec4(1, 1, -1, 1);
        ur/=ur.w;

        glLoadIdentity();
        glScalef(ur.x, ur.y, 1);
        glTranslatef(-1, 1, -0.1f/*z_near*/);
        glScalef(2.0f/width, -2.0f/height, 1.0f/width);
    }

    void Stage::outlineActors()
    {
        PROFILER_CPU_TIMESLICE("outlineActors");
        beginPickOutline();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(mProjection);
        glMatrixMode(GL_MODELVIEW);
        setupUIViewMatrix(mProjection, mWidth, mHeight);

        if (mState==STATE_PROFILER)
        {
            mProfilerOverlay->updateUI();
        }
        else if (mState==STATE_DEFAULT)
        {
            uint64_t time;

            time = timerAbsoluteTime();
            onUpdate((time-mPrevTime)*0.000001f);
            mPrevTime = time;
        }

        endPickOutline();

#if defined(DEBUG) || defined(_DEBUG)
        if (dumpPickImage)
        {
            GLuint w = (GLuint)mWidth;
            GLuint h = (GLuint)mHeight;
            uint8_t* p = new uint8_t[w*h*4];
            ui::readPickBuffer(w*h*4, p);
            SOIL_save_image("pick.tga", SOIL_SAVE_TYPE_TGA, w, h, 4, p);		
            delete [] p;
            dumpPickImage = 0;
        }
#endif
    }

    void Stage::renderActors()
    {
        PROFILER_CPU_TIMESLICE("renderActors");
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0);
        glClearStencil(0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::perspectiveGTX(60.0f, mWidth/mHeight, 0.1f, 600.0f)/*mProjection*/);

        glMatrixMode(GL_MODELVIEW);
        setupUIViewMatrix(mProjection, mWidth, mHeight);

        glPushMatrix();
        onPaint();
        glPopMatrix();

        ui::render();

        glFlush();
    }
}