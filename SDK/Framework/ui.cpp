#include "UI.h"

#include "DefaultFontData.h"
#include "SpectatorCamera.h"

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
        return keyStateCur[key];
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
