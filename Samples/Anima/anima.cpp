#define _CRT_SECURE_NO_WARNINGS

#include <framework.h>
#include <SpectatorCamera.h>
#include <graphics.h>

#include "model.h"

ml::vec3 shPoly_v3[10] = {
    { 0.09010f, -0.02145f, -0.12871f},
    {-0.09010f,  0.02145f,  0.12871f},
    {-0.11890f, -0.06688f, -0.11147f},
    {-0.09438f, -0.04290f, -0.10298f},
    {-0.22310f, -0.18878f, -0.40330f},
    { 0.48052f,  0.18020f,  0.12014f},
    {-0.29676f, -0.06140f,  0.01024f},
    { 0.39910f,  0.35816f,  0.61400f},
    {-0.34794f, -0.18420f, -0.27630f},
    {-0.07239f, -0.01280f, -0.03463f},
};

class Anima: public ui::Stage
{
private:
    SpectatorCamera     camera;
    v128                mProj[4];

    Model::model_t      model;
    Model::skeleton_t   skel;
    Model::animation_t  anim;
    Model::pose_t       pose;
    bool                mHasAnimation;


    cpu_timer_t        cpuTimer;
    gfx::gpu_timer_t   gpuTimer;

public:
    Anima()
    {
        Model::init();

        Model::loadModel("boblampclean.md5mesh", &model, &skel);
        mHasAnimation = Model::loadAnimation("boblampclean.md5anim", &anim, &skel);

        Model::createPose(&pose, &skel);

        ml::make_perspective_mat4(mProj, 30.0f * FLT_DEG_TO_RAD_SCALE, mWidth/mHeight, 0.1f, 10000.0f);

        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x = camera.maxVelocity.y = camera.maxVelocity.z = 60;

        memcpy(&gfx::autoVars.shCoef, shPoly_v3, sizeof(ml::vec3) * 10);

        gfx::autoVars.projParams.x = mProj[0].m128_f32[0];
        gfx::autoVars.projParams.y = mProj[1].m128_f32[1];
        gfx::autoVars.projParams.z = mProj[2].m128_f32[2];
        gfx::autoVars.projParams.w = mProj[3].m128_f32[2];

        gfx::gpu_timer_init(&gpuTimer);
    }

    ~Anima()
    {
        gfx::gpu_timer_fini(&gpuTimer);

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
        glLoadMatrixf((float*)mProj);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        v128 m[4];
        camera.getViewMatrix(m);

        glLoadMatrixf((float*)m);

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

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        memcpy(&gfx::autoVars.matMV, (float*)m, sizeof(float) * 16);

        gpu_timer_start(&gpuTimer);
        Model::render(&model, &skel, &pose);
        gpu_timer_stop(&gpuTimer);

        ui::displayStats(
            10.0f, 10.0f, 300.0f, 70.0f,
            cpu_timer_measured(&cpuTimer) / 1000.0f,
            gfx::gpu_timer_measured(&gpuTimer) / 1000.0f
        );
    }

    void onUpdate(float dt)
    {
        static const float maxTimeStep = 0.03333f;

        cpu_timer_start(&cpuTimer);
        Model::update(std::min(dt, maxTimeStep), &anim, &skel, &pose);
        cpu_timer_stop(&cpuTimer);

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