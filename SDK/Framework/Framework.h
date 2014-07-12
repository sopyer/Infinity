#ifndef _FRAMEWORK_H_INCLUDED_
#   define _FRAMEWORK_H_INCLUDED_

#include <SDL2/SDL.h>
#include <opengl.h>

#include <ui.h>
#include <ResourceHelpers.h>
#include <profiler.h>
#include <physfs/physfs.h>
#include <Timer.h>

//TODO: make shorter e.g. log
namespace logging
{
    void message(const char *s,...);
}

namespace app
{
    void init();
    void fini();
    void update(float dt);
    void render();
    void recompilePrograms();
    void resize(int width, int height);
}

namespace fwk
{
    extern SDL_Window*      window;
    extern SDL_GLContext    context;

    void init(const char* argv0);
    void fini();

    void run();

    void setCaption(const char* caption);
    void recompilePrograms();
    void exit();
}

#endif
