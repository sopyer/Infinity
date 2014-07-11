#include <cstdlib>
#include <opengl.h>
#include <SDL2/SDL.h>
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

void Platform_Initialise();
void Platform_Finalise();

enum
{
    STATE_DEFAULT,
    STATE_SHADER_EDIT,
    STATE_PROFILER
};

enum
{
    PROF_STATE_NO_CAPTURE,
    PROF_STATE_FRAME_CAPTURE,
    PROF_STATE_TIMESLICE_CAPTURE,
    PROF_STATE_DATA_RETRIEVAL
};

namespace ui
{
    Stage::Stage():
        mState(STATE_DEFAULT),
        mProfilerState(PROF_STATE_NO_CAPTURE),
        mRunLoop(true)
    {
        ////!!!! Refactor
        mFullscreen = false;
        mWidth = 1280;
        mHeight = 720;

        ml::make_ortho2D_mat4(mProj, 0, mWidth, mHeight, 0);

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

        if (mProfilerState == PROF_STATE_DATA_RETRIEVAL)
        {
            mProfilerState = PROF_STATE_NO_CAPTURE;
            mProfilerOverlay->loadProfilerData();
        }
        else if (
            mProfilerState==PROF_STATE_TIMESLICE_CAPTURE &&
            ui::keyIsPressed(SDL_SCANCODE_GRAVE) &&
            ui::keyWasReleased(SDL_SCANCODE_T)
        )
        {
            mProfilerState = PROF_STATE_DATA_RETRIEVAL;
            profilerStopCapture();
        }
        else if (mProfilerState == PROF_STATE_FRAME_CAPTURE)
        {
            mProfilerState = PROF_STATE_DATA_RETRIEVAL;
            profilerStopCapture();
        }
        else if (
            mProfilerState==PROF_STATE_NO_CAPTURE &&
            ui::keyIsPressed(SDL_SCANCODE_GRAVE)  &&
            ui::keyWasReleased(SDL_SCANCODE_T)
        )
        {
            mProfilerState = PROF_STATE_TIMESLICE_CAPTURE;
            profilerStartCapture();
        }
        else if (
            mProfilerState==PROF_STATE_NO_CAPTURE &&
            ui::keyIsPressed(SDL_SCANCODE_GRAVE)  &&
            ui::keyWasReleased(SDL_SCANCODE_F)
        )
        {
            mProfilerState = PROF_STATE_FRAME_CAPTURE;
            profilerStartCapture();
        }
        else if (
            mProfilerState!=PROF_STATE_NO_CAPTURE &&
            mProfilerState!=PROF_STATE_TIMESLICE_CAPTURE
        )
        {
            assert(!"Invalid state");
        }

        SDL_Event   E;
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
            profilerStartSyncPoint();
            {
                PROFILER_CPU_TIMESLICE("Frame");

                {
                    PROFILER_CPU_TIMESLICE("Frame sync");
                    gfx::beginFrame();
                }

                glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);

                uint64_t time;
                time = timerAbsoluteTime();
                //TODO: Merge all three
                handleInput();
                ui::update();
                if (mState==STATE_PROFILER)
                {
                    PROFILER_CPU_TIMESLICE("mProfilerOverlay->updateUI");
                    mProfilerOverlay->updateUI((time-mPrevTime)*0.000001f);
                }
                else if (mState==STATE_DEFAULT)
                {
                    PROFILER_CPU_TIMESLICE("onUpdate");
                    onUpdate((time-mPrevTime)*0.000001f);
                }
                if (mShaderEditOverlay->requireReset())
                {
                    PROFILER_CPU_TIMESLICE("onShaderRecompile");
                    onShaderRecompile();
                }
                mPrevTime = time;

                renderActors();

                ui::render();
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

                gfx::endFrame();

                {
                    PROFILER_CPU_TIMESLICE("SDL_GL_SwapBuffers");
                    SDL_GL_SwapWindow(fwk::window);
                }
            }
            profilerStopSyncPoint();
        }
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
        glLoadMatrixf((float*)mProj);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
        onPaint();
        glPopMatrix();
    }
}