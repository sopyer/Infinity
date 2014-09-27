#include <fwk/fwk.h>
#include <al/al.h>
#include <al/alc.h>

namespace app
{
    ALCdevice*     mAudioDevice;
    ALCcontext*    mAudioContext;
    media_player_t player;

    void init()
    {
        mediaInit();

        mAudioDevice  = alcOpenDevice(NULL);
        mAudioContext = alcCreateContext(mAudioDevice, NULL);

        alcMakeContextCurrent(mAudioContext);

        player = mediaCreatePlayer("d:\\bin\\Shadowgrounds\\Data\\Videos\\logo.wmv");
    }

    void fini()
    {
        mediaDestroyPlayer(player);
        mediaShutdown();

        alcDestroyContext(mAudioContext);
        alcCloseDevice(mAudioDevice);
    }

    void render()
    {
        static bool firstTime = true;
        if (firstTime)
        {
            mediaStartPlayback(player);
            firstTime = false;
        }

        mediaPlayerUpdate(player);

        gfx::set2DStates();

        v128 mv[4];
        ml::make_identity_mat4(mv);
        gfx::setModelViewMatrix(mv);
        gfx::setProjectionMatrix(mv);

        {
            gfx::setStdProgram(gfx::STD_FEATURE_COLOR);
            gfx::setMVP();

            glBindVertexArray(vf::p2cu4_vertex_t::vao);
            glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p2cu4_vertex_t));

            GLuint baseVertex;
            vf::p2cu4_vertex_t* v = gfx::frameAllocVertices<vf::p2cu4_vertex_t>(6, &baseVertex);

            vf::set(v++, -1.0f,  1.0f, 0xFF000000);
            vf::set(v++,  1.0f,  1.0f, 0xFF000000);
            vf::set(v++, -1.0f, -0.7f, 0xFF330000);
            vf::set(v++,  1.0f, -0.7f, 0xFF330000);
            vf::set(v++, -1.0f, -1.0f, 0xFF330000);
            vf::set(v++,  1.0f, -1.0f, 0xFF330000);

            glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 6);
        }

        gfx::setUIMatrices();

        mediaPlayerPrepareRender(player);
        gfx::setMVP();

        {
            float vidw = 640;
            float vidh = 320;
            float x0 = gfx::width / 2.0f - vidw / 2.0f;
            float x1 = gfx::width / 2.0f + vidw / 2.0f;
            float ym = gfx::height / 2.0f;

            glBindVertexArray(vf::p2uv2cu4_vertex_t::vao);
            glBindVertexBuffer(0, gfx::dynBuffer, 0, sizeof(vf::p2uv2cu4_vertex_t));

            GLuint baseVertex;
            vf::p2uv2cu4_vertex_t* v = gfx::frameAllocVertices<vf::p2uv2cu4_vertex_t>(8, &baseVertex);

            vf::set(v++, x0, ym - vidh, 0.0f, 0.0f, 0xFFFFFFFF);
            vf::set(v++, x1, ym - vidh, 1.0f, 0.0f, 0xFFFFFFFF);
            vf::set(v++, x0, ym,        0.0f, 1.0f, 0xFFFFFFFF);
            vf::set(v++, x1, ym,        1.0f, 1.0f, 0xFFFFFFFF);
            glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 4);
            baseVertex += 4;

            vf::set(v++, x0, ym,        0.0f, 1.0f, 0x4CFFFFFF);
            vf::set(v++, x1, ym,        1.0f, 1.0f, 0x4CFFFFFF);
            vf::set(v++, x0, ym + vidh, 0.0f, 0.0f, 0x00FFFFFF);
            vf::set(v++, x1, ym + vidh, 1.0f, 0.0f, 0x00FFFFFF);
            glDrawArrays(GL_TRIANGLE_STRIP, baseVertex, 4);
        }

        const GLfloat w = 640.0f/2.0f/2.0f, h = 360.0f/2.0f;
    }

    void update(float){}
    void resize(int, int) {}
    void recompilePrograms() {}
}
