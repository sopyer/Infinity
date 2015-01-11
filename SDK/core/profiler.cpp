#include <SDL2/SDL.h>
#include <core/core.h>

#define MAX_PROFILER_EVENTS     1*1024*1024
#define LOG2_RES 21
#define MAX_PROFILER_IDS        2048

static_assert(MAX_PROFILER_IDS<=0x10000, "Maximum id should not exceed capacity of uint16_t");

extern "C"
{
    static atomic_t          lastId        = 0;
    static atomic_t          numEvents     = 0;
    static atomic_t          doCapture     = 0;
    static atomic_t          captureActive = 0;
    static profiler_event_t  events[MAX_PROFILER_EVENTS];
    static const char*       idNames[MAX_PROFILER_IDS];

    static uint32_t  quantShift;     //Precision shift for timestamp
    static uint64_t  maxPeriod;      //Max timespan for capture
    static uint64_t  startTime;
    static uint64_t  endTime;

    void profilerInit()
    {
        uint64_t freq = SDL_GetPerformanceFrequency();
        assert(freq);

        int log2 = bit_fls(freq);
        quantShift = log2 > LOG2_RES ? log2-LOG2_RES : 0;
        maxPeriod = (1ul<<TIME_BITS)<<quantShift;
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

    void profilerAddCPUEvent(uint16_t id, EventPhase eventPhase)
    {
        assert(id < lastId);
        if (!doCapture) return;

        size_t count = _InterlockedIncrement(&numEvents);
        uint64_t ts = SDL_GetPerformanceCounter();
        if (count<=MAX_PROFILER_EVENTS && ts<endTime)
        {
            size_t             i   = count-1;
            profiler_event_t&  evt = events[i];
            SDL_threadID       tid = SDL_ThreadID();
            
            //TODO: Quick hack! Works on win32
            assert(tid<=65535);

            evt.id        = id;
            evt.phase     = eventPhase;
            evt.tid       = (uint16_t)tid;
            evt.timestamp = (ts-startTime) >> quantShift;
        }
        else
        {
            _InterlockedDecrement(&numEvents);
            assert(0);
        }
    }

    uint16_t profilerGenerateId()
    {
        size_t id = _InterlockedIncrement(&lastId);
        assert(id <= MAX_PROFILER_IDS);
        return id - 1;
    }

    void profilerAddDesc(uint16_t id, const char* name)
    {
        assert(id<lastId);
        idNames[id] = name;
    }

    void profilerGetData(size_t* numEvents, const profiler_event_t** events, const char*** names)
    {
        assert(!captureActive && !doCapture);
        *numEvents = ::numEvents;
        *events    = ::events;
        *names     = ::idNames;
    }

}