#include <framework.h>
#include <al/al.h>
#include <al/alc.h>
#include <SOIL/SOIL.h>

void processFrame(GLuint& texture, ALuint& buffer, unsigned int& frameTime);
void init(ctx::Context context);
void init(GLsizei numTextures, GLuint* textures, ALsizei numBuffers, ALuint* buffers);
void cleanup();
void getFrame(GLuint& texture, unsigned int& frameTime);
void getAudio(ALuint& buffer);

#include <mmsystem.h>

const int NUM_BUFFERS = 3;
const int NUM_TEXTURES = 3;

int	SOIL_save_OGL_texture
	(
		const char *filename,
		int image_type,
		int channels,
		unsigned int texture_id
	)
{
	if (texture_id)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	GLenum err;
	int width, height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	err = glGetError();
	assert(err == GL_NO_ERROR);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	err = glGetError();
	assert(err == GL_NO_ERROR);
	unsigned char* p = new unsigned char[width*height*4];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, p);
	err = glGetError();
	assert(err == GL_NO_ERROR);
	SOIL_save_image(filename, image_type, width, height, channels, p);		
	delete [] p;

	return glGetError()==GL_NO_ERROR;
}


class VideoSample: public UI::SDLStage
{
	public:
		GLuint		textures[NUM_TEXTURES];
		ALCdevice*	mAudioDevice;
		ALCcontext*	mAudioContext;
		ALuint		mSource;
		ALuint		mTexture;
		ALuint		mBuffers[NUM_BUFFERS];

		VideoSample():mTexture(0)
		{
			mAudioDevice  = alcOpenDevice(NULL);
			mAudioContext = alcCreateContext(mAudioDevice, NULL);
			
			alcMakeContextCurrent(mAudioContext);

			alGenSources(1, &mSource);
			alGenBuffers(NUM_BUFFERS, mBuffers);

			mBase = ctx::getCurrent();
			ctx::cloneCurrent(2, true, mCtx);
			ctx::makeCurrent(mCtx[0]);

			glGenTextures(NUM_TEXTURES, textures);
			
			init(mCtx[1]);
			glGenTextures(NUM_TEXTURES, textures);
			init(NUM_TEXTURES, textures, NUM_BUFFERS, mBuffers);
			Sleep(1000);
			ALuint buffer=0;
			processFrame(mTexture, buffer, frameTime);
			if (buffer)
			{
				alSourceQueueBuffers(mSource, 1, &buffer);
				alSourcePlay(mSource);
			}
		}
		
		ctx::Context	mBase, mCtx[2];
		
		~VideoSample()
		{
			cleanup();
			ctx::destroy(2, mCtx);
			glDeleteTextures(NUM_TEXTURES, textures);
			
			alDeleteBuffers(NUM_BUFFERS, mBuffers);
			alDeleteSources(1, &mSource);
			alcDestroyContext(mAudioContext);
			alcCloseDevice(mAudioDevice);
		}
	protected:
		unsigned int baseTime, timeOfNextFrame, curTexture, frameTime;
		virtual void onPaint()
		{
			ctx::makeCurrent(mCtx[0]);
			static int firstTime = true;
			
			if (firstTime)
			{
				timeOfNextFrame = baseTime = timeGetTime();
				firstTime = false;
			}
			else if (timeGetTime()>=timeOfNextFrame)
			{
				ALuint buffer = 0;
				ALint count;

				alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &count);
				if (count>0)
				{
					alSourceUnqueueBuffers(mSource, 1, &buffer);
				}
				
				processFrame(mTexture, buffer, frameTime);

				if (buffer)
					alSourceQueueBuffers(mSource, 1, &buffer);
				
				//getFrame(mTexture, frameTime);
				timeOfNextFrame = baseTime+frameTime;
			}
			
			//ALuint buffer = 0;
			//ALint count;

			//alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &count);
			//if (count>0)
			//{
			//	alSourceUnqueueBuffers(mSource, 1, &buffer);
			//	getAudio(buffer);
			//	//if (buffer)
			//		alSourceQueueBuffers(mSource, 1, &buffer);
			//}
			
			ALint state, bufCount;
			alGetSourcei(mSource, AL_BUFFERS_QUEUED, &bufCount);
			alGetSourcei(mSource, AL_SOURCE_STATE, &state);
			if (state!=AL_PLAYING && bufCount>0)
				alSourcePlay(mSource);

			const GLfloat w = 6.4f*1.1f, h = 3.6f*1.1f;

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0, 0, -5);
			
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glEnable(GL_TEXTURE_2D);
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
			ctx::makeCurrent(mBase);
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