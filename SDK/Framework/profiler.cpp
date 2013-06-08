#include <SDL2/SDL.h>
#include <algorithm>
#include <assert.h>
#include <windows.h>
#include "profiler.h"

#define MAX_PROFILER_EVENTS     16384

extern "C"
{
    static volatile long		numEvents = 0;
    static profiler_event_t	    events[MAX_PROFILER_EVENTS];
    static volatile long        captureInProgress = FALSE;

    void profilerBeginDataCapture()
    {
        numEvents         = 0;
        captureInProgress = TRUE;
    }

    void profilerEndDataCapture()
    {
        captureInProgress = FALSE;

        __int64 ticksPerSecond = SDL_GetPerformanceFrequency();

        // convert timestamps to microseconds
        for (long i = 0; i < numEvents; ++i)
        {
            events[i].timestamp = events[i].timestamp * 1000000 / ticksPerSecond;
        }
    }

    int profilerIsCaptureInProgress ()
    {
        return captureInProgress;
    }

    void profilerAddCPUEvent(size_t id, size_t eventPhase)
    {
        if (!captureInProgress) return;

        size_t count = _InterlockedIncrement(&numEvents);
        if (count<=MAX_PROFILER_EVENTS)
        {
            size_t              i     = count-1;
            profiler_event_t&   event = events[i];

            event.id        = id;
            event.phase     = eventPhase;
            event.threadID  = SDL_ThreadID();
            event.timestamp = SDL_GetPerformanceCounter();
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