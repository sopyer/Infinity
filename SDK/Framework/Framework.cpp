#include <windows.h>
#include <cstdlib>
#include "Framework.h"

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
    static SDL_Surface* screen;

    void init(const char* argv0)
    {
        PHYSFS_init(argv0);

        PHYSFS_mount("AppData"        , 0, 1);
        PHYSFS_mount("../AppData"    , 0, 1);
        PHYSFS_mount("../../AppData", 0, 1);

        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)											// Init The SDL Library, The VIDEO Subsystem
        {
            logging::message("Unable to open SDL: %s\n", SDL_GetError() );					// If SDL Can't Be Initialized
            exit(1);															// Get Out Of Here. Sorry.
        }

        uint32_t flags = SDL_HWSURFACE|SDL_OPENGLBLIT;									// We Want A Hardware Surface And Special OpenGLBlit Mode

        bool fullscreen = false;

        if (fullscreen)		
        {
            flags |= SDL_FULLSCREEN;
        }

        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );								// In order to use SDL_OPENGLBLIT we have to
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );							// set GL attributes first
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
        SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, TRUE );							// colors and doublebuffering
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );
        SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );

        if(!(screen = SDL_SetVideoMode(1280, 720, 32, flags)))
        {
            logging::message("Unable to open screen surface: %s\n", SDL_GetError() );		// If Something's Gone Wrong, Report
            exit(1);															// And Exit
        }

        importOpenGL();

        vg::init();
        ui::init(1280, 720);
        mt::init();
    }

    void cleanup()
    {
        mt::cleanup();
        ui::cleanup();
        vg::cleanup();

        SDL_Quit();

        PHYSFS_deinit();
    }
}
