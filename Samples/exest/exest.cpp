#define _CRT_SECURE_NO_WARNINGS

#include <fwk/fwk.h>

#include "CDLODTerrain.h"

static const float zn = 0.1f;
static const float zf = 10000.0f;

namespace app
{
    SpectatorCamera camera;

    v128 proj[4];

    bool      fixedMode = false;
    ml::vec3  VPpp;
    v128      VP[4];
    v128      viewQuat;

    CDLODTerrain terrain;

    CameraDirector camDirector;

    bool drawWireframe;

    void init()
    {
        terrain.initialize();

        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x = camera.maxVelocity.y = camera.maxVelocity.z = 60;
        camera.setPosition(vi_set(0.0f, 80.0f, 110.0f, 1.0f));

        PHYSFS_File*    src = PHYSFS_openRead("hm.raw");
        //explicit conversion to avoid warning on 32-bit system
        //assert(src.size()<SIZE_MAX);
        ///size_t fileSize = (size_t)src.size();

        //src.read(data, fileSize, 1);

        //int   imgWidth, imgHeight, imgChannels;
        //unsigned char*    pixelsPtr = SOIL_load_image_from_memory(data, fileSize,
        //  &imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_L);
        //assert(imgChannels==1);

        //uint32_t blockCount, vertCount;
        //src.seek(4+4+4);
        //src.read(&blockCount, sizeof(blockCount), 1);
        //vertCount = blockCount*16+1;
        uint32_t w = 3073;
        uint32_t h = 4097;

        uint16_t* data = new uint16_t[w*h];
        
        PHYSFS_read(src, data, w*h*sizeof(uint16_t), 1);
        PHYSFS_close(src);

        terrain.setHeightmap(data, w, h);
        
        drawWireframe = false;
        
        //SOIL_free_image_data(pixelsPtr);
        delete [] data;

        ui::debugAddPrograms(1, &terrain.prgTerrain);
    }

    void fini()
    {
        terrain.cleanup();
    }

    void drawFrustum(float zn, float zf)
    {
        float sx = 1.0f / proj[0].m128_f32[0];
        float sy = 1.0f / proj[1].m128_f32[1];

        v128 scale = vi_set(sx, sy, -1.0f, 0.0f);
        v128 vzn   = vi_set_all(zn);
        v128 vzf   = vi_set_all(zf);
        v128 pn    = vi_mul(vzn, scale);
        v128 pf    = vi_mul(vzf, scale);
        v128 sign  = vi_seti_x(VI_SIGN_MASK);

        ml::vec3 frustumPoints[8];

        v128 pts[8] = {
            vi_or(vi_swizzle<0, 0, 1, 1>(sign), pn),
            vi_or(vi_swizzle<1, 0, 1, 1>(sign), pn),
            vi_or(vi_swizzle<1, 1, 1, 1>(sign), pn),
            vi_or(vi_swizzle<0, 1, 1, 1>(sign), pn),
            vi_or(vi_swizzle<0, 0, 1, 1>(sign), pf),
            vi_or(vi_swizzle<1, 0, 1, 1>(sign), pf),
            vi_or(vi_swizzle<1, 1, 1, 1>(sign), pf),
            vi_or(vi_swizzle<0, 1, 1, 1>(sign), pf)
        };

        v128 pos = vi_load_v3(&VPpp);
        for (size_t i=0; i<ARRAY_SIZE(frustumPoints); ++i)
        {
            v128 r = ml::rotate_vec3_quat(viewQuat, pts[i]);
            vi_store_v3(&frustumPoints[i], vi_add(r, pos));
        }

        static const int indices[36] = 
        {
            0, 1, 3, 1, 2, 3,
            1, 0, 4, 1, 4, 5,
            3, 2, 7, 2, 6, 7,
            4, 0, 7, 0, 3, 7,
            1, 5, 2, 2, 5, 6,
            4, 7, 5, 5, 7, 6,
        };

        GLuint  offset;
        GLuint  size;
        v128*   vertices;
        static const size_t NUM_LINES = 12;

        size     = sizeof(gfx::line_t) * NUM_LINES;
        vertices = (v128*)gfx::dynbufAllocMem(size, gfx::caps.ssboAlignment, &offset);
        *vertices++ = vi_load_v3(&frustumPoints[0]); *vertices++ = vi_load_v3(&frustumPoints[1]);
        *vertices++ = vi_load_v3(&frustumPoints[1]); *vertices++ = vi_load_v3(&frustumPoints[2]);
        *vertices++ = vi_load_v3(&frustumPoints[2]); *vertices++ = vi_load_v3(&frustumPoints[3]);
        *vertices++ = vi_load_v3(&frustumPoints[3]); *vertices++ = vi_load_v3(&frustumPoints[0]);

        *vertices++ = vi_load_v3(&frustumPoints[4]); *vertices++ = vi_load_v3(&frustumPoints[5]);
        *vertices++ = vi_load_v3(&frustumPoints[5]); *vertices++ = vi_load_v3(&frustumPoints[6]);
        *vertices++ = vi_load_v3(&frustumPoints[6]); *vertices++ = vi_load_v3(&frustumPoints[7]);
        *vertices++ = vi_load_v3(&frustumPoints[7]); *vertices++ = vi_load_v3(&frustumPoints[4]);

        *vertices++ = vi_load_v3(&frustumPoints[0]); *vertices++ = vi_load_v3(&frustumPoints[4]);
        *vertices++ = vi_load_v3(&frustumPoints[1]); *vertices++ = vi_load_v3(&frustumPoints[5]);
        *vertices++ = vi_load_v3(&frustumPoints[2]); *vertices++ = vi_load_v3(&frustumPoints[6]);
        *vertices++ = vi_load_v3(&frustumPoints[3]); *vertices++ = vi_load_v3(&frustumPoints[7]);
        gfx::drawLines(vi_set(0.0f, 1.0f, 0.0f, 1.0f), NUM_LINES, gfx::dynBuffer, offset, size);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glEnable(GL_CULL_FACE);

        gfx::setStdProgram(gfx::STD_FEATURE_COLOR);
        gfx::setMVP();

        glBindVertexArray(vf::p3cu4_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p3cu4_vertex_t));

        GLuint baseVertex;
        GLuint numVertices = ARRAY_SIZE(indices);

        vf::p3cu4_vertex_t* v = gfx::frameAllocVertices<vf::p3cu4_vertex_t>(numVertices * 2, &baseVertex);

        uint32_t color = 0x800080FF;
        //Simple sorting for convex alpha object rendering
        //First render back faces
        //Then render front faces
        glCullFace(GL_FRONT);
        for (GLuint i=0; i<numVertices; ++i)
        {
            vf::set(v++,
                frustumPoints[indices[i]].x,
                frustumPoints[indices[i]].y,
                frustumPoints[indices[i]].z,
                color
            );
        }
        glDrawArrays(GL_TRIANGLES, baseVertex, numVertices);
        baseVertex+=+numVertices;

        glCullFace(GL_BACK);
        for (GLuint i=0; i<numVertices; ++i)
        {
            vf::set(v++,
                frustumPoints[indices[i]].x,
                frustumPoints[indices[i]].y,
                frustumPoints[indices[i]].z,
                color
            );
        }
        glDrawArrays(GL_TRIANGLES, baseVertex, numVertices);

        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
    }

    void recompilePrograms()
    {
        terrain.reset();
    }

    void resize(int width, int height)
    {
        ml::make_perspective_mat4(proj, 30.0f * FLT_DEG_TO_RAD_SCALE, (float)width/(float)height, 0.1f, 10000.0f);
    }

    void render()
    {
        v128 vm[4];
        camera.getViewMatrix(vm);

        gfx::set3DStates();

        gfx::setProjectionMatrix(proj);
        gfx::setModelViewMatrix(vm);

        glDisable(GL_CULL_FACE);

        gfx::setStdProgram(gfx::STD_FEATURE_COLOR);
        gfx::setMVP();

        glBindVertexArray(vf::p3cu4_vertex_t::vao);
        glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p3cu4_vertex_t));

        GLuint baseVertex;
        vf::p3cu4_vertex_t* v = gfx::frameAllocVertices<vf::p3cu4_vertex_t>(3, &baseVertex);

        vf::set(v++, -3.0f, -1.0f, -10.0f, 0xFF0000FF);
        vf::set(v++,  3.0f, -1.0f, -10.0f, 0xFF00FF00);
        vf::set(v++,  0.0f,  4.0f, -10.0f, 0xFFFF0000);
        glDrawArrays(GL_TRIANGLES, baseVertex, 3);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if (drawWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);

        if (!fixedMode)
        {
            vi_store_v3(&terrain.viewPoint, camera.getPosition());
            terrain.setSelectMatrix(gfx::autoVars.matMVP);
        }
        else
        {
            terrain.viewPoint = VPpp;
            terrain.setSelectMatrix(VP);
        }

        terrain.drawTerrain();

        if (drawWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (fixedMode)
        {
            drawFrustum(zn, 0.7f*zf);
        }

        gfx::set2DStates();
        gfx::setUIMatrices();

        //Draw ui
        ui::displayStats(
            10.0f, 10.0f, 300.0f, 120.0f,
            cpu_timer_measured(&terrain.cpuTimer) / 1000.0f,
            gfx::gpu_timer_measured(&terrain.gpuTimer) / 1000.0f
        );

        char str[256];

        _snprintf(str, 256, "CPU select time - %f ms", cpu_timer_measured(&terrain.cpuSelectTimer) / 1000.0f);
        vg::drawString(vg::defaultFont, 25.0f, 83.0f, 0xFFFFFFFF, str, strlen(str));

        _snprintf(str, 256, "CPU draw time - %f ms", cpu_timer_measured(&terrain.cpuRenderTimer) / 1000.0f);
        vg::drawString(vg::defaultFont, 25.0f, 101.0f, 0xFFFFFFFF, str, strlen(str));

        int patches = terrain.patchCount;
        int vtx = patches*terrain.patchDim*terrain.patchDim;
        _snprintf(str, 256, "Patches: %d, Vtx: %d", patches, vtx);
        vg::drawString(vg::defaultFont, 25.0f, 119.0f, 0xFFFFFFFF, str, strlen(str));
    }

    void update(float dt)
    {
        ui::processCameraInput(&camera, dt);
        ui::processCameraDirectorInput(&camDirector, &camera);

        if (ui::keyWasReleased(SDLK_BACKSLASH))
        {
            terrain.maxPatchCount = (terrain.maxPatchCount)?0:CDLODTerrain::MAX_PATCH_COUNT;
        }

        if (ui::keyWasReleased(SDLK_LEFTBRACKET))
        {
            terrain.maxPatchCount -= (terrain.maxPatchCount>0)?1:0;
        }

        if (ui::keyWasReleased(SDLK_RIGHTBRACKET))
        {
            terrain.maxPatchCount += (terrain.maxPatchCount<CDLODTerrain::MAX_PATCH_COUNT)?1:0;
        }

        if (ui::keyWasReleased(SDL_SCANCODE_L))
        {
            fixedMode = !fixedMode;
            if (fixedMode)
            {
                v128 vm[4];
                camera.getViewMatrix(vm);
                ml::mul_mat4(VP, proj, vm);
                vi_store_v3(&VPpp, camera.getPosition());
                viewQuat = ml::conjugate_quat(camera.getOrientation());
            }
        }

        if (ui::keyWasReleased(SDL_SCANCODE_F))
        {
            drawWireframe = !drawWireframe;
        }
    }
}
