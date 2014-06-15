#ifndef __FRAMEWORK_GRAPHICS_H_INCLUDED__
#   define __FRAMEWORK_GRAPHICS_H_INCLUDED__

#include <opengl.h>

namespace graphics
{
    static const GLuint64   GL_TIMEOUT_INFINITE = ~0ull;
    static const GLuint     NUM_FRAMES_DELAY = 2; //2 * number of GPUs
    static const GLsizeiptr DYNAMIC_BUFFER_FRAME_SIZE = 1 * (1<<20);

    struct gl_caps_t
    {
        GLint uboAlignment;
    };

    extern gl_caps_t caps;

    extern GLuint dynBuffer;
    extern GLint  dynBufferOffset;

    void init();
    void fini();

    void beginFrame();
    void endFrame();

    void* allocDynVerts(GLsizeiptr size, GLsizei stride, GLuint* baseVertex);
    void* allocDynMem(GLsizeiptr size, GLuint align = 0, GLuint* offset = 0);
}

#endif