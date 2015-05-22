#pragma once

enum EventPhase
{
    PROF_EVENT_PHASE_BEGIN,
    PROF_EVENT_PHASE_END,
    PROF_EVENT_PHASE_MARKER,
    PROF_EVENT_PHASE_COUNT
};

#define TIME_BITS  30
#define PHASE_BITS  2
#define MAX_PROFILER_EVENTS     1*1024*1024

static_assert(TIME_BITS+PHASE_BITS <= 32, "Check that phase and timestamps use up to 32 bits");

struct profiler_event_t
{
    uint16_t id;       //TODO: make it uint16_t
    uint16_t tid;
    uint32_t phase     : PHASE_BITS;
    uint32_t timestamp : TIME_BITS;
};

static_assert(sizeof(profiler_event_t)==8, "Fix packing in profiler_event_t in order ro maintain smaller event size");

struct event_capture_t
{
    uint64_t  maxPeriod;      //Max timespan for capture
    uint64_t  freq;
    uint64_t  startTime;
    atomic_t  numEvents;
    uint32_t  quantShift;     //Precision shift for timestamp
    uint32_t  maxEvents;
    profiler_event_t  events[MAX_PROFILER_EVENTS];
};

void event_capture_init(event_capture_t* capture, uint64_t freq, uint32_t log2res);
void event_capture_reset(event_capture_t* capture, uint64_t startTime);
void event_capture_add(
    event_capture_t* capture,
    uint16_t trackID, uint16_t eventID,
    EventPhase eventPhase, uint64_t ts
);
uint32_t event_capture_time_ms(event_capture_t* capture, uint32_t evtIdx);

// CPU capture interface

void profilerInit();

void profilerStartCapture   ();
void profilerStopCapture    ();
int  profilerIsCaptureActive();

void profilerStartSyncPoint();
void profilerStopSyncPoint ();

void profilerAddCPUEvent(uint16_t id, EventPhase eventPhase);

uint16_t profilerGenerateId();

// NOTE: name should be compile time(preferred) or has entire program lifetime
void profilerAddDesc(uint16_t id, const char* name);
event_capture_t* profilerGetData();
const char** profilerGetNames();
