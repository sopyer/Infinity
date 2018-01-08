#include <gfx/gfx.h>

#include "gfx_res.h"

namespace gfx
{
    static GLsync     frameSync[NUM_FRAMES_DELAY] = {0, 0};
    static int        frameID = 0;

    int width;
    int height;

    GLuint dynBuffer;
    GLint  dynBufferOffset;

    static uint8_t*   dynBufBasePtr;
    static GLsizeiptr dynBufAllocated;

    v128 uiProj[4];

    gl_caps_t caps;

    mspace_t memArena = 0;

//!!!!!TODO: implements proper simple caching solution with bitset
    //enum RevObjects
    //{
    //    REVOBJ_STD_TRANSFORMS,
    //    REVOBJ_MVP,
    //    NUM_REVISIONED_OBJECTS
    //};

    //core::bitset<NUM_REVISIONED_OBJECTS> resSubmitStatus;
    //bitset_init(bs)
    //bitset_set(bs, int, int);
    //int bitset_is_set(bs, int)

    static const char* glString(GLenum value)
    {
        switch (value)
        {
            case GL_DEBUG_SOURCE_API:
                return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                return "WINSYS";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                return "SHADERCOMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                return "3RDPARTY";
            case GL_DEBUG_SOURCE_APPLICATION:
                return "APP";
            case GL_DEBUG_SOURCE_OTHER:
                return "OTHER";
            case GL_DEBUG_TYPE_ERROR:
                return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                return "DEPRECATED";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                return "UNDEFINED";
            case GL_DEBUG_TYPE_PORTABILITY:
                return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:
                return "PERFORMANCE";
            case GL_DEBUG_TYPE_OTHER:
                return "OTHER";
            case GL_DEBUG_TYPE_MARKER:
                return "MARKER";
            case GL_DEBUG_TYPE_PUSH_GROUP:
                return "PUSHGROUP";
            case GL_DEBUG_TYPE_POP_GROUP:
                return "POPGROUP";
            case GL_DEBUG_SEVERITY_HIGH:
                return "HIGH";
            case GL_DEBUG_SEVERITY_MEDIUM:
                return "MEDIUM";
            case GL_DEBUG_SEVERITY_LOW:
                return "LOW";
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                return "NOTIFY";
        }

        return "UNKNOWN";
    }

    void APIENTRY debugCallback(
        GLenum source, GLenum type,
        GLuint id, GLenum severity,
        GLsizei length, const GLchar *message,
        const void *userParam
    )
    {
        UNUSED(userParam);
        UNUSED(length);
        UNUSED(id);
        core_log(
            LOG_CAT_VIDEO, LOG_PRIO_ERROR,
            "SRC[%s] TYPE[%s] SEV[%s] : %s\n",
            glString(source), glString(type),
            glString(severity), message
        );
    }

    void init(int w, int h)
    {
        memArena = mem_create_space(512 * (1<<10));

#ifdef _DEBUG
        glDebugMessageCallback(debugCallback, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

        width  = w;
        height = h;

        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,        &caps.uboAlignment);
        glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &caps.ssboAlignment);
        glGetIntegerv(GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT,        &caps.tboAlignment);

        GLsizeiptr size  = DYNAMIC_BUFFER_FRAME_SIZE * NUM_FRAMES_DELAY;
        GLbitfield flags = GL_MAP_WRITE_BIT|GL_MAP_PERSISTENT_BIT|GL_MAP_COHERENT_BIT;

        glCreateBuffers(1, &dynBuffer);
        glNamedBufferStorage(dynBuffer, size, 0, flags);
        dynBufBasePtr = (uint8_t*)glMapNamedBufferRange(dynBuffer, 0, size, flags);

        vg::init();

        gfx_res::init();
    }

    void fini()
    {
        gfx_res::fini();

        vg::fini();

        glDeleteSync(frameSync[0]);
        glDeleteSync(frameSync[1]);

        mem_destroy_space(memArena);
    }

    void resize(int w, int h)
    {
        width  = w;
        height = h;

        ml::make_ortho2D_mat4(uiProj, 0, (float)width, (float)height, 0);
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

        ml::make_identity_mat4(autoVars.matMV);
        ml::make_identity_mat4(autoVars.matP);
        ml::make_identity_mat4(autoVars.matMVP);

        glViewport(0, 0, width, height);

        glUseProgram(0);
        glClearColor   (0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepthf  (1.0f);
        glClearStencil (0x00);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
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

        glCreateVertexArrays(1, &vao);
        for (GLuint i = 0; i < numEntries; ++i)
        {
            const vertex_element_t& e = entries[i];
            if (e.integer)
            {
                glVertexArrayAttribIFormat(vao, e.attrib, e.size, e.type, e.offset);
            }
            else
            {
                glVertexArrayAttribFormat(vao, e.attrib, e.size, e.type, e.normalized, e.offset);
            }
            glVertexArrayAttribBinding(vao, e.attrib, e.stream);
            glEnableVertexArrayAttrib(vao, e.attrib);
        }

        for (GLuint i = 0; i < numStreams; ++i)
        {
            glVertexArrayBindingDivisor(vao, i, streamDivisors ? streamDivisors[i] : 0);
        }

        return vao;
    }

    bool dynbufAlignMem(GLuint align, GLuint* offset)
    {
        if (align != 0)
        {
            GLsizei rem  = (dynBufferOffset + dynBufAllocated) % align;
            GLsizei aoff = rem == 0 ? 0 : align - rem;

            if (dynBufAllocated + aoff > DYNAMIC_BUFFER_FRAME_SIZE)
                return false;

            dynBufAllocated += aoff;
            assert((dynBufferOffset + dynBufAllocated) % align == 0);
        }

        if (offset) *offset = dynBufferOffset + dynBufAllocated;

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

    void* dynbufAllocMem(GLsizeiptr size, GLuint align, GLuint* offset)
    {
        if (!dynbufAlignMem(align, offset))
            return 0;

        return dynbufAlloc(size);
    }

    struct line_global_t
    {
        v128  uMVP[4];
        v128  uColor;
        v128  uData;
    };

    //!!TODO: fix near plane clipping bug
    void drawLines(v128 color, GLsizei count, GLuint buffer, GLintptr offset, GLsizeiptr size)
    {
        GLuint offsetGlobal;
        GLsizeiptr     sizeGlobal = sizeof(line_global_t);
        line_global_t* global     = (line_global_t*)dynbufAllocMem(sizeGlobal, caps.uboAlignment, &offsetGlobal);

        const float w = 2.0f;
        global->uColor = color;
        global->uData = vi_set(
            w / (float)width,
            w / (float)height,
            width / (float)height,
            0.0);
        mem_copy(&global->uMVP, &autoVars.matMVP, sizeof(ml::mat4x4));

        assert(sizeof(line_t)*count <= (size_t)size);
        assert(offset % caps.ssboAlignment == 0);

        glUseProgram(gfx_res::prgLine);

        glBindBufferRange(GL_UNIFORM_BUFFER,        0, dynBuffer, offsetGlobal, sizeGlobal);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, buffer,    offset,       size);

        glBindVertexArray(vf::empty_geom_t::vao);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
    }

    void drawPoints(float ptsize, v128 color, GLsizei count, GLuint buffer, GLintptr offset, GLsizeiptr size)
    {
        GLuint offsetGlobal;
        GLsizeiptr     sizeGlobal = sizeof(line_global_t);
        line_global_t* global     = (line_global_t*)dynbufAllocMem(sizeGlobal, caps.uboAlignment, &offsetGlobal);

        global->uColor = color;
        global->uData = vi_set(
            ptsize / (float)width,
            ptsize / (float)height,
            0.0f, 0.0f
        );
        mem_copy(&global->uMVP, &autoVars.matMVP, sizeof(ml::mat4x4));

        assert(sizeof(point_t)*count <= (size_t)size);
        assert(offset % caps.ssboAlignment == 0);

        glUseProgram(gfx_res::prgPoint);

        glBindBufferRange(GL_UNIFORM_BUFFER,        0, dynBuffer, offsetGlobal, sizeGlobal);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, buffer,    offset,       size);

        glBindVertexArray(vf::empty_geom_t::vao);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
    }

    void drawXZGrid(float x0, float z0, float x1, float z1, int numQuads, v128 color)
    {
        float stepX = (x1-x0) / numQuads;
        float stepZ = (z1-z0) / numQuads;
        int numLines = (numQuads + 1);

        GLuint  offset;
        GLuint  size;
        v128*   v;

        size = sizeof(gfx::line_t) * 2 * numLines;
        v    = (v128*)gfx::dynbufAllocMem(size, gfx::caps.ssboAlignment, &offset);
        for ( int i = 0; i < numLines; ++i )
        {
            *v++ = vi_set(x0, 0.0f, z0 + i * stepZ, 1.0f);
            *v++ = vi_set(x1, 0.0f, z0 + i * stepZ, 1.0f);
            *v++ = vi_set(x0 + i * stepX, 0.0f, z0, 1.0f);
            *v++ = vi_set(x0 + i * stepX, 0.0f, z1, 1.0f);
        }
        gfx::drawLines(color, 2 * numLines, gfx::dynBuffer, offset, size);
    }

    void draw2DLineStrip(float* vertices, GLuint numVertices, uint32_t color)
    {
        assert(numVertices>1);

        GLuint numLines = numVertices - 1;

        gfx::setStdProgram(gfx::STD_FEATURE_COLOR);
        gfx::setMVP();

        GLuint baseVertex;

        glBindVertexArray(vf::p2cu4_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p2cu4_vertex_t));

        vf::p2cu4_vertex_t* v = gfx::frameAllocVertices<vf::p2cu4_vertex_t>(numLines*4, &baseVertex);

        for (GLuint i = 0; i < numLines; ++i)
        {
            float x  = vertices[i*2];
            float y  = vertices[i*2+1];
            float x_ = vertices[i*2+2];
            float y_ = vertices[i*2+3];
            float nx = y  - y_;
            float ny = x_ - x;
            float scale = 10*core::max(ml::abs(nx), ml::abs(ny));

            if (scale<FLT_EPSILON) continue;

            nx /= scale;
            ny /= scale;

            vf::set(v++, x +nx, y +ny, color);
            vf::set(v++, x -nx, y -ny, color);
            vf::set(v++, x_+nx, y_+ny, color);
            vf::set(v++, x_-nx, y_-ny, color);
        }
        glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, numLines*4);
    }

    void drawRects(size_t count, float* rects, uint32_t* colors)
    {
        glUseProgram(gfx_res::prgRect);
        gfx::setMVP();

        GLuint rectOffset, colOffset;

        float*   rectDst = (float*)  gfx::dynbufAllocMem(sizeof(float)*4*count,   0, &rectOffset);
        uint8_t* colDst  = (uint8_t*)gfx::dynbufAllocMem(sizeof(uint8_t)*4*count, 0, &colOffset);

        {
            PROFILER_CPU_TIMESLICE("Data copy");
            mem_copy(rectDst, rects, sizeof(float)*4*count);
            mem_copy(colDst,  colors,   sizeof(uint8_t)*4*count);
        }

        glBindVertexArray(gfx_res::vaoRect);

        glBindVertexBuffer(0, gfx::dynBuffer, rectOffset, sizeof(float)*4);
        glBindVertexBuffer(1, gfx::dynBuffer, colOffset,  sizeof(uint8_t)*4);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);

        glBindVertexArray(0);
    }

    auto_vars_t autoVars;

    static const size_t AUTO_VARS_COUNT = 4;

    var_desc_t autoVarDesc[AUTO_VARS_COUNT] = {
        { "au_Proj",   GL_FLOAT_VEC4,  1, offsetof(auto_vars_t, projParams) },
        { "au_MV",     GL_FLOAT_MAT4,  1, offsetof(auto_vars_t, matMV)      },
        { "au_MVP",    GL_FLOAT_MAT4,  1, offsetof(auto_vars_t, matMVP)     },
        { "au_SHcoef", GL_FLOAT_VEC3, 10, offsetof(auto_vars_t, shCoef)     }
    };

    GLuint auto_var_get_index(size_t nameLen, const char* uname, GLenum type, GLint arraySize)
    {
        int ind;

        for (size_t i = 0; i < AUTO_VARS_COUNT; ++i)
        {
            assert(arraySize<2 || nameLen>3);
            if (autoVarDesc[i].type != type) continue;
            if (arraySize > 1 && autoVarDesc[i].arraySize != arraySize) continue;
            size_t len = arraySize>1 ? nameLen-3 : nameLen;
            if (cstr_comparen(uname, autoVarDesc[i].name, len, &ind)==EOK && ind != 0) continue;

            return i;
        }

        return GL_INVALID_INDEX;
    }

    struct ubo_desc_data_t
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

    ubo_desc_t createUBODesc(GLuint prg, const char* name)
    {
        stack_mem_t stalloc = core::get_thread_data_stack();

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

        ubo_desc_data_t* descProto = (ubo_desc_data_t*)stack_mem_alloc(stalloc, calcBlobSize1<ubo_desc_data_t, ubo_desc_data_t::mapping_t>(blockProps.numVars));

        descProto->numVars = 0;
        descProto->bufferSize = blockProps.bufferSize;

        for(int uni = 0; uni < blockProps.numVars; ++uni)
        {
            uni_props_t uniProps;

            glGetProgramResourceiv(prg, GL_UNIFORM, uniIndices[uni], 5, reqUniProps, 5, NULL, (GLint*)&uniProps);

            char* uname = stack_mem_alloc<char>(stalloc, uniProps.nameLen);
            glGetProgramResourceName(prg, GL_UNIFORM, uniIndices[uni], uniProps.nameLen, NULL, uname);

            GLuint index = auto_var_get_index(uniProps.nameLen-1, uname, uniProps.type, uniProps.arraySize);
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

        size_t descSize = calcBlobSize1<ubo_desc_data_t, ubo_desc_data_t::mapping_t>(descProto->numVars);

        ubo_desc_data_t* desc = (ubo_desc_data_t*)malloc(descSize);

        mem_copy(desc, descProto, descSize);

        stack_mem_reset(stalloc, uniIndices);

        return desc;
    }

    int matchInterface(GLuint prg, const char* name, bool ubuffer, GLint numVars, var_desc_t* desc)
    {
        stack_mem_t stalloc = core::get_thread_data_stack();

        struct block_props_t
        {
            GLint numVars;
            GLint bufferSize;
        };
        static const GLenum reqBlockProps[2] = {GL_NUM_ACTIVE_VARIABLES, GL_BUFFER_DATA_SIZE};

        static const GLenum reqBlockUnis[1]  = {GL_ACTIVE_VARIABLES};

        GLenum type    = ubuffer ? GL_UNIFORM_BLOCK : GL_SHADER_STORAGE_BLOCK;
        GLenum vartype = ubuffer ? GL_UNIFORM : GL_BUFFER_VARIABLE;
        struct uni_props_t
        {
            GLint  nameLen;
            GLenum type;
            GLint  arraySize;
            GLint  arrayStride;
            GLint  offset;
        };
        static const GLenum reqUniProps[5]   = {GL_NAME_LENGTH, GL_TYPE, GL_ARRAY_SIZE, GL_ARRAY_STRIDE, GL_OFFSET};

        if (numVars<=0 || !desc) return MATCH_INVALID_PARAMS;

        GLuint block = glGetProgramResourceIndex(prg, type, name);

        if (block == GL_INVALID_INDEX) return MATCH_INVALID_BLOCK;

        block_props_t blockProps;
        glGetProgramResourceiv(prg, type, block, 2, reqBlockProps, 2, NULL, (GLint*)&blockProps);

        if (blockProps.numVars != numVars) return MATCH_COUNT_MISMATCH;

        GLint* uniIndices = stack_mem_alloc<GLint>(stalloc, blockProps.numVars);
        glGetProgramResourceiv(prg, type, block, 1, reqBlockUnis, blockProps.numVars, NULL, uniIndices);

        for(int uni = 0; uni < blockProps.numVars; ++uni)
        {
            uni_props_t uniProps;

            glGetProgramResourceiv(prg, vartype, uniIndices[uni], 5, reqUniProps, 5, NULL, (GLint*)&uniProps);

            char* uname = stack_mem_alloc<char>(stalloc, uniProps.nameLen);
            glGetProgramResourceName(prg, vartype, uniIndices[uni], uniProps.nameLen, NULL, uname);

            if (desc[uni].type   != uniProps.type)   return MATCH_VAR_N_DESC_MISMATCH+uni;
            if (desc[uni].offset != uniProps.offset) return MATCH_VAR_N_DESC_MISMATCH+uni;
            if (uniProps.arraySize > 1 && desc[uni].arraySize != uniProps.arraySize) return MATCH_VAR_N_DESC_MISMATCH+uni;

            int ind;
            size_t len = uniProps.arraySize>1 ? uniProps.nameLen-3 : uniProps.nameLen;
            if (desc[uni].name && cstr_comparen(desc[uni].name, uname, len, &ind) == EOK && ind != 0)
            {
                return MATCH_VAR_N_DESC_MISMATCH+uni;
            }

            stack_mem_reset(stalloc, uname);
        }

        stack_mem_reset(stalloc, uniIndices);

        return MATCH_SUCCESS;
    }

    void destroyUBODesc(ubo_desc_t desc)
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

    void updateUBO(ubo_desc_t desc, void* mem, size_t size)
    {
        assert((size_t)desc->bufferSize <= size);

        uint8_t* dst = (uint8_t*)mem;
        uint8_t* src = (uint8_t*)&autoVars;

        for (GLint vi = 0; vi < desc->numVars; ++vi)
        {
            ubo_desc_data_t::mapping_t& m   = desc->mappings[vi];
            var_desc_t&                 var = autoVarDesc[m.index];
            size_t                      sz  = gl_type_size(var.type);
            for (GLint i = 0; i < var.arraySize; ++i)
            {
                mem_copy(&dst[m.offset + m.arrayStride*i], &src[var.offset + sz*i], sz);
            }
        }
    }

    void setStdProgram(size_t featureMask)
    {
        assert(featureMask<gfx_res::STD_PROGRAM_COUNT);
        glUseProgram(gfx_res::stdPrograms[featureMask]);
    }

    struct std_transforms_t
    {
        v128 uMV[4];
        v128 uProj;
    };

    struct mvp_t
    {
        v128 uMVP[4];
    };

    void setStdTransforms(GLuint index)
    {
        GLuint     offset;
        GLsizeiptr size = sizeof(std_transforms_t);

        std_transforms_t* transforms = (std_transforms_t*)gfx::dynbufAllocMem(size, gfx::caps.uboAlignment, &offset);

        mem_copy(transforms->uMV, &autoVars.matMV, 4*sizeof(v128));
        transforms->uProj = vi_loadu_v4(&autoVars.projParams);

        glBindBufferRange(GL_UNIFORM_BUFFER, index, dynBuffer, offset, size);
    }

    void setMVP(GLuint index)
    {
        GLuint     offset;
        GLsizeiptr size = sizeof(mvp_t);

        mvp_t* transforms = (mvp_t*)gfx::dynbufAllocMem(size, gfx::caps.uboAlignment, &offset);

        mem_copy(transforms->uMVP, &autoVars.matMVP, 4*sizeof(v128));

        glBindBufferRange(GL_UNIFORM_BUFFER, index, dynBuffer, offset, size);
    }

    void setModelViewMatrix(v128* view)
    {
        mem_copy(autoVars.matMV, view, sizeof(v128)*4);

        ml::mul_mat4(autoVars.matMVP, autoVars.matP, view);
    }

    void setProjectionMatrix(v128* proj)
    {
        mem_copy(autoVars.matP, proj, sizeof(v128)*4);

        ml::mul_mat4(autoVars.matMVP, proj, autoVars.matMV);

        gfx::autoVars.projParams.x = proj[0].m128_f32[0];
        gfx::autoVars.projParams.y = proj[1].m128_f32[1];
        gfx::autoVars.projParams.z = proj[2].m128_f32[2];
        gfx::autoVars.projParams.w = proj[3].m128_f32[2];
    }

    void setUIMatrices()
    {
        ml::make_identity_mat4(autoVars.matMV);
        setProjectionMatrix(uiProj);
    }

    void set3DStates()
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
    }

    void set2DStates()
    {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }


    void gpu_timer_init(gpu_timer_t* timer)
    {
        glCreateQueries(GL_TIME_ELAPSED, NUM_FRAMES_DELAY, timer->queries);
        //Work around in order to avoid getting GL error when querying timer before it was issued
        for (size_t i = 0; i < NUM_FRAMES_DELAY; ++i)
        {
            glBeginQuery(GL_TIME_ELAPSED, timer->queries[i]);
            glEndQuery(GL_TIME_ELAPSED);
        }
        timer->measuredTime = 0;
    }

    void gpu_timer_fini(gpu_timer_t* timer)
    {
        glDeleteQueries(NUM_FRAMES_DELAY, timer->queries);
    }

    void gpu_timer_start(gpu_timer_t* timer)
    {
        glGetQueryObjectui64v(timer->queries[frameID], GL_QUERY_RESULT, &timer->measuredTime);
        glBeginQuery(GL_TIME_ELAPSED, timer->queries[frameID]);
    }

    void gpu_timer_stop(gpu_timer_t* timer)
    {
        UNUSED(timer);
        glEndQuery(GL_TIME_ELAPSED);
    }

    uint64_t gpu_timer_measured(gpu_timer_t* timer)
    {
        return timer->measuredTime / 1000;
    }
}

int gfx_alloc_geom(
    gfx_stack_alloc32_t* alloc,
    uint32_t  vertexSize,   uint32_t  vertexCount,
    uint32_t  indexSize,    uint32_t  indexCount,
    uint32_t* vertexOffset, uint32_t* indexOffset,
    uint32_t* totalSize
)
{
    assert(alloc);
    assert(vertexOffset);
    assert(indexOffset);

    GLuint verticesSize = vertexSize * vertexCount;
    GLuint indicesSize  = indexSize * indexCount;

    size_t voffset;
    size_t ioffset;

    voffset = core::align_up(alloc->used, vertexSize);
    ioffset = bit_align_up(voffset+verticesSize, indexSize);

    size_t used = ioffset + indicesSize;

    int success = used <= alloc->size;

    if (success)
    {
        *vertexOffset = voffset;
        *indexOffset  = ioffset;
        *totalSize    = used - voffset;
        alloc->used    = used;
    }

    return success;
}

#include "DefaultFontData.cpp"
#include "Font.cpp"
#include "gfx_res.cpp"
#include "nanovg_gfx.cpp"
#include "Path.cpp"
#include "Paint.cpp"
#include "res_utils.cpp"
#include "SUI.cpp"
#include "VG.cpp"

extern "C"
{
#include "nanovg.c"
#include "nanovg_utils.c"
#include "opengl.c"
}
