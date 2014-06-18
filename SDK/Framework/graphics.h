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

    struct VertexElement
    {
        GLuint     stream;
        GLuint     offset;
        GLuint     attrib;
        GLenum     type       : 16;
        GLint      size       : 4;
        GLboolean  integer    : 1;
        GLboolean  normalized : 1;
    };

    extern gl_caps_t caps;

    extern GLuint dynBuffer;

    void init();
    void fini();

    void beginFrame();
    void endFrame();

    GLuint createVAO(GLuint numEntries, VertexElement* entries, GLuint numStreams, GLuint* streamDivisors);

    bool  dynbufAlignMem(GLuint align, GLuint* offset);
    bool  dynbufAlignVert(GLsizei stride, GLuint* baseVertex);
    void* dynbufAlloc(GLsizeiptr size);
    void* dynbufAllocVert(GLsizeiptr size, GLsizei stride, GLuint* baseVertex);
    void* dynbufAllocMem(GLsizeiptr size, GLuint align, GLuint* offset);

/*------------- Immediate mode implementation --------------*/
#define IM_BEGIN(VertexType, primitiveType)                               \
    {                                                                     \
        GLuint      primType = primitiveType;                             \
        GLuint      numVertices = 0;                                      \
        GLuint      baseVertex;                                           \
        GLuint      vertexSize = sizeof(VertexType);                      \
        VertexType  v = {0};                                              \
        dynBufAlignVert(vertexSize, &baseVertex)                          \


#define IM_ATTRIB(field, value)                                           \
        v.(field) = (value);                                              \


#define IM_ATTRIB_ISSUE(field, value)                                     \
        v.(field) = (value);                                              \
        VertexType* pv = (VertexType*)graphics::dynbufAlloc(vertexSize);  \
        *pv = v;                                                          \
        ++numVertices;                                                    \


#define IM_END()                                                          \
        glDrawArrays(primType, baseVertex, numVertices);                  \
    }                                                                     \


/*------------- Miscellaneous --------------*/
#define GL_CHECK_ERROR()                              \
    {                                                 \
        GLenum err = glGetError();                    \
        assert(err==GL_NO_ERROR);                     \
    }

}

#endif