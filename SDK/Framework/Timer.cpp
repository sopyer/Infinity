#include <SDL2/SDL.h>
#include <assert.h>
#include <timer.h>

uint64_t timerAbsoluteTime()
{
    uint64_t freq, time;

    time = SDL_GetPerformanceCounter();
    freq = SDL_GetPerformanceFrequency();

    return time * 1000000  / freq;
}

void cpu_timer_init(cpu_timer_t* timer)
{
    memset(timer, 0, sizeof(timer));
}

void cpu_timer_start(cpu_timer_t* timer)
{
    timer->measuredTime = 0;
    timer->startTime = SDL_GetPerformanceCounter();
}

void cpu_timer_stop(cpu_timer_t* timer)
{
    timer->measuredTime = SDL_GetPerformanceCounter() - timer->startTime;
}

uint64_t cpu_timer_measured(cpu_timer_t* timer)
{
    return timer->measuredTime * 1000000 / SDL_GetPerformanceFrequency();
}
