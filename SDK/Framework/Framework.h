#ifndef _FRAMEWORK_H_INCLUDED_
#	define _FRAMEWORK_H_INCLUDED_

#include <opengl.h>

#include <UI.h>
#include <ResourceHelpers.h>
#include <Context.h>
#include <profiler.h>
#include <physfs/physfs.h>

//TODO: make shorter e.g. log
namespace logging
{
	void message(const char *s,...);
}

#define CHECK_GL_ERROR()                \
    {                                   \
        GLenum err = glGetError();      \
        assert(err==GL_NO_ERROR);       \
    }

#endif