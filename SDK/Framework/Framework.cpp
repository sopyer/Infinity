#include <SDL2/SDL.h>
#include <windows.h>

#undef min
#undef max

#include <stdio.h>
#include <stdarg.h>

#include "Framework.h"
#include "scheduler.h"
#include "graphics.h"

namespace logging
{
    void message(const char *s,...)
    {
        char str[4096];
        DWORD written=0;
        va_list vl;
        va_start(vl, s);
        vsnprintf(str, 4096, s, vl);
        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str, strlen(str), &written, 0);
        OutputDebugStringA(str);
        va_end(vl);
    }
}

namespace fwk
{
    SDL_Window*      window;
    SDL_GLContext    context;

    uint64_t prevTime;
    bool     runLoop;
    bool     recompileGLPrograms;

    void init(const char* argv0)
    {
        PHYSFS_init(argv0);

        PHYSFS_mount("AppData"        , 0, 1);
        PHYSFS_mount("../AppData"    , 0, 1);
        PHYSFS_mount("../../AppData", 0, 1);

        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)
        {
            logging::message("Unable to open SDL: %s\n", SDL_GetError() );
            ::exit(1);
        }

        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
        SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, TRUE );
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 4 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );

        window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if(!window)
        {
            logging::message("Unable to create window: %s\n", SDL_GetError() );
            ::exit(1);
        }

        context = SDL_GL_CreateContext(window);

        importOpenGL();

        ut::init();
        mt::init(1, 128);
        vg::init();
        ui::init(1280, 720);
        gfx::init(1280, 720);
        app::init();
        app::resize(1280, 720);

        prevTime = timerAbsoluteTime();
        runLoop  = true;
        recompileGLPrograms = false;
    }

    void run()
    {
        while (runLoop)
        {
            profilerStartSyncPoint();
            {
                PROFILER_CPU_TIMESLICE("Frame");

                {
                    PROFILER_CPU_TIMESLICE("Frame sync and gfx init");
                    gfx::beginFrame();
                }

                uint64_t time = timerAbsoluteTime();
                float dt = (time-prevTime)*0.000001f;
                prevTime = time;

                ui::update(dt);
                app::update(dt);

                if (recompileGLPrograms)
                {
                    recompileGLPrograms = false;
                    app::recompilePrograms();
                }

                {
                    PROFILER_CPU_TIMESLICE("onPaint");
                    app::render();
                }
                ui::render();

                gfx::endFrame();

                {
                    PROFILER_CPU_TIMESLICE("SDL_GL_SwapBuffers");
                    SDL_GL_SwapWindow(window);
                }
            }
            profilerStopSyncPoint();
        }
    }

    void fini()
    {
        app::fini();
        gfx::fini();
        ui::fini();
        vg::fini();
        mt::fini();
        ut::fini();

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        PHYSFS_deinit();
    }

    void exit()
    {
        runLoop = false;
    }

    void recompilePrograms()
    {
        recompileGLPrograms = true;
    }

    void setCaption(const char* caption)
    {
        SDL_SetWindowTitle(window, caption);
    }
}

int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    fwk::run();
 
    fwk::fini();

    return 0;
}
