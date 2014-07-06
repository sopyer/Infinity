#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <ResourceHelpers.h>
#include <SOIL.h>
#include <SpectatorCamera.h>
#include <CameraDirector.h>
#include <utils.h>

#include "CDLODTerrain.h"

static const float zn = 0.1f;
static const float zf = 10000.0f;

class Exest: public ui::Stage
{
private:
    SpectatorCamera camera;

    v128 mProj[4];

    bool      fixedMode;
    ml::vec3  VPpp;
    v128      VP[4];
    v128      viewQuat;

    CDLODTerrain terrain;

    CameraDirector camDirector;

    bool drawWireframe;

public:
    Exest(): fixedMode(false)
    {
        terrain.initialize();

        ml::make_perspective_mat4(mProj, 30.0f * FLT_DEG_TO_RAD_SCALE, mWidth/mHeight, zn, zf);

        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x = camera.maxVelocity.y = camera.maxVelocity.z = 60;

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

        addPrograms(1, &terrain.prgTerrain);

        graphics::autoVars.projParams.x = mProj[0].m128_f32[0];
        graphics::autoVars.projParams.y = mProj[1].m128_f32[1];
        graphics::autoVars.projParams.z = mProj[2].m128_f32[2];
        graphics::autoVars.projParams.w = mProj[3].m128_f32[2];
    }

    ~Exest()
    {
        terrain.cleanup();
    }

    void drawFrustum(float zn, float zf)
    {
        float sx = 1.0f / mProj[0].m128_f32[0];
        float sy = 1.0f / mProj[1].m128_f32[1];

        v128 scale = vi_set(sx, sy, -1.0f, 0.0f);
        v128 vzn   = vi_set_fff0(zn);
        v128 vzf   = vi_set_fff0(zf);
        v128 pn    = vi_mul(vzn, scale);
        v128 pf    = vi_mul(vzf, scale);
        v128 sign  = vi_set_i000(FLT_SIGN);

        ml::vec3 frustumPoints[8];

        v128 pts[8] = {
            vi_or(vi_swizzle<VI_SWIZZLE_MASK(0, 0, 1, 1)>(sign), pn),
            vi_or(vi_swizzle<VI_SWIZZLE_MASK(1, 0, 1, 1)>(sign), pn),
            vi_or(vi_swizzle<VI_SWIZZLE_MASK(1, 1, 1, 1)>(sign), pn),
            vi_or(vi_swizzle<VI_SWIZZLE_MASK(0, 1, 1, 1)>(sign), pn),
            vi_or(vi_swizzle<VI_SWIZZLE_MASK(0, 0, 1, 1)>(sign), pf),
            vi_or(vi_swizzle<VI_SWIZZLE_MASK(1, 0, 1, 1)>(sign), pf),
            vi_or(vi_swizzle<VI_SWIZZLE_MASK(1, 1, 1, 1)>(sign), pf),
            vi_or(vi_swizzle<VI_SWIZZLE_MASK(0, 1, 1, 1)>(sign), pf)
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

        glUseProgram(0);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3fv((float*)&frustumPoints[0]); glVertex3fv((float*)&frustumPoints[1]);
        glVertex3fv((float*)&frustumPoints[1]); glVertex3fv((float*)&frustumPoints[2]);
        glVertex3fv((float*)&frustumPoints[2]); glVertex3fv((float*)&frustumPoints[3]);
        glVertex3fv((float*)&frustumPoints[3]); glVertex3fv((float*)&frustumPoints[0]);

        glVertex3fv((float*)&frustumPoints[4]); glVertex3fv((float*)&frustumPoints[5]);
        glVertex3fv((float*)&frustumPoints[5]); glVertex3fv((float*)&frustumPoints[6]);
        glVertex3fv((float*)&frustumPoints[6]); glVertex3fv((float*)&frustumPoints[7]);
        glVertex3fv((float*)&frustumPoints[7]); glVertex3fv((float*)&frustumPoints[4]);

        glVertex3fv((float*)&frustumPoints[0]); glVertex3fv((float*)&frustumPoints[4]);
        glVertex3fv((float*)&frustumPoints[1]); glVertex3fv((float*)&frustumPoints[5]);
        glVertex3fv((float*)&frustumPoints[2]); glVertex3fv((float*)&frustumPoints[6]);
        glVertex3fv((float*)&frustumPoints[3]); glVertex3fv((float*)&frustumPoints[7]);
        glEnd();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glEnable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_2D);

        //Simple sorting for convex alpha object rendering
        //First render back faces
        //Then render front faces
        glColor4f(1.0f, 0.5f, 0.0f, 0.5f);
        glCullFace(GL_FRONT);
        glBegin(GL_TRIANGLES);
        for (int i=0; i<ARRAY_SIZE(indices); ++i)
        {
            glVertex3fv((float*)&frustumPoints[indices[i]]);
        }
        glEnd();

        glCullFace(GL_BACK);
        glBegin(GL_TRIANGLES);
        for (int i=0; i<ARRAY_SIZE(indices); ++i)
        {
            glVertex3fv((float*)&frustumPoints[indices[i]]);
        }
        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
    }

protected:
    void onShaderRecompile()
    {
        terrain.reset();
    }

    void onPaint()
    {
        glClearDepth(1.0);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadMatrixf((float*)mProj);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        v128 vm[4];
        camera.getViewMatrix(vm);
        glLoadMatrixf((float*)vm);

        v128 MVP[4];
        ml::mul_mat4(MVP, mProj, vm);
        memcpy(&graphics::autoVars.matMV,  vm,  sizeof(v128)*4);
        memcpy(&graphics::autoVars.matMVP, MVP, sizeof(v128)*4);

        glDisable(GL_CULL_FACE);

        glUseProgram(0);
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(-3, -1, -10);
        glColor3f(0, 1, 0);
        glVertex3f( 3, -1, -10);
        glColor3f(0, 0, 1);
        glVertex3f( 0, 4, -10);
        glEnd();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if (drawWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);

        if (!fixedMode)
        {
            vi_store_v3(&terrain.viewPoint, camera.getPosition());
            terrain.setSelectMatrix(MVP);
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

        glDisable(GL_CULL_FACE);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);

        //Draw ui
        ui::displayStats(10.0f, 10.0f, 300.0f, 120.0f, terrain.getCPUTime(), terrain.getGPUTime());

        char str[256];
        
        _snprintf(str, 256, "CPU select time - %f ms", terrain.getCPUSelectTime());
        vg::drawString(ui::defaultFont, 25.0f, 83.0f, str, strlen(str));
        
        _snprintf(str, 256, "CPU draw time - %f ms", terrain.getCPUDrawTime());
        vg::drawString(ui::defaultFont, 25.0f, 101.0f, str, strlen(str));
        
        int patches = terrain.patchCount;
        int vtx = patches*terrain.patchDim*terrain.patchDim;
        _snprintf(str, 256, "Patches: %d, Vtx: %d", patches, vtx);
        vg::drawString(ui::defaultFont, 25.0f, 119.0f, str, strlen(str));

        CHECK_GL_ERROR();
    }

    void onUpdate(float dt)
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
                ml::mul_mat4(VP, mProj, vm);
                vi_store_v3(&VPpp, camera.getPosition());
                viewQuat = ml::conjugate_quat(camera.getOrientation());
            }
        }

        if (ui::keyWasReleased(SDL_SCANCODE_F))
        {
            drawWireframe = !drawWireframe;
        }
    }
};

int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    {
        Exest app;
        app.run();
    }

    fwk::fini();

    return 0;
}