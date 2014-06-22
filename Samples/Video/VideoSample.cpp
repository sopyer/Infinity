#include <framework.h>
#include <al/al.h>
#include <al/alc.h>

#include <media_api.h>

class VideoSample: public ui::Stage
{
public:
    ALCdevice*	mAudioDevice;
    ALCcontext*	mAudioContext;
    media_player_t player;

    VideoSample()
    {
        mediaInit();

        mAudioDevice  = alcOpenDevice(NULL);
        mAudioContext = alcCreateContext(mAudioDevice, NULL);

        alcMakeContextCurrent(mAudioContext);

        player = mediaCreatePlayer("d:\\bin\\Shadowgrounds\\Data\\Videos\\logo.wmv");
    }

    ~VideoSample()
    {
        mediaDestroyPlayer(player);
        mediaShutdown();

        alcDestroyContext(mAudioContext);
        alcCloseDevice(mAudioDevice);
    }
protected:
    virtual void onPaint()
    {
        static bool firstTime = true;
        if (firstTime)
        {
            mediaStartPlayback(player);
            firstTime = false;
        }

        mediaPlayerUpdate(player);

        const GLfloat w = 6.4f/2.0f/2.0f, h = 3.6f/2.0f;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);

        glBegin(GL_QUADS);

        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

        glVertex2f(-1.0f,  1.0f);
        glVertex2f( 1.0f,  1.0f);

        glColor4f(0.0f, 0.0f, 0.2f, 1.0f);

        glVertex2f( 1.0f, -0.7f);
        glVertex2f(-1.0f, -0.7f);

        glVertex2f(-1.0f, -0.7f);
        glVertex2f( 1.0f, -0.7f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);

        glEnd();

        glEnable(GL_DEPTH_TEST);

        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glTranslatef(0, 0, -5);
        glScalef(w, h, 0.0f);

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mediaPlayerPrepareRender(player);//Rename to SetTextures

        glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, 1.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 0.0f);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  0.0f);
        glEnd();

        glDisable(GL_BLEND);
    }
};

extern "C" int main(int argc, char** argv)
{
    fwk::init(argv[0]);

    {
        VideoSample app;
        app.run();
    }

    fwk::fini();

    return 0;
}