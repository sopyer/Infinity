#include <windows.h>
#include <cstdlib>
#include "Framework.h"
#include <SDL2/SDL.h>
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
        OutputDebugString(str);
        va_end(vl);
    }
}

namespace fwk
{
    SDL_Window*      window;
    SDL_GLContext    context;

    void init(const char* argv0)
    {
        PHYSFS_init(argv0);

        PHYSFS_mount("AppData"        , 0, 1);
        PHYSFS_mount("../AppData"    , 0, 1);
        PHYSFS_mount("../../AppData", 0, 1);

        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)
        {
            logging::message("Unable to open SDL: %s\n", SDL_GetError() );
            exit(1);
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
            exit(1);
        }

        context = SDL_GL_CreateContext(window);

        importOpenGL();

        ut::init();
        vg::init();
        ui::init(1280, 720);
        mt::init(1, 128);
        gfx::init();
    }

    void fini()
    {
        gfx::fini();
        mt::fini();
        ui::fini();
        vg::fini();
        ut::fini();

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        PHYSFS_deinit();
    }
}
