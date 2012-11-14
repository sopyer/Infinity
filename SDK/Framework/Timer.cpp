#include "Timer.h"
#include <assert.h>

uint64_t timerAbsoluteTime()
{
    LARGE_INTEGER freq, time;
    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&freq);
    uint64_t t1 = time.QuadPart*1000000;
    uint64_t t2 = t1/freq.QuadPart;
    return t2;
}
