#include <assert.h>
#include <stdint.h>
#include "graphics.h"

namespace graphics
{
    static GLsync     frameSync[NUM_FRAMES_DELAY] = {0, 0};
    static int        frameID = 0;

    GLuint dynBuffer;
    GLint  dynBufferOffset;

    static uint8_t*   dynBufBasePtr;
    static GLsizeiptr dynBufAllocated;

    gl_caps_t caps;

    void init()
    {
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &caps.uboAlignment);

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
        assert(frameID > 0);
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
        assert(frameID > 0);
        assert(frameID < NUM_FRAMES_DELAY);

        frameSync[frameID] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        
        frameID = (frameID + 1) % NUM_FRAMES_DELAY;
    }

    GLuint createVAO(GLuint numEntries, VertexElement* entries, GLuint numStreams, GLuint* streamDivisors)
    {
        GLuint vao;

        glGenVertexArrays(1, &vao);
        for (GLuint i = 0; i < numEntries; ++i)
        {
            VertexElement& e = entries[i];
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

    //TODO: implement guard logic in order to debug memory overwrites!!!!!
    void* allocDynVerts(GLsizeiptr size, GLsizei stride, GLuint* baseVertex)
    {
        void* verts;

        if (dynBufAllocated + size + stride > DYNAMIC_BUFFER_FRAME_SIZE)
            return 0;

        GLsizei rem = dynBufAllocated % stride;

        dynBufAllocated += (rem == 0 ? 0 : stride - rem);
        assert(dynBufAllocated % stride == 0);

        verts = dynBufBasePtr + dynBufferOffset + dynBufAllocated;
        if (baseVertex) *baseVertex = dynBufAllocated / stride;

        dynBufAllocated += size;

        return verts;
    }

    void* allocDynMem(GLsizeiptr size, GLuint align, GLuint* offset)
    {
        void* mem;

        if (dynBufAllocated + size + align > DYNAMIC_BUFFER_FRAME_SIZE)
            return 0;

        if (align)
        {
            GLsizei rem = dynBufAllocated % align;

            dynBufAllocated += (rem == 0 ? 0 : align - rem);
            assert(dynBufAllocated % align == 0);
        }

        mem = dynBufBasePtr + dynBufferOffset + dynBufAllocated;
        if (offset) *offset = dynBufAllocated;

        dynBufAllocated += size;

        return mem;
    }
}
