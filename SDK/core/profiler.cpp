#include <SDL2/SDL.h>
#include <core/profiler.h>

#define LOG2_RES 21
#define MAX_PROFILER_IDS        2048

static_assert(MAX_PROFILER_IDS<=0x10000, "Maximum id should not exceed capacity of uint16_t");

static atomic_t          lastId        = 0;
static atomic_t          doCapture     = 0;
static atomic_t          captureActive = 0;
static const char*       idNames[MAX_PROFILER_IDS];

static event_capture_t capture;

void event_capture_init(event_capture_t* capture, uint64_t freq, uint32_t log2res)
{
    assert(capture);
    assert(freq);

    uint32_t log2 = bit_fls(freq);
    uint32_t quantShift = log2 > log2res ? log2-log2res : 0;

    capture->freq       = freq;
    capture->quantShift = quantShift;
    capture->maxPeriod  = (1ul<<TIME_BITS)<<quantShift;
}

void event_capture_start(event_capture_t* capture, uint64_t startTime)
{
    capture->numEvents = 0;
    capture->startTime = startTime;
}

void event_capture_stop(event_capture_t* capture, uint64_t endTime)
{
    capture->endTime = (uint32_t)core::min((endTime-capture->startTime) >> capture->quantShift, (1ull<<TIME_BITS)-1);
}
void event_capture_add(event_capture_t* capture, uint16_t trackID, uint16_t eventID, EventPhase eventPhase, uint64_t ts)
{
    atomic_t count = _InterlockedIncrement(&capture->numEvents);
    if (count<=capture->numEvents)
    {
        profiler_event_t&  evt = capture->events[count-1];
        evt.id        = eventID;
        evt.phase     = eventPhase;
        evt.tid       = trackID;
        evt.timestamp = core::min((ts-capture->startTime) >> capture->quantShift, (1ull<<TIME_BITS)-1);
    }
    else
    {
        _InterlockedDecrement(&capture->numEvents);
    }
}

void profilerInit()
{
    event_capture_init(&capture, SDL_GetPerformanceFrequency(), LOG2_RES);
}

void profilerStartCapture()
{
    captureActive = TRUE;
    event_capture_start(&capture, SDL_GetPerformanceCounter());
}

void profilerStopCapture()
{
    captureActive = FALSE;
    event_capture_stop(&capture, SDL_GetPerformanceCounter());
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
    }
}

void profilerAddCPUEvent(uint16_t id, EventPhase eventPhase)
{
    assert(id < lastId);
    if (!doCapture) return;

    SDL_threadID  tid = SDL_ThreadID();
    //TODO: Quick hack! Works on win32
    assert(tid<=65535);

    event_capture_add(&capture, (uint16_t)tid, id, eventPhase, SDL_GetPerformanceCounter());
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

event_capture_t* profilerGetData()
{
    assert(!captureActive && !doCapture);
    return &capture;
}

const char** profilerGetNames()
{
    assert(!captureActive && !doCapture);
    return idNames;
}
