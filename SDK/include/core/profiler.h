#pragma once

#include <stdint.h>

#   ifdef __cplusplus
extern "C"
{
#   endif

enum EventPhase
{
    PROF_EVENT_PHASE_BEGIN,
    PROF_EVENT_PHASE_END,
    PROF_EVENT_PHASE_MARKER,
    PROF_EVENT_PHASE_COUNT
};

#define TIME_BITS  30
#define PHASE_BITS  2

static_assert(TIME_BITS+PHASE_BITS <= 32, "Check that phase and timestamps use up to 32 bits");

struct profiler_event_t
{
    size_t   id;       //TODO: make it uint16_t
    uint16_t tid;
    uint32_t phase     : PHASE_BITS;
    uint32_t timestamp : TIME_BITS;
};

void profilerInit();
void profilerFini();

void profilerStartCapture   ();
void profilerStopCapture    ();
int  profilerIsCaptureActive();

void profilerStartSyncPoint();
void profilerStopSyncPoint ();

void profilerAddCPUEvent(size_t id, uint32_t eventPhase);
void profilerGetData    (size_t* numEvents, const profiler_event_t** events);

#   ifdef __cplusplus
}

struct ProfilerCPUAutoTimeslice 
{
    size_t id;

     ProfilerCPUAutoTimeslice(const char* name) { id = (size_t)name; profilerAddCPUEvent( id, PROF_EVENT_PHASE_BEGIN ); }
    ~ProfilerCPUAutoTimeslice()                 { profilerAddCPUEvent( id, PROF_EVENT_PHASE_END ); }
};

#   endif
