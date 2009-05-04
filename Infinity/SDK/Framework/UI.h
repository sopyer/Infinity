#ifndef __UI_H_INCLUDED__
#	define __UI_H_INCLUDED__

#include <sigslot.h>
#include <gl\glfw.h>
#include <gl\glew.h>

#include <..\vfs\vfs.h>
#include <glClasses.h>
#include <Singleton.h>
#include <FirstPersonCamera.h>
#include <TextureManager.h>
#include <ProgramManager.h>
#include <ShaderManager.h>
#include <FontManager.h>
#include <VG\VG.h>
#include <utils.h>
#include <Timer.h>

#include <Events.h>

namespace UI
{
	class Actor;
	class Container;
	class Stage;
}

#	include <Actor.h>
#	include <Container.h>
#	include <Stage.h>



#endif