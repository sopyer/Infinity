#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <SpectatorCamera.h>

#include "model.h"

v128 shPoly[10] = {
    { 0.09010f, -0.02145f, -0.12871f, 0.0f},
    {-0.09010f,  0.02145f,  0.12871f, 0.0f},
    {-0.11890f, -0.06688f, -0.11147f, 0.0f},
    {-0.09438f, -0.04290f, -0.10298f, 0.0f},
    {-0.22310f, -0.18878f, -0.40330f, 0.0f},
    { 0.48052f,  0.18020f,  0.12014f, 0.0f},
    {-0.29676f, -0.06140f,  0.01024f, 0.0f},
    { 0.39910f,  0.35816f,  0.61400f, 0.0f},
    {-0.34794f, -0.18420f, -0.27630f, 0.0f},
    {-0.07239f, -0.01280f, -0.03463f, 1.0f},
};

class Anima: public ui::Stage
{
private:
    SpectatorCamera     camera;
    glm::mat4           mProj;

    Model::model_t      model;
    Model::skeleton_t   skel;
    Model::animation_t  anim;
    Model::pose_t       pose;
    bool                mHasAnimation;


    CPUTimer            cpuTimer;
    GPUTimer            gpuTimer;

public:
    Anima()
    {
        Model::init();

        Model::loadModel("boblampclean.md5mesh", &model, &skel);
        mHasAnimation = Model::loadAnimation("boblampclean.md5anim", &anim, &skel);

        Model::createPose(&pose, &skel);

        mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration = glm::vec3(150, 150, 150);
        camera.maxVelocity  = glm::vec3(60, 60, 60);

        CHECK_GL_ERROR();
    }

    ~Anima()
    {
        Model::destroyModel    (&model);
        Model::destroyAnimation(&anim);
        Model::destroySkeleton (&skel);
        Model::destroyPose     (&pose);

        Model::fini();
    }

protected:
    void onPaint()
    {
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
        Model::render(&model, &skel, &pose, mProj*vm, shPoly);
        gpuTimer.stop();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glDisable(GL_BLEND);

        ui::displayStats(10.0f, 10.0f, 300.0f, 70.0f, cpuTime, gpuTime);

        CHECK_GL_ERROR();
    }

    float cpuTime, gpuTime;

    void onUpdate(float dt)
    {
        static const float maxTimeStep = 0.03333f;

        cpuTimer.start();

        Model::update(std::min(dt, maxTimeStep), &anim, &skel, &pose);

        cpuTime = (float)cpuTimer.elapsed();
        cpuTimer.stop();

        gpuTime = (float)gpuTimer.getResult();

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

    fwk::fini();

    return 0;
}