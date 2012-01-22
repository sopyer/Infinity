#include <framework.h>
#include <al/al.h>
#include <al/alc.h>
#include <SOIL/SOIL.h>

#include <media_api.h>

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
            //player = mediaCreatePlayer("L:\\Video\\Clips\\My Soul, Your Beats.flv");
            mediaStartPlayback(player);
		}
		
		ctx::Context	mBase, mCtx[2];
		
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
            mediaPlayerUpdate(player);
            mediaPlayerRender(player);
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