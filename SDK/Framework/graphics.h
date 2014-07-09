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

    struct empty_geom_t
    {
        static GLuint vao;
    };
}

namespace gfx
{
    static const GLuint64   GL_TIMEOUT_INFINITE = ~0ull;
    static const GLuint     NUM_FRAMES_DELAY = 2; //2 * number of GPUs
    static const GLsizeiptr DYNAMIC_BUFFER_FRAME_SIZE = 1 * (1<<20);

    struct gl_caps_t
    {
        GLint uboAlignment;
        GLint ssboAlignment;
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

    GLuint createVAO(GLuint numEntries, const vertex_element_t* entries, GLuint numStreams, GLuint* streamDivisors);

    bool  dynbufAlignMem(GLuint align, GLuint* offset);
    bool  dynbufAlignVert(GLsizei stride, GLuint* baseVertex);
    void* dynbufAlloc(GLsizeiptr size);
    void* dynbufAllocVert(GLsizeiptr size, GLsizei stride, GLuint* baseVertex);
    void* dynbufAllocMem(GLsizeiptr size, GLuint align, GLuint* offset);

    struct ubo_desc_data_t;
    typedef ubo_desc_data_t* ubo_desc_t;

    struct auto_vars_t
    {
        ml::mat4x4 matMVP;
        ml::mat4x4 matMV;
        ml::mat4x4 matP;
        ml::vec4   projParams;
        ml::vec3   shCoef[10];
    };

    extern auto_vars_t autoVars;

    ubo_desc_t createUBODesc (GLuint prg, const char* name);
    void       destroyUBODesc(ubo_desc_t desc);
    void       updateUBO     (ubo_desc_t desc, void* mem, size_t size);

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

    struct point_t
    {
        v128 p;
    };

    struct line_t
    {
        v128 p0, p1;
    };

    void drawLines(v128 color, GLsizei count, GLuint buffer, GLintptr offset, GLsizeiptr size);
    void drawPoints(float ptsize, v128 color, GLsizei count, GLuint buffer, GLintptr offset, GLsizeiptr size);

    struct gpu_timer_t
    {
        GLuint   queries[NUM_FRAMES_DELAY];
        uint64_t measuredTime;
    };

    void     gpu_timer_init    (gpu_timer_t* timer);
    void     gpu_timer_fini    (gpu_timer_t* timer);
    void     gpu_timer_start   (gpu_timer_t* timer);
    void     gpu_timer_stop    (gpu_timer_t* timer);
    uint64_t gpu_timer_measured(gpu_timer_t* timer);
}

#endif