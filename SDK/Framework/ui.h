#ifndef __UI_H_INCLUDED__
#	define __UI_H_INCLUDED__

#include <opengl.h>
#include <vg/vg.h>
#include <utils/utils.h>

class  SpectatorCamera;
struct CameraDirector;

namespace ui
{
    typedef uint32_t CheckBoxID;

	extern vg::Font       defaultFont;

    static const uint32_t ID_MAIN_VIEW;

    static const size_t   RAINBOW_TABLE_L_SIZE = 533;
    extern const uint32_t rainbowTableL[RAINBOW_TABLE_L_SIZE];

    static const uint32_t ID_INVALID  = 0xFFFFFFFF;

    //TODO: Refactor!!!!!!
	void     init(GLsizei width, GLsizei height); //Refactor later to add support for resolution change!!!!
    void     update();
    void     render();
	void     cleanup();

    bool     keyIsPressed  (int key);
    bool     keyWasPressed (int key);
    bool     keyIsReleased (int key);
    bool     keyWasReleased(int key);

    void     mouseCapture   ();
    void     mouseRelease   ();
    bool     mouseIsCaptured();

    bool     mouseIsPressed  (int button);
    bool     mouseWasPressed (int button);
    bool     mouseIsReleased (int button);
    bool     mouseWasReleased(int button);
    
    bool     mouseWasWheelUp  ();
    bool     mouseWasWheelDown();

    void     mouseRelOffset(int* dx, int* dy);
    void     mouseAbsOffset(int*  x, int*  y);

    void     processCameraDirectorInput(CameraDirector*  camDirector, SpectatorCamera* camera);
    void     processCameraInput        (SpectatorCamera* camera, float dt);
    void     processZoomAndPan         (float& scale, float& transX, float& transY, bool& drag);

    void     displayStats(float x, float y, float w, float h, float cpuTime, float gpuTime);

    CheckBoxID checkBoxAdd(float x0, float y0, float x1, float y1, int checked);
    int        checkBoxIsChecked(CheckBoxID id);

    void setMouseActiveID(uint32_t id);
    bool isHighlightedID (uint32_t id);
    bool isActiveID      (uint32_t id);
    bool wasLClickedID   (uint32_t id);
}

namespace ui
{
	class Actor;
	class Container;
	class Stage;
}

#endif