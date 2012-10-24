#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <glm\glmext.h>
#include <cassert>
#include <SpectatorCamera.h>

#include "model.h"

class Anima: public ui::Stage
{
private:
    SpectatorCamera     camera;

    VFS				mVFS;

    glm::mat4	mProj;

    mt::Task                mUpdateTask;
    ui::ProfileStatsBox     mStatsBox;

    MD5Model model;

    CPUTimer	cpuTimer;
    GPUTimer	gpuTimer;

public:
    Anima(): moveFwd(false), moveBwd(false), moveLeft(false), moveRight(false), loc(0)
    {
        lastTimeMark = timerAbsoluteTime();

        VFS::mount("AppData");
        VFS::mount("../AppData");
        VFS::mount("../../AppData");

        model.skin4PointShader.Create();
        model.LoadModel("boblampclean.md5mesh");
        model.LoadAnim ("boblampclean.md5anim");

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);

        glm::mat4 lookAt = glm::lookAtGTX<float>(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(1, 0, 0));
        glm::mat4 proj = glm::perspectiveGTX<float>(33.0f, 1.33333333f, 0.1f, 1200.0f);

        mt::addTimedTask<Anima, &Anima::handleInput>(this, 20);

        add(mStatsBox
            .setPos(10, 10)
            .setSize(300, 70)
            );

		addPrograms(1, &model.skin4PointShader.program);

        FILE* cam = fopen("cameras.txt", "r");
        if (cam)
        {
            while(!feof(cam))
            {
                glm::__quatGTX  q;
                glm::vec3       p;
                fscanf(cam, "%f %f %f %f %f %f %f\n", &q.x, &q.y, &q.z, &q.w, &p.x, &p.y, &p.z);
                savedCamOrient.push_back(q);
                savedCamPos.push_back(p);
            }
            fclose(cam);
        }
    }

    ~Anima()
    {
        model.skin4PointShader.Destroy();
        FILE* cam = fopen("cameras.txt", "w");
        if (cam)
        {
            for (size_t i=0; i<savedCamOrient.size(); ++i)
            {
                glm::__quatGTX  q=savedCamOrient[i];
                glm::vec3       p=savedCamPos[i];
                fprintf(cam, "%f %f %f %f %f %f %f\n", q.x, q.y, q.z, q.w, p.x, p.y, p.z);
            }
            fclose(cam);
        }
    }

protected:
    void onShaderRecompile()
    {
		model.skin4PointShader.bindUniforms();
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
        if (event.keysym.sym==SDLK_0)
        {
            savedCamOrient.push_back(camera.getOrientation());
            savedCamPos.push_back(camera.getPosition());
        }
        if (event.keysym.sym==SDLK_PERIOD && savedCamOrient.size())
        {
            loc=(loc+1)%savedCamOrient.size();
            camera.setOrientation(savedCamOrient[loc]);
            camera.setPosition(savedCamPos[loc]);
        }
        if (event.keysym.sym==SDLK_COMMA && savedCamOrient.size())
        {
            loc=(loc-1)%savedCamOrient.size();
            camera.setOrientation(savedCamOrient[loc]);
            camera.setPosition(savedCamPos[loc]);
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

        cpuTimer.start();

        static const float maxTimeStep = 0.03333f;
        static float       prevTime    = std::clock() / (float)CLOCKS_PER_SEC;
        
        float curTime   = std::clock() / (float)CLOCKS_PER_SEC;
        float deltaTime = curTime - prevTime;
        
        prevTime  = curTime;
        deltaTime = std::min( deltaTime, maxTimeStep );

        model.Update(deltaTime);
        
        double cpuTime = cpuTimer.elapsed();
        cpuTimer.stop();

        gpuTimer.start();
        model.Render(mProj*vm);
        gpuTimer.stop();

        mStatsBox.setStats((float)cpuTime, (float)gpuTimer.getResult());

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);

        err = glGetError();
        assert(err==GL_NO_ERROR);
    }

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
    }
};

int main(int argc, char** argv)
{
    Anima app;
    app.run();

    return 0;
}