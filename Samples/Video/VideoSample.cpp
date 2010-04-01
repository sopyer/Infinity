#include <framework.h>

void processFrame(GLuint texture, unsigned int& frameTime);
void init();
void cleanup();

#include <mmsystem.h>

class VideoSample: public UI::SDLStage
{
	public:
		GLuint textures[2];
		VideoSample()
		{
			glGenTextures(2, textures);

			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_2D, textures[1]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			init();

			processFrame(textures[0], frameTime);
			curTexture = 1; //hack ensure correct behaviour in first frame
		}
		
		~VideoSample()
		{
			cleanup();
			glDeleteTextures(2, textures);
		}
	protected:
		unsigned int baseTime, timeOfNextFrame, curTexture, frameTime;
		virtual void onPaint()
		{
			static int firstTime = true;
			
			if (firstTime)
			{
				timeOfNextFrame = baseTime = timeGetTime();
				firstTime = false;
			}

			if (timeGetTime()>=timeOfNextFrame)
			{
				printf("%d %d\n", timeGetTime(), timeOfNextFrame);
				processFrame(textures[curTexture], frameTime);
				timeOfNextFrame = baseTime+frameTime;
				curTexture = 1-curTexture;
			}

			const GLfloat w = 7.04f, h = 3.96f;

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0, 0, -5);
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textures[curTexture]);
			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
				glVertex2f(-w/2,  h/2);
				glTexCoord2f(1, 0);
				glVertex2f( w/2,  h/2);
				glTexCoord2f(1, 1);
				glVertex2f( w/2, -h/2);
				glTexCoord2f(0, 1);
				glVertex2f(-w/2, -h/2);
			glEnd();
		}

	private:
		VFS				mVFS;
};

extern "C" int main(int argc, char** argv)
{
	ui::init();
	{
		VideoSample app;
		app.run();
	}
	ui::cleanup();

	return 0;
}