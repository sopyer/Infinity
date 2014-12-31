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

struct profiler_event_t
{
    uint64_t threadID;          //TODO: make it uint16_t
    size_t   id;                //TODO: make it uint16_t
    uint32_t timestamp : TIME_BITS;
    uint32_t phase     : PHASE_BITS;
};

struct profiler_desc_t
{
    const char* name;
};

void profilerInit();
void profilerFini();

void profilerStartCapture   ();
void profilerStopCapture    ();
int  profilerIsCaptureActive();

void profilerStartSyncPoint();
void profilerStopSyncPoint ();

void profilerAddCPUEvent(size_t id, size_t eventPhase);
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
