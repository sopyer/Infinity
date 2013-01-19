#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <glm\glmext.h>
#include <utils.h>
#include <SpectatorCamera.h>

#include "model.h"

class Anima: public ui::Stage
{
private:
    SpectatorCamera     camera;

    glm::mat4	mProj;

    mt::Task                mUpdateTask;

    MD5Model model;

    CPUTimer	cpuTimer;
    GPUTimer	gpuTimer;

public:
    Anima()
    {
        model.LoadModel("boblampclean.md5mesh");
        model.LoadAnim ("boblampclean.md5anim");

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);

        addPrograms(1, &model.program);
    }

    ~Anima()
    {
    }

protected:
    void onShaderRecompile()
    {
        model.resetUniforms();
    }

    void onKeyUp(const KeyEvent& event)
    {
        if ((event.keysym.sym==SDLK_LALT  && event.keysym.mod==KMOD_LCTRL||
            event.keysym.sym==SDLK_LCTRL && event.keysym.mod==KMOD_LALT))
        {
            releaseMouse();
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

        gpuTimer.start();
        model.Render(mProj*vm);
        gpuTimer.stop();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, cpuTime, gpuTime);

        err = glGetError();
        assert(err==GL_NO_ERROR);
    }

    float cpuTime, gpuTime;

    void onUpdate(float dt)
    {
        static const float maxTimeStep = 0.03333f;

        cpuTimer.start();

        model.Update(std::min(dt, maxTimeStep));

        cpuTime = (float)cpuTimer.elapsed();
        cpuTimer.stop();

        gpuTime = (float)gpuTimer.getResult();

        if (isMouseCaptured())
            ui::processCameraInput(&camera, dt);
    }
};

int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    {
        Anima app;
        app.run();
    }

    fwk::cleanup();

    return 0;
}