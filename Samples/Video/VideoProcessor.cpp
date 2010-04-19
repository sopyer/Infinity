#include "VideoPlaybackEngine.h"
#include <gl/glee.h>
#include <al/al.h>

VideoPlaybackEngine engine;

void init()
{
	engine.init();
}

void cleanup()
{
	engine.cleanup();
}

void processFrame(GLuint texture, ALuint buffer, unsigned int& frameTime)
{
	engine.decodeVideoTask(texture, buffer, frameTime);
}
