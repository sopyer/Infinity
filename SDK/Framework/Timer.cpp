#include <SDL2/SDL.h>
#include <assert.h>

uint64_t timerAbsoluteTime()
{
    uint64_t freq, time;

    time = SDL_GetPerformanceCounter();
    freq = SDL_GetPerformanceFrequency();

    return time * 1000000 / freq;
}
