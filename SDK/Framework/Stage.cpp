#include <cstdlib>
#include <opengl.h>
#include <SDL2/SDL.h>
#include <utils.h>
#include <vg/vg.h>
#include <profiler.h>
#include "UI.h"
#include "ShaderEditOverlay.h"
#include "Framework.h"
#include "Timer.h"
#include "Stage.h"
#include "graphics.h"

namespace fwk
{
    extern bool runLoop;
    extern bool recompileGLPrograms;
}

namespace ui
{
    extern ShaderEditOverlay*  overlayShaderEdit;

    Stage::Stage()
    {
        ////!!!! Refactor
        mFullscreen = false;
        mWidth = 1280;
        mHeight = 720;

        ml::make_ortho2D_mat4(mProj, 0, mWidth, mHeight, 0);

        mPrevTime = timerAbsoluteTime();
    }

    void Stage::addPrograms(size_t count, GLuint* programs)
    {
        overlayShaderEdit->addPrograms(count, programs);
    }

    void onSDLEvent(SDL_Event& event);

    void Stage::setCaption(const char* caption)
    {
        SDL_SetWindowTitle(fwk::window, caption);
    }

    void Stage::run()
    {
        while (fwk::runLoop)
        {
            profilerStartSyncPoint();
            {
                PROFILER_CPU_TIMESLICE("Frame");

                {
                    PROFILER_CPU_TIMESLICE("Frame sync");
                    gfx::beginFrame();
                }

                glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);

                uint64_t time = timerAbsoluteTime();
                float dt = (time-mPrevTime)*0.000001f;
                mPrevTime = time;

                ui::update(dt);
                onUpdate(dt);

                if (fwk::recompileGLPrograms)
                {
                    fwk::recompileGLPrograms = false;
                    onShaderRecompile();
                }

                renderActors();
                ui::render();

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