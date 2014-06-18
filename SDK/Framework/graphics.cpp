#include <assert.h>
#include <stdint.h>
#include "graphics.h"
#include "framework.h"
#include <utils.h>

namespace vf
{
    GLuint skinned_geom_t::vao;
}

namespace graphics
{
    static GLsync     frameSync[NUM_FRAMES_DELAY] = {0, 0};
    static int        frameID = 0;

    GLuint dynBuffer;
    GLint  dynBufferOffset;

    static uint8_t*   dynBufBasePtr;
    static GLsizeiptr dynBufAllocated;

    gl_caps_t caps;

    void APIENTRY glDebugCallback(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length,
        const GLchar *message, const void *userParam
    )
    {
        logging::message("Message : %s\n", message);
    }

    void init()
    {
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &caps.uboAlignment);

        glDebugMessageCallback( glDebugCallback, NULL );

        vertex_element_t descSkinnedGeom[5] = 
        {
            {0, offsetof(vf::skinned_geom_t, px), 0, GL_FLOAT,         3, GL_FALSE, GL_FALSE},
            {0, offsetof(vf::skinned_geom_t, nx), 1, GL_FLOAT,         3, GL_FALSE, GL_FALSE},
            {0, offsetof(vf::skinned_geom_t, u),  2, GL_FLOAT,         2, GL_FALSE, GL_FALSE},
            {0, offsetof(vf::skinned_geom_t, b),  3, GL_UNSIGNED_BYTE, 4, GL_TRUE,  GL_FALSE},
            {0, offsetof(vf::skinned_geom_t, w),  4, GL_FLOAT,         4, GL_FALSE, GL_FALSE},
        };

        vf::skinned_geom_t::vao = createVAO(5, descSkinnedGeom, 0, NULL);

        GLsizeiptr size  = DYNAMIC_BUFFER_FRAME_SIZE * NUM_FRAMES_DELAY;
        GLbitfield flags = GL_MAP_WRITE_BIT|GL_MAP_PERSISTENT_BIT|GL_MAP_COHERENT_BIT;

        glGenBuffers(1, &dynBuffer);
        glNamedBufferStorageEXT(dynBuffer, size, 0, flags);
        dynBufBasePtr = (uint8_t*)glMapNamedBufferRangeEXT(dynBuffer, 0, size, flags);
    }

    void fini()
    {
        glDeleteSync(frameSync[0]);
        glDeleteSync(frameSync[1]);
    }

    void beginFrame()
    {
        assert(frameID >= 0);
        assert(frameID < NUM_FRAMES_DELAY);

        if (frameSync[frameID])
        {
            GLenum result = glClientWaitSync(frameSync[frameID], GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_INFINITE);
            
            assert(result==GL_ALREADY_SIGNALED || result==GL_CONDITION_SATISFIED);
            
            glDeleteSync(frameSync[frameID]);
            frameSync[frameID] = 0;
        }

        dynBufferOffset = frameID * DYNAMIC_BUFFER_FRAME_SIZE;
        dynBufAllocated = 0;
    }

    void endFrame()
    {
        assert(frameID >= 0);
        assert(frameID < NUM_FRAMES_DELAY);

        frameSync[frameID] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        
        frameID = (frameID + 1) % NUM_FRAMES_DELAY;
    }

    GLuint createVAO(GLuint numEntries, vertex_element_t* entries, GLuint numStreams, GLuint* streamDivisors)
    {
        GLuint vao;

        glGenVertexArrays(1, &vao);
        for (GLuint i = 0; i < numEntries; ++i)
        {
            vertex_element_t& e = entries[i];
            if (e.integer)
            {
                glVertexArrayVertexAttribIFormatEXT (vao, e.attrib, e.size, e.type, e.offset);
            }
            else
            {
                glVertexArrayVertexAttribFormatEXT  (vao, e.attrib, e.size, e.type, e.normalized, e.offset);
            }
            glVertexArrayVertexAttribBindingEXT (vao, e.attrib, e.stream);
            glEnableVertexArrayAttribEXT(vao, e.attrib);
        }

        for (GLuint i = 0; i < numStreams; ++i)
        {
            glVertexArrayVertexBindingDivisorEXT(vao, i, streamDivisors ? streamDivisors[i] : 0);
        }

        return vao;
    }

    bool dynbufAlignMem(GLuint align, GLuint* offset)
    {
        if (align != 0)
        {
            GLsizei rem  = dynBufAllocated % align;
            GLsizei aoff = rem == 0 ? 0 : align - rem;

            if (dynBufAllocated + aoff > DYNAMIC_BUFFER_FRAME_SIZE)
                return false;

            dynBufAllocated += aoff;
            assert(dynBufAllocated % align == 0);
        }

        if (offset) *offset = dynBufferOffset + dynBufAllocated;

        return true;
    }

    bool  dynbufAlignVert(GLsizei stride, GLuint* baseVertex)
    {
        GLuint offset;

        if (!dynbufAlignMem(stride, &offset)) return false;

        *baseVertex = offset / stride;

        return true;
    }

    void* dynbufAlloc(GLsizeiptr size)
    {
        void* mem;

        if (dynBufAllocated + size > DYNAMIC_BUFFER_FRAME_SIZE)
            return 0;

        mem = dynBufBasePtr + dynBufferOffset + dynBufAllocated;
        dynBufAllocated += size;

        return mem;
    }

    void* dynbufAllocVert(GLsizeiptr size, GLsizei stride, GLuint* baseVertex)
    {
        if (!dynbufAlignVert(stride, baseVertex))
            return 0;

        return dynbufAlloc(size);
    }

    void* dynbufAllocMem(GLsizeiptr size, GLuint align, GLuint* offset)
    {
        if (!dynbufAlignMem(align, offset))
            return 0;

        return dynbufAlloc(size);
    }
}
