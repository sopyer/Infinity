#include <framework.h>
#include <glm\glmext.h>
#include <cassert>
#include <ResourceHelpers.h>
#include <SOIL.h>
#include <SpectatorCamera.h>

#include "CDLODTerrain.h"

class Exest: public ui::SDLStage
{
private:
    SpectatorCamera     camera;

    VFS				mVFS;

    glm::mat4	mProj, VP;

    float vertAngle, horzAngle;
    CDLODTerrain terrain;

    mt::Task                mUpdateTask;
    ui::ProfileStatsBox     mStatsBox;

public:
    Exest(): fixedMode(false), moveFwd(false), moveBwd(false), moveLeft(false), moveRight(false)
    {
        lastTimeMark = timerAbsoluteTime();

        VFS::mount("AppData");
        VFS::mount("../AppData");
        VFS::mount("../../AppData");

        terrain.initialize();

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);
        //camera.viewMatrix = glm::mat4(-0.68835747f,   2.7175300e-008f,   0.72537768f,   0.0f,
        //    0.094680540f,  0.99144882f,       0.089849062f,  0.0f,
        //    -0.71917069f,   0.13052642f,      -0.68246961f,   0.0f,
        //    0.0f,          0.0f,              0.0f,          1.0f);
        //camera.pos = glm::vec3(-451.47745f,  45.857117f, -437.07669f);

        terrain.viewData.uViewPoint = vi_load_zero();
        glm::mat4 lookAt = glm::lookAtGTX<float>(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(1, 0, 0));
        glm::mat4 proj = glm::perspectiveGTX<float>(33.0f, 1.33333333f, 0.1f, 1200.0f);

        VP = proj*lookAt;

        mt::addTimedTask<Exest, &Exest::handleInput>(this, 20);

        File	src = VFS::openRead("bridge_demo.pmp");
        //explicit conversion to avoid warning on 32-bit system
        //assert(src.size()<SIZE_MAX);
        ///size_t fileSize = (size_t)src.size();

        //src.read(data, fileSize, 1);

        //int	imgWidth, imgHeight, imgChannels;
        //unsigned char*	pixelsPtr = SOIL_load_image_from_memory(data, fileSize,
        //	&imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_L);
        //assert(imgChannels==1);

        uint32_t blockCount, vertCount;
        src.seek(4+4+4);
        src.read(&blockCount, sizeof(blockCount), 1);
        vertCount = blockCount*16+1;
        uint16_t* data = new uint16_t[vertCount*vertCount];
        src.read(data, vertCount*vertCount*sizeof(uint16_t), 1);
        terrain.setHeightmap(data, vertCount, vertCount);

        //SOIL_free_image_data(pixelsPtr);
        delete [] data;

        memset(prevKeystate, 0, SDLK_LAST);

        add(mStatsBox.add(mSelectTimeLabel)
            .add(mDrawTimeLabel)
            .setPos(10, 10)
            .setSize(300, 120)
            );

        mt::addTimedTask<Exest, &Exest::onUpdateStats>(this, 250);

        GLuint prgs[] = {terrain.prgTerrain, terrain.prgInstancedTerrain};
        addPrograms(2, prgs);
    }

    ui::Label	mDrawTimeLabel;
    ui::Label	mSelectTimeLabel;

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

    void onKeyUp(const KeyEvent& event)
    {
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

    void onPaint()
    {
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
            glm::vec3 pos = camera.getPosition();
            terrain.viewData.uViewPoint = vi_set(pos.x, pos.y, pos.z, 0.0f);
            terrain.setSelectMatrix(mProj*vm);
        }
        else
        {
            terrain.viewData.uViewPoint = vi_set(VPpp.x, VPpp.y, VPpp.z, 0.0f);
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

        GLenum err = glGetError();
        assert(err==GL_NO_ERROR);
    }

    void onUpdateStats()
    {
        mStatsBox.setStats(terrain.getCPUTime(), terrain.getGPUTime());
        wchar_t str[256];
        _snwprintf(str, 256, L"CPU select time - %f ms", terrain.getCPUSelectTime());
        mSelectTimeLabel.setText(str);
        _snwprintf(str, 256, L"CPU draw time - %f ms", terrain.getCPUDrawTime());
        mDrawTimeLabel.setText(str);
    }

    bool fixedMode;
    glm::vec3 VPpp;
    uint8_t prevKeystate[SDLK_LAST];
    bool moveFwd, moveBwd, moveLeft, moveRight;
    __int64 lastTimeMark;

    void onMotion(const MotionEvent& event)
    {
        camera.rotateSmoothly((float)-event.xrel, (float)-event.yrel);
    }

    void handleInput()
    {
        glm::vec3 direction;
        float heading = 0.0f, pitch = 0.0f;
        uint8_t *keystate = SDL_GetKeyState(NULL);
        if (keystate[SDLK_ESCAPE])
            close();
        if (keystate[SDLK_w])
        {
            camera.velocity.z = moveFwd?camera.velocity.z:0.0f;
            moveFwd = true;
            direction.z += 1.0f;
        }
        else
            moveFwd = false;
        if (keystate[SDLK_s])
        {
            camera.velocity.z = moveBwd?camera.velocity.z:0.0f;
            moveBwd = true;
            direction.z -= 1.0f;
        }
        else
            moveBwd = false;
        if (keystate[SDLK_a])
        {
            camera.velocity.x = moveLeft?camera.velocity.x:0.0f;
            moveLeft = true;
            direction.x -= 1.0f;
        }
        else
            moveLeft = false;
        if (keystate[SDLK_d])
        {
            camera.velocity.x = moveRight?camera.velocity.x:0.0f;
            moveRight = true;
            direction.x += 1.0f;
        }
        else
            moveRight = false;
        if (keystate[SDLK_KP8])
            pitch += 1.50f;
        if (keystate[SDLK_KP5])
            pitch += -1.50f;
        if (keystate[SDLK_KP4])
            heading += 1.50f;
        if (keystate[SDLK_KP6])
            heading += -1.50f;

        _int64 time = timerAbsoluteTime();

        camera.rotateSmoothly(heading, pitch);
        camera.updatePosition(direction, (time-lastTimeMark)*0.000001f);

        lastTimeMark = time;

        bool lockView = false;
        if (keystate[SDLK_l]&&!prevKeystate[SDLK_l])
        {
            fixedMode = !fixedMode;
            lockView  = fixedMode==true;
        }

        if (keystate[SDLK_f]&&!prevKeystate[SDLK_f])
        {
            terrain.drawWireframe = !terrain.drawWireframe;
        }

        if (keystate[SDLK_i]&&!prevKeystate[SDLK_i])
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

        memcpy(prevKeystate, keystate, SDLK_LAST);
    }
};

int main(int argc, char** argv)
{
    Exest app;
    app.run();

    return 0;
}