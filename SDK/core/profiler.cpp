#include <SDL2/SDL.h>
#include <core/core.h>

#define MAX_PROFILER_EVENTS     16384
#define LOG2_RES 21

extern "C"
{
    static volatile long        numEvents = 0;
    static profiler_event_t     events[MAX_PROFILER_EVENTS];
    static volatile long        doCapture = FALSE;
    static long                 captureActive = FALSE;
    static uint32_t             quantShift;     //Precision shift for timestamp
    static uint64_t             maxPeriod;      //Max timespan for capture
    static uint64_t             startTime;
    static uint64_t             endTime;

    void profilerInit()
    {
        uint64_t freq = SDL_GetPerformanceFrequency();
        assert(freq);

        int log2 = bit_fls(freq);
        quantShift = log2 > LOG2_RES ? log2-LOG2_RES : 0;
        maxPeriod = ((0xFFFFFFFFu)>>PHASE_BITS)<<quantShift;
    }

    void profilerFini()
    {
    }
    
    void profilerStartCapture()
    {
        numEvents     = 0;
        captureActive = TRUE;
        startTime     = SDL_GetPerformanceCounter();
        endTime       = startTime + maxPeriod;
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
                events[i].timestamp = ((uint64_t)events[i].timestamp<<quantShift)* 1000000 / ticksPerSecond;
            }
        }
    }

    void profilerAddCPUEvent(size_t id, size_t eventPhase)
    {
        if (!doCapture) return;

        size_t count = _InterlockedIncrement(&numEvents);
        uint64_t ts = SDL_GetPerformanceCounter();
        if (count<=MAX_PROFILER_EVENTS && ts<=endTime)
        {
            size_t              i     = count-1;
            profiler_event_t&   event = events[i];

            event.id        = id;
            event.phase     = eventPhase;
            event.threadID  = SDL_ThreadID();
            event.timestamp = (ts-startTime) >> quantShift;
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