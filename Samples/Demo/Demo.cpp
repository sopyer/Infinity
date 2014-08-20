#include <fwk/fwk.h>

namespace app
{
    SpectatorCamera     camera;
    v128                proj[4];

    cpu_timer_t        cpuTimer;
    gfx::gpu_timer_t   gpuTimer;

    void init()
    {
        camera.acceleration.x = camera.acceleration.y = camera.acceleration.z = 150;
        camera.maxVelocity.x  = camera.maxVelocity.y  = camera.maxVelocity.z  =  60;

        gfx::gpu_timer_init(&gpuTimer);
    }

    void fini()
    {
        gfx::gpu_timer_fini(&gpuTimer);
    }

    void render()
    {
        gpu_timer_start(&gpuTimer);
        cpu_timer_start(&cpuTimer);

        glClearDepth(1.0);

        glFrontFace(GL_CCW);
        glCullFace (GL_BACK);
        glEnable   (GL_CULL_FACE);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadMatrixf((float*)proj);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        v128 m[4];
        camera.getViewMatrix(m);

        memcpy(&gfx::autoVars.matMV, (float*)m, sizeof(float) * 16);
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

        gfx::drawXZGrid(-500.0f, -500.0f, 500.0f, 500.0f, 40, vi_set(0.0f, 1.0f, 0.0f, 1.0f));

        ui::displayStats(
            10.0f, 10.0f, 300.0f, 70.0f,
            cpu_timer_measured(&cpuTimer) / 1000.0f,
            gfx::gpu_timer_measured(&gpuTimer) / 1000.0f
        );

        cpu_timer_stop(&cpuTimer);
        gpu_timer_stop(&gpuTimer);
    }

    void update(float dt)
    {
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
