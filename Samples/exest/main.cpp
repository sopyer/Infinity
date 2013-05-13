#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <glm\glmext.h>
#include <ResourceHelpers.h>
#include <SOIL.h>
#include <SpectatorCamera.h>
#include <CameraDirector.h>
#include <utils.h>

#include "CDLODTerrain.h"

class Exest: public ui::Stage
{
private:
    SpectatorCamera camera;

    glm::mat4 mProj;
    
    bool      fixedMode;
    glm::vec3 VPpp;
    glm::mat4 VP;

    CDLODTerrain terrain;

    mt::Task            mUpdateTask;

    CameraDirector camDirector;

public:
    Exest(): fixedMode(false), loc(0)
    {
        terrain.initialize();

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);
        //camera.viewMatrix = glm::mat4(-0.68835747f,   2.7175300e-008f,   0.72537768f,   0.0f,
        //    0.094680540f,  0.99144882f,       0.089849062f,  0.0f,
        //    -0.71917069f,   0.13052642f,      -0.68246961f,   0.0f,
        //    0.0f,          0.0f,              0.0f,          1.0f);
        //camera.pos = glm::vec3(-451.47745f,  45.857117f, -437.07669f);

        glm::mat4 lookAt = glm::lookAtGTX<float>(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(1, 0, 0));
        glm::mat4 proj = glm::perspectiveGTX<float>(33.0f, 1.33333333f, 0.1f, 1200.0f);

        VP = proj*lookAt;

        PHYSFS_File*    src = PHYSFS_openRead("hm.raw");
        //explicit conversion to avoid warning on 32-bit system
        //assert(src.size()<SIZE_MAX);
        ///size_t fileSize = (size_t)src.size();

        //src.read(data, fileSize, 1);

        //int	imgWidth, imgHeight, imgChannels;
        //unsigned char*	pixelsPtr = SOIL_load_image_from_memory(data, fileSize,
        //	&imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_L);
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

        //SOIL_free_image_data(pixelsPtr);
        delete [] data;

        GLuint prgs[] = {terrain.prgTerrain, terrain.prgInstancedTerrain};
        addPrograms(2, prgs);
    }

    ~Exest()
    {
        terrain.cleanup();
    }

    void drawFrustum(const glm::mat4& frustum)
    {
        glm::mat4 inverted = glm::inverseGTX(frustum);

        static const glm::vec4 points[8] = {
            glm::vec4(-1, -1, -1, 1), 
            glm::vec4( 1, -1, -1, 1), 
            glm::vec4( 1,  1, -1, 1), 
            glm::vec4(-1,  1, -1, 1), 
            glm::vec4(-1, -1,  1, 1), 
            glm::vec4( 1, -1,  1, 1), 
            glm::vec4( 1,  1,  1, 1), 
            glm::vec4(-1,  1,  1, 1), 
        };

        glm::vec4 wpoints[8];

        for (size_t i=0; i<8; ++i)
            wpoints[i] = inverted*points[i];

        glUseProgram(0);
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        glVertex4fv(wpoints[0]); glVertex4fv(wpoints[1]);
        glVertex4fv(wpoints[1]); glVertex4fv(wpoints[2]);
        glVertex4fv(wpoints[2]); glVertex4fv(wpoints[3]);
        glVertex4fv(wpoints[3]); glVertex4fv(wpoints[0]);

        glVertex4fv(wpoints[4]); glVertex4fv(wpoints[5]);
        glVertex4fv(wpoints[5]); glVertex4fv(wpoints[6]);
        glVertex4fv(wpoints[6]); glVertex4fv(wpoints[7]);
        glVertex4fv(wpoints[7]); glVertex4fv(wpoints[4]);

        glVertex4fv(wpoints[0]); glVertex4fv(wpoints[4]);
        glVertex4fv(wpoints[1]); glVertex4fv(wpoints[5]);
        glVertex4fv(wpoints[2]); glVertex4fv(wpoints[6]);
        glVertex4fv(wpoints[3]); glVertex4fv(wpoints[7]);
        glEnd();
    }

protected:
    void onShaderRecompile()
    {
        terrain.reset();
    }

    std::vector<glm::__quatGTX> savedCamOrient;
    std::vector<glm::vec3>      savedCamPos;
    int                         loc;

    void onKeyUp(const KeyEvent& event)
    {
        if ((event.keysym.sym==SDLK_LALT  && event.keysym.mod==KMOD_LCTRL||
            event.keysym.sym==SDLK_LCTRL && event.keysym.mod==KMOD_LALT))
        {
            releaseMouse();
        }
        if (event.keysym.sym==SDLK_BACKSLASH)
        {
            terrain.maxPatchCount = (terrain.maxPatchCount)?0:CDLODTerrain::MAX_PATCH_COUNT;
        }
        if (event.keysym.sym==SDLK_LEFTBRACKET)
        {
            terrain.maxPatchCount -= (terrain.maxPatchCount>0)?1:0;
        }
        if (event.keysym.sym==SDLK_RIGHTBRACKET)
        {
            terrain.maxPatchCount += (terrain.maxPatchCount<CDLODTerrain::MAX_PATCH_COUNT)?1:0;
        }
    }

    void onTouch(const ButtonEvent& event)
    {
        UNUSED(event);
        captureMouse();
    }

    void onPaint()
    {
        GLenum err;

        glClearDepth(1.0);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadMatrixf(mProj);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glm::mat4 vm;
        camera.getViewMatrix(vm);
        glLoadMatrixf(vm);

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

        terrain.viewDir = glm::vec4(vm[0].z, vm[1].z, -vm[2].z, 1);

        if (!fixedMode)
        {
            terrain.viewPoint = camera.getPosition();
            terrain.setSelectMatrix(mProj*vm);
        }
        else
        {
            terrain.viewPoint = VPpp;
            terrain.setSelectMatrix(VP);
            drawFrustum(VP);
        }

        terrain.setMVPMatrix(mProj*vm);

        terrain.drawTerrain();

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

        err = glGetError();
        assert(err==GL_NO_ERROR);
    }

    void onUpdate(float dt)
    {
        bool lockView = false;

        if (isMouseCaptured())
        {
            ui::processCameraInput(&camera, dt);
        }

        ui::processCameraDirectorInput(&camDirector, &camera);

        if (ui::keyWasReleased(SDL_SCANCODE_L))
        {
            fixedMode = !fixedMode;
            lockView  = fixedMode==true;
        }

        if (ui::keyWasReleased(SDL_SCANCODE_F))
        {
            terrain.drawWireframe = !terrain.drawWireframe;
        }

        if (ui::keyWasReleased(SDL_SCANCODE_I))
        {
            terrain.useInstancing = !terrain.useInstancing;
        }

        if (lockView)
        {
            glm::mat4 vm;
            camera.getViewMatrix(vm);
            VP = mProj*vm;
            VPpp = camera.getPosition();
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

    fwk::cleanup();

    return 0;
}