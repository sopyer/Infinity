#include "VideoPlaybackEngine.h"
#include <gl\glee.h>

VideoPlaybackEngine engine;

void init()
{
	engine.init();
}

void cleanup()
{
	engine.cleanup();
}

void processFrame(GLuint texture, unsigned int& frameTime)
{
	engine.decodeVideoTask(texture, frameTime);
}
