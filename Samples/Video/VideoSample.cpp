#include <framework.h>
#include <al/al.h>
#include <al/alc.h>

void processFrame(GLuint texture, ALuint buffer, unsigned int& frameTime);
void init();
void cleanup();

#include <mmsystem.h>

const int NUM_BUFFERS = 3;

class VideoSample: public UI::SDLStage
{
	public:
		GLuint		textures[2];
		ALCdevice*	mAudioDevice;
		ALCcontext*	mAudioContext;
		ALuint		mSource;
		ALuint		mBuffers[NUM_BUFFERS];
		int			mUsedBuffers;

		VideoSample()
		{
			ALenum err;
			mAudioDevice  = alcOpenDevice(NULL);
			mAudioContext = alcCreateContext(mAudioDevice, NULL);
			
			alcMakeContextCurrent(mAudioContext);

			mSource = -1;
			mBuffers[0]=mBuffers[1]=-1;
			alGenSources(1, &mSource);
			alGenBuffers(NUM_BUFFERS, mBuffers);

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

			mUsedBuffers = 1;
			processFrame(textures[0], mBuffers[0], frameTime);
			alSourceQueueBuffers(mSource, 1, mBuffers);
			alSourcePlay(mSource);
			curTexture = 1; //hack ensure correct behaviour in first frame
		}
		
		~VideoSample()
		{
			cleanup();
			glDeleteTextures(2, textures);
			
			alDeleteBuffers(NUM_BUFFERS, mBuffers);
			alDeleteSources(1, &mSource);
			alcDestroyContext(mAudioContext);
			alcCloseDevice(mAudioDevice);
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
				ALuint buffer = 0;
				
				if (mUsedBuffers<NUM_BUFFERS)
				{
					buffer = mBuffers[mUsedBuffers++];
				}
				else
				{
					ALint count;
					alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &count);
					if (count>0)
					{
						alSourceUnqueueBuffers(mSource, 1, &buffer);
					}
				}

				processFrame(textures[curTexture], buffer, frameTime);
				if (buffer)
					alSourceQueueBuffers(mSource, 1, &buffer);
				timeOfNextFrame = baseTime+frameTime;
				curTexture = 1-curTexture;
			}
			
			ALint state;
			alGetSourcei(mSource, AL_SOURCE_STATE, &state);
			if (state!=AL_PLAYING)
				alSourcePlay(mSource);

			const GLfloat w = 6.4*1.1f, h = 3.6*1.1f;

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