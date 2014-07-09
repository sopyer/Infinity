#ifndef _TIMER_H_INCLUDED_
#   define _TIMER_H_INCLUDED_

#include <SDL2/SDL.h>
#include <utils.h>

struct cpu_timer_t
{
    uint64_t startTime;
    uint64_t measuredTime;
};

void     cpu_timer_init    (cpu_timer_t* timer);
void     cpu_timer_start   (cpu_timer_t* timer);
void     cpu_timer_stop    (cpu_timer_t* timer);
uint64_t cpu_timer_measured(cpu_timer_t* timer);

uint64_t timerAbsoluteTime();

#endif