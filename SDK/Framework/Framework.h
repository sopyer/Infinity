#ifndef _FRAMEWORK_H_INCLUDED_
#	define _FRAMEWORK_H_INCLUDED_

#include <SDL2/SDL.h>
#include <opengl.h>

#include <ui.h>
#include <ResourceHelpers.h>
#include <profiler.h>
#include <physfs/physfs.h>
#include <Stage.h>
#include <Timer.h>

//TODO: make shorter e.g. log
namespace logging
{
	void message(const char *s,...);
}

namespace fwk
{
    extern SDL_Window*      window;
    extern SDL_GLContext    context;

    void init(const char* argv0);
    void cleanup();
}

#define CHECK_GL_ERROR()                \
    {                                   \
        GLenum err = glGetError();      \
        assert(err==GL_NO_ERROR);       \
    }

#endif