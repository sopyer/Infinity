#pragma once

#include <core/core.h>
#include <opengl.h>
#include <gfx/vg.h>

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

    struct static_geom_t
    {
        float   px, py, pz;
        float   nx, ny, xz;
        float   u, v;

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

    extern int width;
    extern int height;

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
        GLuint     type       : 16;
        GLuint     size       : 4;
        GLuint     integer    : 1;
        GLuint     normalized : 1;
    };

    extern gl_caps_t caps;

    extern GLuint dynBuffer;

    void init(int w, int h);
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

    void setStdTransforms(GLuint index = 0);

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
    void drawXZGrid(float x0, float z0, float x1, float z1, int numQuads, v128 color);

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

namespace res
{
    //Up to 15 headers, others are ignored
    GLuint createShaderFromFile  (GLenum shaderType, const char* path, size_t numHdr=0, const char** headers=0);
    GLuint createProgramFromFiles(const char* vshPath, const char* fshPath, size_t numHdr=0, const char** headers=0);
    GLuint createProgramFromFiles(const char* vshPath, const char* gshPath, const char* fshPath, size_t numHdr=0, const char** headers=0);
    GLuint createProgramFromFiles(size_t numHdr, const char** headers, size_t numSrc, ...);

    bool    compileAndAttachShader(GLuint program, GLenum type, GLsizei len, const char* source);
    GLuint  createProgram(GLenum type, const char* source);
    GLuint  linkProgram(GLsizei shaderCount, ...);
    void    getUniforms(GLuint program, GLsizei uniformCount, const char** uniformNames, GLint* uniforms);

    GLuint createTexture2D(const char* name,
        GLint  forceSRGB=false,
        GLint  minFilter=GL_LINEAR_MIPMAP_LINEAR,
        GLint  magFilter=GL_LINEAR,
        GLint  genMipmap=GL_TRUE,
        GLint* width = 0,
        GLint* height = 0);

    GLuint createSpecialTexture1D(GLint internalformat, GLsizei width,
        GLenum format, GLenum type, const GLvoid *pixels);

    GLuint createRenderbuffer(GLenum type, GLsizei width, GLsizei height);

    void saveScreenToFile(const char* imageName);
}
