#pragma once

#include <gfx/gfx.h>

class  SpectatorCamera;
struct CameraDirector;

namespace ui
{
    typedef uint32_t CheckBoxID;

    static const uint32_t ID_MAIN_VIEW;

    static const size_t   RAINBOW_TABLE_L_SIZE = 533;
    extern const uint32_t rainbowTableL[RAINBOW_TABLE_L_SIZE];

    static const uint32_t INVALID_ID  = 0xFFFFFFFF;

    //TODO: Refactor!!!!!!
    void     init(int width, int height); //Refactor later to add support for resolution change!!!!
    void     fini();
    void     update(float dt);
    void     render();

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

    void     debugAddPrograms(size_t count, GLuint* programs);

    CheckBoxID checkBoxAdd(float x0, float y0, float x1, float y1, int checked);
    int        checkBoxIsChecked(CheckBoxID id);

    void addPrograms(size_t count, GLuint* programs);

    enum EventType
    {
        EVENT_LEFT_CLICK = 1<<0,
        EVENT_ACTIVE     = 1<<1,
        EVENT_ACTIVATED  = 1<<2,
    };

    struct refined_events_t
    {
        uint32_t activeArea;
        uint32_t eventMask;
    };

    void refineEvents(refined_events_t* events, uint32_t activeArea);

    inline bool isAreaHighlighted(refined_events_t* events, uint32_t id);
    inline bool isAreaActive     (refined_events_t* events, uint32_t id);
    inline bool wasAreaActivated (refined_events_t* events, uint32_t id);
    
    inline uint32_t getEventArea(EventType evt);
}

/* IMPLEMENTATION */
namespace ui
{
    inline bool isAreaHighlighted(refined_events_t* events, uint32_t id)
    {
        return events->activeArea==id;
    }

    inline bool isAreaActive(refined_events_t* events, uint32_t id)
    {
        return (events->eventMask&EVENT_ACTIVE) && events->activeArea==id;
    }

    inline bool wasAreaActivated(refined_events_t* events, uint32_t id)
    {
        return (events->eventMask&EVENT_ACTIVATED) && events->activeArea==id;
    }

    inline uint32_t getEventArea(refined_events_t* events, uint32_t mask)
    {
        return (events->eventMask&mask) ? events->activeArea : INVALID_ID;
    }
}

namespace ui
{
    class Stage;
}
