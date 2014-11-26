#pragma once

#include <core/core.h>
#include <opengl.h>
#include <gfx/vg.h>
#include <gfx/etlsf.h>

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

    struct p2_vertex_t
    {
        float  x, y;

        static GLuint vao;
    };

    struct p2uv2_vertex_t
    {
        float  x, y;
        float  u, v;

        static GLuint vao;
    };

    struct p2cu4_vertex_t
    {
        float    x, y;
        uint32_t c;

        static GLuint vao;
    };

    struct p2uv2cu4_vertex_t
    {
        float    x, y;
        float    u, v;
        uint32_t c;

        static GLuint vao;
    };

    struct p2uv3_vertex_t
    {
        float  x, y;
        float  u, v, w;

        static GLuint vao;
    };

    struct p3_vertex_t
    {
        float  x, y, z;

        static GLuint vao;
    };

    struct p3cu4_vertex_t
    {
        float    x, y, z;
        uint32_t c;

        static GLuint vao;
    };

    struct p3uv2cu4_vertex_t
    {
        float    x, y, z;
        float    u, v;
        uint32_t c;

        static GLuint vao;
    };

    struct empty_geom_t
    {
        static GLuint vao;
    };

    void set(p2uv2_vertex_t*    vert, float x, float y, float u, float v);
    void set(p2uv3_vertex_t*    vert, float x, float y, float u, float v, float w = 0.0f);
    void set(p2cu4_vertex_t*    vert, float x, float y, uint32_t c);
    void set(p2uv2cu4_vertex_t* vert, float x, float y, float u, float v, uint32_t c);
    void set(p3cu4_vertex_t*    vert, float x, float y, float z, uint32_t c);
    void set(p3uv2cu4_vertex_t* vert, float x, float y, float z, float u, float v, uint32_t c);
}

namespace gfx
{
    static const GLuint64   GL_TIMEOUT_INFINITE = ~0ull;
    static const GLuint     NUM_FRAMES_DELAY = 2; //2 * number of GPUs
    static const GLsizeiptr DYNAMIC_BUFFER_FRAME_SIZE = 10 * (1<<20);

    static const GLuint ATTR_POSITION      = 0;
    static const GLuint ATTR_NORMAL        = 1;
    static const GLuint ATTR_COLOR         = 2;
    static const GLuint ATTR_UV0           = 3;
    static const GLuint ATTR_UV1           = 4;
    static const GLuint ATTR_BLEND_INDICES = 5;
    static const GLuint ATTR_BLEND_WEIGHTS = 6;

    extern int width;
    extern int height;

    struct gl_caps_t
    {
        GLint uboAlignment;
        GLint ssboAlignment;
        GLint tboAlignment;
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

    struct var_desc_t
    {
        const char* name;
        GLenum      type;
        GLint       arraySize;
        GLint       offset;
    };

    enum
    {
        MATCH_SUCCESS,
        MATCH_INVALID_PARAMS,
        MATCH_INVALID_BLOCK,
        MATCH_COUNT_MISMATCH,
        MATCH_VAR_N_DESC_MISMATCH // Should be last
    };

    enum
    {
        STD_FEATURE_COLOR   = 1<<0,
        STD_FEATURE_TEXTURE = 1<<1,
        STD_FEATURE_COUNT = 2
    };

    int matchInterface(GLuint prg, const char* name, bool ubuffer, GLint numVars, var_desc_t* desc);

    extern gl_caps_t caps;

    extern GLuint dynBuffer;

    void init(int w, int h);
    void fini();

    void beginFrame();
    void endFrame();

    GLuint createVAO(GLuint numEntries, const vertex_element_t* entries, GLuint numStreams=0, GLuint* streamDivisors=0);

    bool  dynbufAlignMem(GLuint align, GLuint* offset);
    void* dynbufAlloc(GLsizeiptr size);
    void* dynbufAllocMem(GLsizeiptr size, GLuint align, GLuint* offset);

    template<typename T>
    T* frameAllocVertices(GLsizei count, GLuint* baseVertex)
    {
        GLsizei  stride = sizeof(T);
        GLuint   offset;

        if (!dynbufAlignMem(stride, &offset)) return false;

        *baseVertex = offset / stride;

        return (T*)dynbufAlloc(stride * count);
    }

    template<typename T>
    T* frameAlloc(GLuint align, GLuint* offset)
    {
        if (!dynbufAlignMem(align, offset)) return false;

        return (T*)dynbufAlloc(sizeof(T));
    }

    template<typename T>
    T* frameAllocArray(GLsizei count, GLuint align, GLuint* offset)
    {
        if (!dynbufAlignMem(align, offset)) return false;

        return (T*)dynbufAlloc(count * sizeof(T));
    }

    struct ubo_desc_data_t;
    typedef ubo_desc_data_t* ubo_desc_t;

    struct auto_vars_t
    {
        v128      matMVP[4];
        v128      matMV[4];
        v128      matP[4];
        ml::vec4  projParams;
        ml::vec3  shCoef[10];
    };

    extern auto_vars_t autoVars;

    void setStdProgram(size_t featureMask);

    void setModelViewMatrix (v128* view);
    void setProjectionMatrix(v128* proj);
    void setUIMatrices();

    void set3DStates();
    void set2DStates();

    void setStdTransforms(GLuint index = 0);
    void setMVP          (GLuint index = 0);

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
    void draw2DLineStrip(float* vertices, GLuint numVertices, uint32_t color);

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

//-----------------------   GPU memory   ------------------------------
namespace gfx
{
    struct  gpu_buffer_data_t;
    typedef gpu_buffer_data_t*  gpu_buffer_t;

    gpu_buffer_t  createBuffer   (GLsizeiptr size, GLuint flags = GL_MAP_WRITE_BIT);
    void          destroyBuffer  (gpu_buffer_t buffer);
    uint16_t      allocBufferMem (gpu_buffer_t buffer, GLsizeiptr size, GLuint align=0);
    void          freeBufferMem  (gpu_buffer_t buffer, uint16_t alloc);
    void*         lockBufferMem  (gpu_buffer_t buffer, uint16_t alloc);
    void          unlockBufferMem(gpu_buffer_t buffer);

    GLuint getBufferMemOffset  (gpu_buffer_t buffer, uint16_t alloc);
    GLuint getBufferMemSize    (gpu_buffer_t buffer, uint16_t alloc);
    GLuint getBufferFirstVertex(gpu_buffer_t buffer, uint16_t alloc, GLuint stride);

    GLuint getGPUBuffer(gpu_buffer_t buffer);

    template<typename T>
    T* lockBufferMem(gpu_buffer_t buffer, uint16_t alloc)
    {
        return (T*)lockBufferMem(buffer, alloc);
    }

    template<typename T>
    T* lockBufferVertices(gpu_buffer_t buffer, uint16_t alloc)
    {
        GLuint align  = sizeof(T);
        GLuint offset = getBufferMemOffset(buffer, alloc);
        GLuint rem    = offset % align;
        GLuint offsetAdjust = (rem==0)? 0 : (align - rem);

        uint8_t* mem = (uint8_t*)lockBufferMem(buffer, alloc);

        return (T*)(mem+offsetAdjust);
    }
}
