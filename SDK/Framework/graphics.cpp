#include "graphics.h"
#include "framework.h"
#include <utils.h>

namespace vf
{
    GLuint skinned_geom_t::vao;

    static const graphics::vertex_element_t descSkinnedGeom[5] = 
    {
        {0, offsetof(vf::skinned_geom_t, px), 0, GL_FLOAT,         3, GL_FALSE, GL_FALSE},
        {0, offsetof(vf::skinned_geom_t, nx), 1, GL_FLOAT,         3, GL_FALSE, GL_FALSE},
        {0, offsetof(vf::skinned_geom_t, u),  2, GL_FLOAT,         2, GL_FALSE, GL_FALSE},
        {0, offsetof(vf::skinned_geom_t, b),  3, GL_UNSIGNED_BYTE, 4, GL_TRUE,  GL_FALSE},
        {0, offsetof(vf::skinned_geom_t, w),  4, GL_FLOAT,         4, GL_FALSE, GL_FALSE},
    };
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
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,        &caps.uboAlignment);
        glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &caps.ssboAlignment);

        glDebugMessageCallback( glDebugCallback, NULL );

        vf::skinned_geom_t::vao = createVAO(5, vf::descSkinnedGeom, 0, NULL);

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

    GLuint createVAO(GLuint numEntries, const vertex_element_t* entries, GLuint numStreams, GLuint* streamDivisors)
    {
        GLuint vao;

        glGenVertexArrays(1, &vao);
        for (GLuint i = 0; i < numEntries; ++i)
        {
            const vertex_element_t& e = entries[i];
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

    auto_vars_t autoVars;

    struct auto_var_desc_t
    {
        const char* name;
        GLenum      type;
        GLuint      arraySize;
        GLint       offset;
    };

    static const size_t AUTO_VARS_COUNT = 4;

    auto_var_desc_t autoVarDesc[AUTO_VARS_COUNT] = {
        { "u_Proj",   GL_FLOAT_VEC4,  1, offsetof(auto_vars_t, projParams) },
        { "u_MV",     GL_FLOAT_MAT4,  1, offsetof(auto_vars_t, matMV)      },
        { "u_MVP",    GL_FLOAT_MAT4,  1, offsetof(auto_vars_t, matMVP)     },
        { "u_SHcoef", GL_FLOAT_VEC3, 10, offsetof(auto_vars_t, shCoef)     }
    };

    GLuint auto_var_get_index(const char* uname, GLenum type, GLint arraySize)
    {
        for (size_t i = 0; i < AUTO_VARS_COUNT; ++i)
        {
            if (autoVarDesc[i].type != type) continue;
            if (arraySize > 1 && autoVarDesc[i].arraySize != arraySize) continue;
            if (strncmp(autoVarDesc[i].name, uname, strlen(autoVarDesc[i].name)) != 0) continue;

            return i;
        }

        return GL_INVALID_INDEX;
    }

    struct ubuffer_desc_data_t
    {
        GLint numVars;
        GLint bufferSize;

        struct mapping_t
        {
            GLuint index;
            GLint  offset;
            GLint  arrayStride;
        } mappings[1];
    };

    static size_t ubufCalcSize(size_t numMappings)
    {
        return sizeof(ubuffer_desc_data_t) + (numMappings - 1) * sizeof(ubuffer_desc_data_t::mapping_t);
    }

    ubuffer_desc_t ubufCreateDesc(GLuint prg, const char* name)
    {
        stack_mem_t stalloc = ut::get_thread_data_stack();

        GLint numBlocks = 0;

        glGetProgramInterfaceiv(prg, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks );

        struct block_props_t
        {
            GLint numVars;
            GLint bufferSize;
        };
        static const GLenum reqBlockProps[2] = {GL_NUM_ACTIVE_VARIABLES, GL_BUFFER_DATA_SIZE};

        static const GLenum reqBlockUnis[1]  = {GL_ACTIVE_VARIABLES};

        struct uni_props_t
        {
            GLint nameLen;
            GLint type;
            GLint arraySize;
            GLint arrayStride;
            GLint offset;
        };
        static const GLenum reqUniProps[5]   = {GL_NAME_LENGTH, GL_TYPE, GL_ARRAY_SIZE, GL_ARRAY_STRIDE, GL_OFFSET};

        GLuint block = glGetProgramResourceIndex(prg, GL_UNIFORM_BLOCK, name);

        if (block == GL_INVALID_INDEX) return 0;

        block_props_t blockProps;
        glGetProgramResourceiv(prg, GL_UNIFORM_BLOCK, block, 2, reqBlockProps, 2, NULL, (GLint*)&blockProps);

        if (!blockProps.numVars) return 0;

        GLint* uniIndices = stack_mem_alloc<GLint>(stalloc, blockProps.numVars);
        glGetProgramResourceiv(prg, GL_UNIFORM_BLOCK, block, 1, reqBlockUnis, blockProps.numVars, NULL, uniIndices);

        ubuffer_desc_data_t* descProto = (ubuffer_desc_data_t*)stack_mem_alloc(stalloc, ubufCalcSize(blockProps.numVars));

        descProto->numVars = 0;
        descProto->bufferSize = blockProps.bufferSize;

        for(int uni = 0; uni < blockProps.numVars; ++uni)
        {
            uni_props_t uniProps;

            glGetProgramResourceiv(prg, GL_UNIFORM, uniIndices[uni], 5, reqUniProps, 5, NULL, (GLint*)&uniProps);

            char* uname = stack_mem_alloc<char>(stalloc, uniProps.nameLen);
            glGetProgramResourceName(prg, GL_UNIFORM, uniIndices[uni], uniProps.nameLen, NULL, uname);

            GLuint index = auto_var_get_index(uname, uniProps.type, uniProps.arraySize);
            if (index != GL_INVALID_INDEX)
            {
                size_t i = descProto->numVars;

                descProto->mappings[i].index       = index;
                descProto->mappings[i].offset      = uniProps.offset;
                descProto->mappings[i].arrayStride = uniProps.arrayStride;
                ++descProto->numVars;
            }

            stack_mem_reset(stalloc, uname);
        }

        size_t descSize = ubufCalcSize(descProto->numVars);

        ubuffer_desc_data_t* desc = (ubuffer_desc_data_t*)malloc(descSize);

        memcpy(desc, descProto, descSize);

        stack_mem_reset(stalloc, uniIndices);

        return desc;
    }

    void ubufDestroyDesc(ubuffer_desc_t desc)
    {
        free(desc);
    }

    size_t gl_type_size(GLenum type)
    {
        switch (type)
        {
            case GL_FLOAT:
                return sizeof(float);
            case GL_FLOAT_VEC2:
                return 2 * sizeof(float);
            case GL_FLOAT_VEC3:
                return 3 * sizeof(float);
            case GL_FLOAT_VEC4:
                return 4 * sizeof(float);
            case GL_FLOAT_MAT4:
                return 4 * 4 * sizeof(float);
            default:
                assert(0);
        }
        return 0;
    }

    void ubufUpdateData(ubuffer_desc_t desc, void* mem, size_t size)
    {
        assert(desc->bufferSize <= size);

        uint8_t* dst = (uint8_t*)mem;
        uint8_t* src = (uint8_t*)&autoVars;

        for (GLint vi = 0; vi < desc->numVars; ++vi)
        {
            ubuffer_desc_data_t::mapping_t& m   = desc->mappings[vi];
            auto_var_desc_t&                var = autoVarDesc[m.index];
            size_t                          sz  = gl_type_size(var.type);
            for (GLuint i = 0; i < var.arraySize; ++i)
            {
                memcpy(&dst[m.offset + m.arrayStride*i], &src[var.offset + sz*i], sz);
            }
        }
    }

}
