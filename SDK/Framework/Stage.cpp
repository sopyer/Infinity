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

        mPrevTime = timerAbsoluteTime();
    }

    void Stage::run()
    {
        while (fwk::runLoop)
        {
            profilerStartSyncPoint();
            {
                PROFILER_CPU_TIMESLICE("Frame");

                {
                    PROFILER_CPU_TIMESLICE("Frame sync and gfx init");
                    gfx::beginFrame();
                }

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

                {
                    PROFILER_CPU_TIMESLICE("onPaint");
                    onPaint();
                }
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
}