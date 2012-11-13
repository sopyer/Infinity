#ifndef _FRAMEWORK_H_INCLUDED_
#	define _FRAMEWORK_H_INCLUDED_

#include <opengl.h>

#include <UI.h>
#include <ResourceHelpers.h>
#include <Context.h>
#include <profiler.h>

//TODO: make shorter e.g. log
namespace logging
{
	void message(const char *s,...);
}

class SpectatorCamera;

void processCameraInput(SpectatorCamera* camera, float dt);

#endif