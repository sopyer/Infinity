#ifndef __FRAMEWORK_GRAPHICS_H_INCLUDED__
#   define __FRAMEWORK_GRAPHICS_H_INCLUDED__

#include <opengl.h>
#include <stdint.h>
#include <ml.h>

namespace vf
{
    struct skinned_geom_t
    {
        float   px, py, pz;
        float   nx, ny, xz;
        float   u, v;
        float   w[4];
        uint8_t b[4];

        static GLuint vao;
    };
}

namespace graphics
{
    static const GLuint64   GL_TIMEOUT_INFINITE = ~0ull;
    static const GLuint     NUM_FRAMES_DELAY = 2; //2 * number of GPUs
    static const GLsizeiptr DYNAMIC_BUFFER_FRAME_SIZE = 1 * (1<<20);

    struct gl_caps_t
    {
        GLint uboAlignment;
    };

    struct vertex_element_t
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

    GLuint createVAO(GLuint numEntries, vertex_element_t* entries, GLuint numStreams, GLuint* streamDivisors);

    bool  dynbufAlignMem(GLuint align, GLuint* offset);
    bool  dynbufAlignVert(GLsizei stride, GLuint* baseVertex);
    void* dynbufAlloc(GLsizeiptr size);
    void* dynbufAllocVert(GLsizeiptr size, GLsizei stride, GLuint* baseVertex);
    void* dynbufAllocMem(GLsizeiptr size, GLuint align, GLuint* offset);

    struct ubuffer_desc_data_t;
    typedef ubuffer_desc_data_t* ubuffer_desc_t;

    struct auto_vars_t
    {
        ml::mat4x4 matMVP;
        ml::mat4x4 matMV;
        ml::mat4x4 matP;
        ml::vec3   shCoef[10];
    };

    extern auto_vars_t autoVars;

    ubuffer_desc_t ubufCreateDesc (GLuint prg, const char* name);
    void           ubufDestroyDesc(ubuffer_desc_t desc);
    void           ubufUpdateData(ubuffer_desc_t desc, void* mem, size_t size);

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