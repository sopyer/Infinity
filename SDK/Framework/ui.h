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

	extern vg::Font defaultFont;

    //TODO: Refactor!!!!!!
	void     init(GLsizei width, GLsizei height); //Refactor later to add support for resolution change!!!!
    void     update();
    void     render();
	void     cleanup();

    bool     keyIsPressed  (int key);
    bool     keyWasPressed (int key);
    bool     keyIsReleased (int key);
    bool     keyWasReleased(int key);

    bool     mouseIsPressed  (int button);
    bool     mouseWasPressed (int button);
    bool     mouseIsReleased (int button);
    bool     mouseWasReleased(int button);

    uint32_t mouseOverID();
    void     captureMouse(uint32_t ID);
    void     releaseMouse();

    void     mouseAddEventArea(float x0, float y0, float x1, float y1, uint32_t id);
    void     mouseRemoveEventArea(uint32_t id);
    void     mouseUpdateEventArea(uint32_t id, float x0, float y0, float x1, float y1);
    void     mouseRelOffset(int* dx, int* dy);
    void     mouseAbsOffset(int*  x, int*  y);

    void     beginPickOutline();
    void     addPickOutlineRect(float x, float y, float w, float h, uint32_t id);
    void     endPickOutline();
    
    uint32_t pickID(GLuint x, GLuint y);

#if defined(DEBUG) || defined(_DEBUG)
    void     readPickBuffer(GLsizei sz, void* data);
#endif

    void     processCameraInput(SpectatorCamera* camera, float dt);
    void     processCameraDirectorInput(CameraDirector* camDirector, SpectatorCamera* camera);

    void     displayStats(float x, float y, float w, float h, float cpuTime, float gpuTime);

    CheckBoxID checkBoxAdd(float x0, float y0, float x1, float y1, int checked);
    int        checkBoxIsChecked(CheckBoxID id);
}

namespace ui
{
	class Actor;
	class Container;
	class Stage;
}

#endif