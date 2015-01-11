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
    uint16_t id;       //TODO: make it uint16_t
    uint16_t tid;
    uint32_t phase     : PHASE_BITS;
    uint32_t timestamp : TIME_BITS;
};

static_assert(sizeof(profiler_event_t)==8, "Fix packing in profiler_event_t in order ro maintain smaller event size");

void profilerInit();
void profilerFini();

void profilerStartCapture   ();
void profilerStopCapture    ();
int  profilerIsCaptureActive();

void profilerStartSyncPoint();
void profilerStopSyncPoint ();

void profilerAddCPUEvent(uint16_t id, EventPhase eventPhase);

uint16_t profilerGenerateId();

void profilerAddDesc(uint16_t id, const char* name);
void profilerGetData(size_t* numEvents, const profiler_event_t** events, const char*** names);

#   ifdef __cplusplus
}
#   endif
