#include <fwk/fwk.h>
#include <fwk/SpectatorCamera.h>
#include <fwk/CameraDirector.h>
#include "ShaderEditOverlay.h"
#include "ProfilerOverlay.h"

#include <gfx/gfx_res.h>

void Platform_Initialise();
void Platform_Finalise();

namespace ui
{
#include "RainbowTables.h"

    struct Area
    {
        GLfloat x0, y0, x1, y1;
    };

    static const uint32_t ID_CHECKBOX = 0x01000000;
    static const uint32_t ID_MASK     = 0xFF000000;

    int width;
    int height;
    int fullscreen;

    int   mouseX;
    int   mouseY;
    int   deltaX;
    int   deltaY;
    uint8_t  keyStatePrev[SDL_NUM_SCANCODES];
    uint8_t  keyStateCurr[SDL_NUM_SCANCODES];
    uint32_t mouseStatePrev;
    uint32_t mouseStateCurr;
    bool     mouseWheelUp;
    bool     mouseWheelDown;

    ShaderEditOverlay*  overlayShaderEdit;
    ProfilerOverlay*    overlayProfiler;

    enum
    {
        STATE_DEFAULT,
        STATE_SHADER_EDIT,
        STATE_PROFILER
    };

    enum
    {
        PROF_STATE_NO_CAPTURE,
        PROF_STATE_FRAME_CAPTURE,
        PROF_STATE_TIMESLICE_CAPTURE,
        PROF_STATE_DATA_RETRIEVAL
    };

    size_t uiState;
    size_t profilerState;

    struct CheckBox
    {
        float x0, y0, x1, y1;
        uint32_t id:24;
        int      checked:1;
        int      hover:1;
    };

    static const size_t MAX_CHECKBOX_COUNT = 16;

    size_t   checkBoxCount;
    CheckBox checkBoxes[MAX_CHECKBOX_COUNT];

    void checkBoxUpdateAll();
    void checkBoxRenderAll();

    int      findAreaByID(uint32_t id);
    uint32_t findAreaID  (const point_t& p);

    void processInputEvents();

    void reset();

    void init(int w, int h)
    {
        //!!!! Refactor
        fullscreen = false;
        width      = w;
        height     = h;

        reset();

        overlayShaderEdit = new ShaderEditOverlay;
        overlayShaderEdit->initialise(width, height);

        overlayProfiler = new ProfilerOverlay;
        overlayProfiler->init(width, height);

        GLuint programs[] = {
            impl::simpleUIProgram, impl::stencilCubicAreaAAProgram,
            impl::stencilCubicAreaProgram, impl::linGradProgram
        };

        overlayShaderEdit->addPrograms(ARRAY_SIZE(programs), programs);

        //TODO: HACK!!!!!
        SDL_StartTextInput();

    }

    void fini()
    {
        delete overlayShaderEdit;
        overlayProfiler->fini();
        delete overlayProfiler;
        Platform_Finalise();

        reset();
    }

    void reset()
    {
        memset(keyStateCurr, 0, SDL_NUM_SCANCODES);
        memset(keyStatePrev, 0, SDL_NUM_SCANCODES);

        mouseWheelUp = mouseWheelDown = false;

        mouseStatePrev = 0;
        mouseStateCurr = 0;

        mouseX = mouseY = 0;
        deltaX = deltaY = 0;

        checkBoxCount = 0;

        uiState       = STATE_DEFAULT;
        profilerState = PROF_STATE_NO_CAPTURE;
    }

    void resize(int w, int h)
    {
        width  = w;
        height = h;
    }

    void update(float dt)
    {
        PROFILER_CPU_TIMESLICE("ui->update");

        memcpy(keyStatePrev, keyStateCurr,               SDL_NUM_SCANCODES);
        memcpy(keyStateCurr, SDL_GetKeyboardState(NULL), SDL_NUM_SCANCODES);

        mouseStatePrev = mouseStateCurr;
        mouseStateCurr = SDL_GetMouseState(&mouseX, &mouseY);
        SDL_GetRelativeMouseState(&deltaX, &deltaY);

        mouseWheelUp = mouseWheelDown = false;

        processInputEvents();

        if (profilerState == PROF_STATE_DATA_RETRIEVAL)
        {
            profilerState = PROF_STATE_NO_CAPTURE;
            overlayProfiler->loadProfilerData();
        }
        else if (
            profilerState==PROF_STATE_TIMESLICE_CAPTURE &&
            ui::keyIsPressed(SDL_SCANCODE_GRAVE) &&
            ui::keyWasReleased(SDL_SCANCODE_T)
        )
        {
            profilerState = PROF_STATE_DATA_RETRIEVAL;
            profilerStopCapture();
        }
        else if (profilerState == PROF_STATE_FRAME_CAPTURE)
        {
            profilerState = PROF_STATE_DATA_RETRIEVAL;
            profilerStopCapture();
        }
        else if (
            profilerState==PROF_STATE_NO_CAPTURE &&
            ui::keyIsPressed(SDL_SCANCODE_GRAVE) &&
            ui::keyWasReleased(SDL_SCANCODE_T)
        )
        {
            profilerState = PROF_STATE_TIMESLICE_CAPTURE;
            profilerStartCapture();
        }
        else if (
            profilerState==PROF_STATE_NO_CAPTURE &&
            ui::keyIsPressed(SDL_SCANCODE_GRAVE) &&
            ui::keyWasReleased(SDL_SCANCODE_F)
        )
        {
            profilerState = PROF_STATE_FRAME_CAPTURE;
            profilerStartCapture();
        }
        else if (
            profilerState!=PROF_STATE_NO_CAPTURE &&
            profilerState!=PROF_STATE_TIMESLICE_CAPTURE
        )
        {
            assert(!"Invalid state");
        }

        if (uiState==STATE_PROFILER)
        {
            PROFILER_CPU_TIMESLICE("mProfilerOverlay->updateUI");
            overlayProfiler->updateUI(dt);
        }
        else if (uiState==STATE_DEFAULT)
        {
            PROFILER_CPU_TIMESLICE("onUpdate");
        }
        if (overlayShaderEdit->requireReset())
        {
            PROFILER_CPU_TIMESLICE("onShaderRecompile");
            fwk::recompilePrograms();
        }

        checkBoxUpdateAll();
    }

    void render()
    {
        if (uiState==STATE_PROFILER)
        {
            PROFILER_CPU_TIMESLICE("overlayProfiler->renderFullscreen");
            overlayProfiler->renderFullscreen();
        }
        if (uiState==STATE_SHADER_EDIT)
        {
            PROFILER_CPU_TIMESLICE("overlayShaderEdit->renderFullscreen");
            overlayShaderEdit->renderFullscreen();
        }
        checkBoxRenderAll();
    }

    void refineEvents(refined_events_t* events, uint32_t activeArea)
    {
        assert(events);

        events->eventMask &= ~(EVENT_LEFT_CLICK|EVENT_ACTIVATED);

        if (mouseWasPressed(SDL_BUTTON_LEFT))
        {
            events->eventMask |= EVENT_ACTIVATED|EVENT_ACTIVE;
            events->activeArea = activeArea;
        }
        else if (mouseWasReleased(SDL_BUTTON_LEFT))
        {
            if (activeArea == events->activeArea)
                events->eventMask |= EVENT_LEFT_CLICK;
            else
                events->activeArea = INVALID_ID;

            events->eventMask &= ~(EVENT_ACTIVE);
        }
        else if (!mouseIsPressed(SDL_BUTTON_LEFT))
        {
            events->activeArea = activeArea;
        }
    }

    void mouseCapture()
    {
        SDL_ShowCursor(FALSE);
        SDL_SetWindowGrab(fwk::window, SDL_TRUE);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    void mouseRelease()
    {
        SDL_ShowCursor(TRUE);
        SDL_SetWindowGrab(fwk::window, SDL_FALSE);
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    bool mouseIsCaptured()
    {
        return SDL_GetWindowGrab(fwk::window) == SDL_TRUE;
    }

    void processInputEvents()
    {
        PROFILER_CPU_TIMESLICE("ui::processSDLEvents");
        SDL_Event   E;
        while (SDL_PollEvent(&E))
        {
            switch(E.type)
            {
                case SDL_QUIT:
                    fwk::exit();
                    break;
                case SDL_MOUSEWHEEL:
                    mouseWheelUp   = E.wheel.y > 0;
                    mouseWheelDown = E.wheel.y < 0;
                    break;
                case SDL_TEXTINPUT:
                     if (uiState==STATE_SHADER_EDIT) overlayShaderEdit->inputText(E.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (uiState)
                    {
                        case STATE_SHADER_EDIT:
                            overlayShaderEdit->handleKeyDown(E.key);
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    if (E.key.keysym.sym==SDLK_F4 && ((E.key.keysym.mod & ~(KMOD_NUM|KMOD_CAPS|KMOD_MODE) & KMOD_LALT) != 0))
                    {
                        fwk::exit();
                    }
                    else if (E.key.keysym.sym==SDLK_F5 && ((E.key.keysym.mod & ~(KMOD_NUM|KMOD_CAPS|KMOD_MODE)) == 0))
                    {
                        uiState = uiState==STATE_SHADER_EDIT?STATE_DEFAULT:STATE_SHADER_EDIT;
                        if (uiState==STATE_SHADER_EDIT) overlayShaderEdit->reset();
                    }
                    else if (E.key.keysym.sym==SDLK_F4 && ((E.key.keysym.mod & ~(KMOD_NUM|KMOD_CAPS|KMOD_MODE)) == 0))
                    {
                        uiState = uiState==STATE_PROFILER?STATE_DEFAULT:STATE_PROFILER;
                    }
                    break;
            }
        }
    }

    void addPrograms(size_t count, GLuint* programs)
    {
        overlayShaderEdit->addPrograms(count, programs);
    }

    bool mouseWasWheelUp()
    {
        return mouseWheelUp;
    }

    bool mouseWasWheelDown()
    {
        return mouseWheelDown;
    }

    bool keyIsPressed  (int key)
    {
        assert(key<SDL_NUM_SCANCODES);
        return keyStateCurr[key] != 0;
    }

    bool keyWasPressed (int key)
    {
        assert(key<SDL_NUM_SCANCODES);
        return !keyStatePrev[key] && keyStateCurr[key];
    }

    bool keyIsReleased (int key)
    {
        assert(key<SDL_NUM_SCANCODES);
        return !keyStateCurr[key];
    }

    bool keyWasReleased(int key)
    {
        assert(key<SDL_NUM_SCANCODES);
        return keyStatePrev[key] && !keyStateCurr[key];
    }

    bool mouseIsPressed  (int button)
    {
        return (mouseStateCurr&SDL_BUTTON(button)) != 0;
    }

    bool mouseWasPressed (int button)
    {
        return !(mouseStatePrev&SDL_BUTTON(button)) && (mouseStateCurr&SDL_BUTTON(button));
    }

    bool mouseIsReleased (int button)
    {
        return !(mouseStateCurr&SDL_BUTTON(button));
    }

    bool mouseWasReleased(int button)
    {
        return (mouseStatePrev&SDL_BUTTON(button)) && !(mouseStateCurr&SDL_BUTTON(button));
    }

    void mouseRelOffset(int* dx, int* dy)
    {
        if (dx) *dx = deltaX;
        if (dy) *dy = deltaY;
    }

    void mouseAbsOffset(int* x, int* y)
    {
        if (x) *x = mouseX;
        if (y) *y = mouseY;
    }

    void processCameraInput(SpectatorCamera* camera, float dt)
    {
        if (uiState != STATE_DEFAULT)
        {
            if (mouseIsCaptured()) mouseRelease();
            return;
        }

        bool LCTRL_Down_LALT_Released     = keyIsPressed  (SDL_SCANCODE_LCTRL) && keyWasReleased(SDL_SCANCODE_LALT );
        bool LALT_Down_LCTRL_Released     = keyIsPressed  (SDL_SCANCODE_LALT ) && keyWasReleased(SDL_SCANCODE_LCTRL);
        bool LALT_Released_LCTRL_Released = keyWasReleased(SDL_SCANCODE_LALT ) && keyWasReleased(SDL_SCANCODE_LCTRL);

        bool actionReleaseMouse = LCTRL_Down_LALT_Released || LALT_Down_LCTRL_Released || LALT_Released_LCTRL_Released;

        if (mouseWasPressed(SDL_BUTTON_LEFT))
        {
            mouseCapture();
        }
        else if (mouseIsCaptured() && actionReleaseMouse)
        {
            mouseRelease();
        }
        else if (mouseIsCaptured())
        {
            float   dx = 0.0f, dy = 0.0f, dz = 0.0f;
            float   heading = 0.0f;
            float   pitch   = 0.0f;

            dx += ui::keyIsPressed(SDL_SCANCODE_D)?1.0f:0.0f;
            dx -= ui::keyIsPressed(SDL_SCANCODE_A)?1.0f:0.0f;
            dy += ui::keyIsPressed(SDL_SCANCODE_E)?1.0f:0.0f;
            dy -= ui::keyIsPressed(SDL_SCANCODE_Q)?1.0f:0.0f;
            dz += ui::keyIsPressed(SDL_SCANCODE_W)?1.0f:0.0f;
            dz -= ui::keyIsPressed(SDL_SCANCODE_S)?1.0f:0.0f;

            pitch   += ui::keyIsPressed(SDL_SCANCODE_KP_8)?1.50f:0.0f;
            pitch   -= ui::keyIsPressed(SDL_SCANCODE_KP_5)?1.50f:0.0f;
            heading += ui::keyIsPressed(SDL_SCANCODE_KP_4)?1.50f:0.0f;
            heading -= ui::keyIsPressed(SDL_SCANCODE_KP_6)?1.50f:0.0f;

            camera->rotateSmoothly(heading * FLT_DEG_TO_RAD_SCALE, pitch * FLT_DEG_TO_RAD_SCALE);
            camera->rotateSmoothly((float)-ui::deltaX * FLT_DEG_TO_RAD_SCALE, (float)-ui::deltaY * FLT_DEG_TO_RAD_SCALE);
            camera->updatePosition(dx, dy, dz, dt);
        }
    }

    void displayStats(float x, float y, float w, float h, float cpuTime, float gpuTime)
    {
        char* gpuName;
        char  str[256];

        vg::drawRoundedRect(x, y, x+w, y+h, 5, 5, 0xC0000000, 0xFF00FF32);

        glColor3f(1.0f, 1.0f, 1.0f);

        gpuName = (char*)glGetString(GL_RENDERER);
        vg::drawString(vg::defaultFont, x+15.0f, y+18.0f, gpuName, strlen(gpuName));

        _snprintf(str, 256, "CPU time - %f ms", cpuTime);
        vg::drawString(vg::defaultFont, x+15.0f, y+37.0f, str, strlen(str));

        _snprintf(str, 256, "GPU time - %f ms", gpuTime);
        vg::drawString(vg::defaultFont, x+15.0f, y+55.0f, str, strlen(str));
    }

    void processCameraDirectorInput(CameraDirector* camDirector, SpectatorCamera* camera)
    {
        assert(camDirector && camera);

        if (ui::keyWasReleased(SDL_SCANCODE_0))
        {
            ml::vec4 p, q;

            vi_storeu_v4(&p, camera->getPosition());
            vi_storeu_v4(&q, camera->getOrientation());

            camDirector->savedCamRotation.push_back(q);
            camDirector->savedCamLocation.push_back(p);
        }

        if (ui::keyWasReleased(SDL_SCANCODE_EQUALS) &&
            ui::keyIsPressed(SDL_SCANCODE_LALT)     &&
            camDirector->savedCamRotation.size())
        {
            int max = camDirector->savedCamRotation.size() - 1;
            int idx = camDirector->index;

            idx = idx + 1;
            idx = idx > max ? 0 : idx;

            camDirector->index = idx;

            camera->setOrientation(vi_loadu_v4(&camDirector->savedCamRotation[camDirector->index]));
            camera->setPosition   (vi_loadu_v4(&camDirector->savedCamLocation[camDirector->index]));
        }

        if (ui::keyWasReleased(SDL_SCANCODE_MINUS) &&
            ui::keyIsPressed(SDL_SCANCODE_LALT)    &&
            camDirector->savedCamRotation.size())
        {
            int max = camDirector->savedCamRotation.size() - 1;
            int idx = camDirector->index;

            idx = idx - 1;
            idx = idx < 0 ? max : idx;

            camDirector->index = idx;

            camera->setOrientation(vi_loadu_v4(&camDirector->savedCamRotation[camDirector->index]));
            camera->setPosition   (vi_loadu_v4(&camDirector->savedCamLocation[camDirector->index]));
        }
    }

    void processZoomAndPan(float& scale, float& transX, float& transY, bool& drag)
    {
        int x,  y;
        int rx, ry;

        ui::mouseAbsOffset(&x, &y);
        ui::mouseRelOffset(&rx, &ry);
        if (ui::mouseWasWheelUp())
        {
            scale *= 1.2f;
            transX -= (x-transX)*(1.2f - 1);
            transY -= (y-transY)*(1.2f - 1);
        }
        if (ui::mouseWasWheelDown())
        {
            scale /= 1.2f;
            if (scale<1.0f)
            {
                //fix it a bit
                transX -= (x-transX)*(1/scale/1.2f - 1);
                transY -= (y-transY)*(1/scale/1.2f - 1);
                scale = 1.0f;
            }
            else
            {
                transX -= (x-transX)*(1/1.2f - 1);
                transY -= (y-transY)*(1/1.2f - 1);
            }
        }
        if (ui::mouseWasPressed(SDL_BUTTON_LEFT))
        {
            drag = true;
        }
        if (ui::mouseWasReleased(SDL_BUTTON_LEFT))
        {
            drag = false;
        }
        if (drag)
        {
            transX += rx;
            transY += ry;
        }
    }

    CheckBoxID checkBoxAdd(float x0, float y0, float x1, float y1, int checked)
    {
        assert(checkBoxCount<MAX_CHECKBOX_COUNT);

        CheckBoxID id = checkBoxCount;

        checkBoxes[id].x0 = x0;
        checkBoxes[id].y0 = y0;
        checkBoxes[id].x1 = x1;
        checkBoxes[id].y1 = y1;

        checkBoxes[id].id      = id;
        checkBoxes[id].checked = checked;

        checkBoxCount++;

        return id;
    }

    int checkBoxIsChecked(CheckBoxID id)
    {
        return checkBoxes[id].checked;
    }

    void checkBoxUpdateAll()
    {
        for (size_t i = 0; i < checkBoxCount; ++i)
        {
            if (testPtInRect((float)mouseX, (float)mouseY, checkBoxes[i].x0, checkBoxes[i].y0, checkBoxes[i].x1, checkBoxes[i].y1))
            {
                checkBoxes[i].hover = true;
                if (mouseWasReleased(SDL_BUTTON_LEFT))
                {
                    checkBoxes[i].checked = !checkBoxes[i].checked;
                }
            }
            else
            {
                checkBoxes[i].hover = false;
            }
        }
    }

    void checkBoxRenderAll()
    {
        for (size_t i=0; i<checkBoxCount; ++i)
        {
            static const uint32_t colors[] = {
                0xFF232563,
                0xFF4547A5,
                0xFF3CE484,
                0xFF7FFDA3,
            };

            bool isChecked = checkBoxes[i].checked != 0;
            bool isHover   = checkBoxes[i].hover   != 0;

            float x0 = checkBoxes[i].x0;
            float y0 = checkBoxes[i].y0;
            float x1 = checkBoxes[i].x1;
            float y1 = checkBoxes[i].y1;

            vg::drawRoundedRect(x0, y0, x0+16.0f, y0+16.0f, 2.5f, 2.5f, colors[(isChecked<<1)+isHover], 0xFF00FF32);
        }
    }
}
