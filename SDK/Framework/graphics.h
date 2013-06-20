#ifndef __FRAMEWORK_GRAPHICS_H_INCLUDED__
#   define __FRAMEWORK_GRAPHICS_H_INCLUDED__

#include <opengl.h>

namespace graphics
{
    static const GLuint64 GL_TIMEOUT_INFINITE = ~0;

    void init();
    void fini();

    void beginFrame();
    void endFrame();
}

#endif