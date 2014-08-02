#pragma once

#include <SDL2/SDL.h>

#include <core/core.h>
#include <gfx/gfx.h>
#include <fwk/ui.h>
#include <fwk/media_api.h>
#include <fwk/SpectatorCamera.h>
#include <fwk/CameraDirector.h>

namespace app
{
    void init();
    void fini();
    void update(float dt);
    void render();
    void recompilePrograms();
    void resize(int width, int height);
}

namespace fwk
{
    extern SDL_Window*      window;
    extern SDL_GLContext    context;

    void init(const char* argv0);
    void fini();

    void run();

    void setCaption(const char* caption);
    void recompilePrograms();
    void exit();
}
