#include <fwk/fwk.h>
#include <Remotery.h>

namespace gfx
{
    void resize(int w, int h);
}

namespace ui
{
    void resize(int w, int h);
}

namespace fwk
{
    SDL_Window*      window;
    SDL_GLContext    context;

    Remotery* rmt;

    uint64_t prevTime;
    bool     runLoop;
    bool     recompileGLPrograms;

    void notifyResize(int w, int h);

    void init(const char* argv0)
    {
        rmt_CreateGlobalInstance(&rmt);

        PHYSFS_init(argv0);

        PHYSFS_mount("AppData",       0, 1);
        PHYSFS_mount("../AppData",    0, 1);
        PHYSFS_mount("../../AppData", 0, 1);

        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)
        {
            fprintf(stderr, "Unable to open SDL: %s\n", SDL_GetError());
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
        SDL_GL_SetAttribute( SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1 );

        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

#ifdef _DEBUG
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );
#endif

        window = SDL_CreateWindow(
            "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if(!window)
        {
            fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
            ::exit(1);
        }

        context = SDL_GL_CreateContext(window);

        importOpenGL();

        core::init();
        gfx::init(1280, 720);
        ui::init(1280, 720);
        app::init();
        notifyResize(1280, 720);

        prevTime = timerAbsoluteTime();
        runLoop  = true;
        recompileGLPrograms = false;
    }

    void processWindowEvents()
    {
        SDL_Event E;

        SDL_PumpEvents();
        while (SDL_PeepEvents(&E, 1, SDL_GETEVENT, SDL_WINDOWEVENT, SDL_WINDOWEVENT) > 0)
        {
            switch (E.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                    notifyResize(E.window.data1, E.window.data2);
                    break;
            }
        }
    }

    void run()
    {
        while (runLoop)
        {
            profilerStartSyncPoint();
            {
                PROFILER_CPU_TIMESLICE("Frame");

                processWindowEvents();

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
        ui::fini();
        gfx::fini();
        core::fini();

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        PHYSFS_deinit();

        rmt_DestroyGlobalInstance(rmt);
    }

    void notifyResize(int w, int h)
    {
         ui::resize(w, h);
        gfx::resize(w, h);
        app::resize(w, h);
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

#include "CameraDirector.cpp"
#include "SpectatorCamera.cpp"
#include "ui.cpp"

#include "ProfilerOverlay.cpp"
#include "ShaderEditOverlay.cpp"
#include "ScPlatform.cpp"

extern "C" int assert_handler(const char* cond, const char* file, int line)
{
    const SDL_MessageBoxButtonData buttons[] = {
        { 0, 0, "Ignore" },
        { 0, 1, "Abort" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "Break" },
    };

    cstr1024 msg;

    cstr_printf(msg, "Assertion:(%s) failed in %s:%d", cond, file, line);

    SDL_MessageBoxData data = {
        SDL_MESSAGEBOX_INFORMATION,
        NULL, // no parent window
        "ASSERTION",
        msg,
        3,
        buttons,
        NULL // Default color scheme
    };

    int button = -1;
    int success = SDL_ShowMessageBox(&data, &button);

    switch (button)
    {
        case 0:
            return false;
        case 1:
            core::abort();
    }

    return  true;
}
