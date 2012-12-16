#ifndef __UI_H_INCLUDED__
#	define __UI_H_INCLUDED__

#include <sigslot.h>
#include <opengl.h>

#include <utils/vfs++.h>
#include <Singleton.h>
#include <FirstPersonCamera.h>
#include <VG\VG.h>
#include <Timer.h>

#include <Events.h>

class  SpectatorCamera;
struct CameraDirector;

namespace ui
{
	void init();
    void update();
	void cleanup();

    bool keyIsPressed  (int key);
    bool keyWasPressed (int key);
    bool keyIsReleased (int key);
    bool keyWasReleased(int key);

    bool mouseIsPressed  (int button);
    bool mouseWasPressed (int button);
    bool mouseIsReleased (int button);
    bool mouseWasReleased(int button);

    void mouseRelOffset(int* dx, int* dy);
    void mouseAbsOffset(int*  x, int*  y);

    void processCameraInput(SpectatorCamera* camera, float dt);
    void processCameraDirectorInput(CameraDirector* camDirector, SpectatorCamera* camera);

    void displayStats(float x, float y, float w, float h, float cpuTime, float gpuTime);

	extern vg::Font defaultFont;
}

namespace ui
{
	class Actor;
	class Container;
	class Stage;
}

#	include <Actor.h>
#	include <Container.h>
#	include <Stage.h>
#	include <Controls.h>

#endif