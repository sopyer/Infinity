#ifndef __FRAMEWORK_GRAPHICS_H_INCLUDED__
#   define __FRAMEWORK_GRAPHICS_H_INCLUDED__

#include <opengl.h>

namespace graphics
{
    static const GLuint64 GL_TIMEOUT_INFINITE = ~0;
    static const GLuint   NUM_FRAMES_DELAY = 2; //2 * number of GPUs

    void init();
    void fini();

    void beginFrame();
    void endFrame();
}

#endif