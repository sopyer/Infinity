#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <glm\glmext.h>
#include <utils.h>
#include <SpectatorCamera.h>

#include "model.h"

GLfloat cubeVertices[8*3] =
{
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
};

GLushort cubeIndices[6*2*3] = 
{
    0, 1, 3, 0, 3, 4, 0, 4, 1,
    2, 3, 1, 2, 1, 6, 2, 6, 3,
    5, 6, 1, 5, 1, 4, 5, 4, 6,
    7, 4, 3, 7, 3, 6, 7, 6, 4,
};

class Anima: public ui::Stage
{
private:
    SpectatorCamera     camera;
    glm::mat4           mProj;

    MD5Model            model;

    CPUTimer            cpuTimer;
    GPUTimer            gpuTimer;

    GLuint              prgSkybox;

public:
    Anima()
    {
        model.LoadModel("boblampclean.md5mesh");
        model.LoadAnim ("boblampclean.md5anim");

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);

        prgSkybox = resources::createProgramFromFiles("skybox.vert", "skybox.frag");

        addPrograms(1, &prgSkybox);
        addPrograms(1, &model.prgLighting);
        addPrograms(1, &model.prgDefault);
    }

    ~Anima()
    {
        glDeleteProgram(prgSkybox);
    }

protected:
    void drawSkybox()
    {
        glCullFace(GL_FRONT);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices);

        glUseProgram(prgSkybox);

        glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_SHORT, cubeIndices);

        glUseProgram(0);

        glDisableVertexAttribArray(0);
        glCullFace(GL_BACK);

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

        glFrontFace(GL_CCW);
        glCullFace (GL_BACK);
        glEnable   (GL_CULL_FACE);

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

        drawSkybox();
 
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