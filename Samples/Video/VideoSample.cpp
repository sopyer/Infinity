#include <framework.h>
#include <al/al.h>
#include <al/alc.h>

#include <media_api.h>

const char srcVideoOut[] = 
    "uniform sampler2D samY;                                                                        \n"
    "uniform sampler2D samU;                                                                        \n"
    "uniform sampler2D samV;                                                                        \n"
    "                                                                                               \n"
    "void main()                                                                                    \n"
    "{                                                                                              \n"
    "    float y   = texture2D(samY, gl_TexCoord[0].xy).r;                                          \n"
    "    float u   = texture2D(samU, gl_TexCoord[0].xy).r - 0.5;                                    \n"
    "    float v   = texture2D(samV, gl_TexCoord[0].xy).r - 0.5;                                    \n"
    "                                                                                               \n"
    "    vec3 rgb = vec3(y) + u*vec3(0.0, -0.344, 1.770) + v*vec3(1.403, -0.714, 0.0);              \n"
    "                                                                                               \n"
    "    gl_FragColor = vec4(rgb, 1.0)*gl_Color;                                                    \n"
    "}                                                                                              \n";

GLuint progVideoOut;


class VideoSample: public ui::SDLStage
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

            player = mediaCreatePlayer("k:\\temp\\ShadowgroundsData\\Data\\Videos\\logo.wmv");

            GLint uni;

            progVideoOut = resources::createProgram(GL_FRAGMENT_SHADER, srcVideoOut);

            glUseProgram(progVideoOut);
            uni = glGetUniformLocation(progVideoOut, "samY"); glUniform1i(uni, 0);
            uni = glGetUniformLocation(progVideoOut, "samU"); glUniform1i(uni, 1);
            uni = glGetUniformLocation(progVideoOut, "samV"); glUniform1i(uni, 2);
            glUseProgram(0);

        }
		
		ctx::Context	mBase, mCtx[2];
		
		~VideoSample()
		{
            glDeleteProgram(progVideoOut);

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
		    glUseProgram(progVideoOut);

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

	private:
		VFS				mVFS;
};

extern "C" int main(int argc, char** argv)
{
	VideoSample app;
	app.run();

	return 0;
}