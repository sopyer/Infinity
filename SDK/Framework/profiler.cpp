#include <assert.h>
#include <SDL2/SDL.h>
#include <utils.h>
#include "profiler.h"

#define MAX_PROFILER_EVENTS     16384

extern "C"
{
    static volatile long        numEvents = 0;
    static profiler_event_t     events[MAX_PROFILER_EVENTS];
    static volatile long        doCapture = FALSE;
    static long                 captureActive = FALSE;

    void profilerStartCapture()
    {
        numEvents     = 0;
        captureActive = TRUE;
    }

    void profilerStopCapture()
    {
        captureActive = FALSE;
    }

    int profilerIsCaptureActive()
    {
        return captureActive;
    }

    void profilerStartSyncPoint()
    {
        doCapture = captureActive;
    }

    void profilerStopSyncPoint()
    {
        if (!captureActive)
        {
            doCapture = FALSE;

            uint64_t ticksPerSecond = SDL_GetPerformanceFrequency();

            // convert timestamps to microseconds
            for (long i = 0; i < numEvents; ++i)
            {
                events[i].timestamp = events[i].timestamp * 1000000 / ticksPerSecond;
            }
        }
    }

    void profilerAddCPUEvent(size_t id, size_t eventPhase)
    {
        if (!doCapture) return;

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
        assert(!captureActive && !doCapture);
        *numEvents = ::numEvents;
        *events    = ::events;
    }

}