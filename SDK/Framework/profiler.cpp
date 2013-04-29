#include <algorithm>
#include <assert.h>
#include <windows.h>
#include "profiler.h"

#define MAX_PROFILER_EVENTS     16384

extern "C"
{
    static volatile long		numEvents;
    static profiler_event_t	    events[MAX_PROFILER_EVENTS];

    void profilerBeginDataCollection()
    {
        numEvents = 0;
    }

    void profilerEndDataCollection()
    {
        __int64 ticksPerSecond;
        QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond);

        // convert timestamps to microseconds
        for (long i = 0; i < numEvents; ++i)
        {
            events[i].timestamp = events[i].timestamp * 1000000 / ticksPerSecond;
        }
    }

    void profilerAddCPUEvent(size_t id, size_t eventPhase)
    {
        size_t count = _InterlockedIncrement(&numEvents);
        if (count<=MAX_PROFILER_EVENTS)
        {
            size_t              i     = count-1;
            profiler_event_t&   event = events[i];

            event.id       = id;
            event.phase    = eventPhase;
            event.threadID = GetCurrentThreadId();
            QueryPerformanceCounter((LARGE_INTEGER*)&(event.timestamp));
        }
        else
        {
            _InterlockedDecrement(&numEvents);
            assert(0);
        }
    }

    void profilerGetData(size_t* numEvents, const profiler_event_t** events)
    {
        *numEvents = ::numEvents;
        *events    = ::events;
    }

}