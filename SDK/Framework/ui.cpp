#include "UI.h"

#include "DefaultFontData.h"
#include "SpectatorCamera.h"
#include "CameraDirector.h"

namespace ui
{
    vg::Font defaultFont;

    int   mouseX;
    int   mouseY;
    int   deltaX;
    int   deltaY;
    uint8_t keyStatePrev[SDLK_LAST];
    uint8_t keyStateCur[SDLK_LAST];
    uint8_t mouseStatePrev;
    uint8_t mouseStateCur;

    void init()
    {
        defaultFont = vg::createFont(anonymousProBTTF, sizeof(anonymousProBTTF), 16);

        memcpy(keyStateCur,  SDL_GetKeyState(NULL), SDLK_LAST);
        memcpy(keyStatePrev, keyStateCur,           SDLK_LAST);

        mouseStatePrev = SDL_GetMouseState(&mouseX, &mouseY);
        mouseStateCur  = SDL_GetRelativeMouseState(&deltaX, &deltaY);
        deltaX = 0;
        deltaY = 0;
    }

    void update()
    {
        memcpy(keyStatePrev, keyStateCur,           SDLK_LAST);
        memcpy(keyStateCur,  SDL_GetKeyState(NULL), SDLK_LAST);

        mouseStatePrev = mouseStateCur;
        mouseStateCur  = SDL_GetMouseState(&mouseX, &mouseY);
        SDL_GetRelativeMouseState(&deltaX, &deltaY);
    }

    void cleanup()
    {
        vg::destroyFont(defaultFont);

        memset(keyStateCur,  0, SDLK_LAST);
        memset(keyStatePrev, 0, SDLK_LAST);

        mouseStatePrev = 0;
        mouseStateCur  = 0;

        deltaX = 0;
        deltaY = 0;
    }

    bool keyIsPressed  (int key)
    {
        assert(key<SDLK_LAST);
        return keyStateCur[key] != 0;
    }

    bool keyWasPressed (int key)
    {
        assert(key<SDLK_LAST);
        return !keyStatePrev[key] && keyStateCur[key];
    }

    bool keyIsReleased (int key)
    {
        assert(key<SDLK_LAST);
        return !keyStateCur[key];
    }

    bool keyWasReleased(int key)
    {
        assert(key<SDLK_LAST);
        return keyStatePrev[key] && !keyStateCur[key];
    }

    bool mouseIsPressed  (int button)
    {
        return (mouseStateCur&SDL_BUTTON(button)) != 0;
    }

    bool mouseWasPressed (int button)
    {
        return !(mouseStatePrev&SDL_BUTTON(button)) && (mouseStateCur&SDL_BUTTON(button));
    }

    bool mouseIsReleased (int button)
    {
        return !(mouseStateCur&SDL_BUTTON(button));
    }

    bool mouseWasReleased(int button)
    {
        return (mouseStatePrev&SDL_BUTTON(button)) && !(mouseStateCur&SDL_BUTTON(button));
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
        glm::vec3   direction(0.0f, 0.0f, 0.0f);
        float       heading = 0.0f;
        float       pitch   = 0.0f;

        uint8_t *keystate = SDL_GetKeyState(NULL);

        direction.z += ui::keyStateCur[SDLK_w]?1.0f:0.0f;
        direction.z -= ui::keyStateCur[SDLK_s]?1.0f:0.0f;
        direction.x -= ui::keyStateCur[SDLK_a]?1.0f:0.0f;
        direction.x += ui::keyStateCur[SDLK_d]?1.0f:0.0f;

        pitch   += ui::keyStateCur[SDLK_KP8]?1.50f:0.0f;
        pitch   -= ui::keyStateCur[SDLK_KP5]?1.50f:0.0f;
        heading += ui::keyStateCur[SDLK_KP4]?1.50f:0.0f;
        heading -= ui::keyStateCur[SDLK_KP6]?1.50f:0.0f;

        camera->rotateSmoothly(heading, pitch);
        camera->rotateSmoothly((float)-ui::deltaX, (float)-ui::deltaY);
        camera->updatePosition(direction, dt);
    }

    void displayStats(float x, float y, float w, float h, float cpuTime, float gpuTime)
    {
        char* gpuName;
        char  str[256];

        vg::drawRoundedRect(x, y, x+w, y+h, 5, 5, 0xC0000000, 0xFF00FF32);

        glColor3f(1.0f, 1.0f, 1.0f);

        gpuName = (char*)glGetString(GL_RENDERER);
        vg::drawString(ui::defaultFont, x+15.0f, y+18.0f, gpuName, strlen(gpuName));

        _snprintf(str, 256, "CPU time - %f ms", cpuTime);
        vg::drawString(ui::defaultFont, x+15.0f, y+37.0f, str, strlen(str));

        _snprintf(str, 256, "GPU time - %f ms", gpuTime);
        vg::drawString(ui::defaultFont, x+15.0f, y+55.0f, str, strlen(str));
    }

    void processCameraDirectorInput(CameraDirector* camDirector, SpectatorCamera* camera)
    {
        assert(camDirector && camera);

        if (ui::keyWasReleased(SDLK_0))
        {
            camDirector->savedCamRotation.push_back(camera->getOrientation());
            camDirector->savedCamLocation.push_back(camera->getPosition());
        }

        if (ui::keyWasReleased(SDLK_EQUALS) &&
            ui::keyIsPressed(SDLK_LALT)     &&
            camDirector->savedCamRotation.size())
        {
            int max = camDirector->savedCamRotation.size() - 1;
            int idx = camDirector->index;

            idx = idx + 1;
            idx = idx > max ? 0 : idx;

            camDirector->index = idx;

            camera->setOrientation(camDirector->savedCamRotation[camDirector->index]);
            camera->setPosition   (camDirector->savedCamLocation[camDirector->index]);
        }

        if (ui::keyWasReleased(SDLK_MINUS) &&
            ui::keyIsPressed(SDLK_LALT)    &&
            camDirector->savedCamRotation.size())
        {
            int max = camDirector->savedCamRotation.size() - 1;
            int idx = camDirector->index;

            idx = idx - 1;
            idx = idx < 0 ? max : idx;

            camDirector->index = idx;

            camera->setOrientation(camDirector->savedCamRotation[camDirector->index]);
            camera->setPosition   (camDirector->savedCamLocation[camDirector->index]);
        }
    }
}
