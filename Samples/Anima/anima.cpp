#define _CRT_SECURE_NO_WARNINGS

#include <fwk/fwk.h>

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

namespace app
{
    SpectatorCamera     camera;
    v128                proj[4];

    Model::model_t      model;
    Model::skeleton_t   skel;
    Model::animation_t  anim;
    Model::pose_t       pose;
    bool                mHasAnimation;


    cpu_timer_t        cpuTimer;
    gfx::gpu_timer_t   gpuTimer;

    void init()
    {
        Model::init();

        Model::loadModel("boblampclean.md5mesh", &model, &skel);
        mHasAnimation = Model::loadAnimation("boblampclean.md5anim", &anim, &skel);

        Model::createPose(&pose, &skel);

        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x  = camera.maxVelocity.y  = camera.maxVelocity.z  =  60;

        memcpy(&gfx::autoVars.shCoef, shPoly_v3, sizeof(ml::vec3) * 10);

        gfx::gpu_timer_init(&gpuTimer);
    }

    void fini()
    {
        gfx::gpu_timer_fini(&gpuTimer);

        Model::destroyModel    (&model);
        Model::destroyAnimation(&anim);
        Model::destroySkeleton (&skel);
        Model::destroyPose     (&pose);

        Model::fini();
    }

    void render()
    {
        glClearDepth(1.0);

        //!!!!!TODO: fix culling
        //glFrontFace(GL_CW);
        //glCullFace (GL_BACK);
        //glEnable   (GL_CULL_FACE);

        v128 m[4];
        camera.getViewMatrix(m);
        memcpy(&gfx::autoVars.matMV, (float*)m, sizeof(float) * 16);

        gfx::drawXZGrid(-500.0f, -500.0f, 500.0f, 500.0f, 40, vi_set(0.0f, 1.0f, 1.0f, 1.0f));

        gpu_timer_start(&gpuTimer);
        Model::render(&model, &skel, &pose);
        gpu_timer_stop(&gpuTimer);

        ui::displayStats(
            10.0f, 10.0f, 300.0f, 70.0f,
            cpu_timer_measured(&cpuTimer) / 1000.0f,
            gfx::gpu_timer_measured(&gpuTimer) / 1000.0f
        );
    }

    void update(float dt)
    {
        static const float maxTimeStep = 0.03333f;

        cpu_timer_start(&cpuTimer);
        Model::update(core::min(dt, maxTimeStep), &anim, &skel, &pose);
        cpu_timer_stop(&cpuTimer);

        ui::processCameraInput(&camera, dt);
    }

    void recompilePrograms() {}

    void resize(int width, int height)
    {
        ml::make_perspective_mat4(proj, 30.0f * FLT_DEG_TO_RAD_SCALE, (float)width/(float)height, 0.1f, 10000.0f);

        gfx::autoVars.projParams.x = proj[0].m128_f32[0];
        gfx::autoVars.projParams.y = proj[1].m128_f32[1];
        gfx::autoVars.projParams.z = proj[2].m128_f32[2];
        gfx::autoVars.projParams.w = proj[3].m128_f32[2];
    }
}
