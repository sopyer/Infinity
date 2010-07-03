#include "VideoPlaybackEngine.h"
#include <gl/glee.h>
#include <al/al.h>

VideoPlaybackEngine engine;

void init(GLsizei numTextures, GLuint* textures, ALsizei numBuffers, ALuint* buffers)
{
	engine.init(numTextures, textures, numBuffers, buffers);
}

void init(ctx::Context context)
{
	engine.init(context);
}

void cleanup()
{
	engine.cleanup();
}

void getFrame(GLuint& texture, unsigned int& frameTime)
{
	engine.getFrame(texture, frameTime);
}

void getAudio(ALuint& buffer)
{
	engine.getAudio(buffer);
}

void processFrame(GLuint& texture, ALuint& buffer, unsigned int& frameTime)
{
	//engine.decodeVideoTask(texture, buffer, frameTime);
	engine.getFrame(texture, buffer, frameTime);
}
