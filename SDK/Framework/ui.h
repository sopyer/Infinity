#ifndef __UI_H_INCLUDED__
#	define __UI_H_INCLUDED__

#include <sigslot.h>
#include <opengl.h>

#include <..\vfs++\vfs++.h>
#include <Singleton.h>
#include <FirstPersonCamera.h>
#include <VG\VG.h>
#include <utils.h>
#include <Timer.h>

#include <Events.h>

namespace ui
{
	void init();
	void cleanup();
}

namespace ui
{
	class Actor;
	class Container;
	class Stage;

	extern vg::Font defaultFont;
}

#	include <Actor.h>
#	include <Container.h>
#	include <Stage.h>
#	include <Controls.h>

#endif