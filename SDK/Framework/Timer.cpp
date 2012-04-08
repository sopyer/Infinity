#include "Timer.h"
#include <assert.h>

__int64 timerAbsoluteTime()
{
    LARGE_INTEGER freq, time;
    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&freq);
    __int64 t1 = time.QuadPart*1000000;
    __int64 t2 = t1/freq.QuadPart;
    return t2;
}
