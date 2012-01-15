#ifndef __FRAMEWORK_PROFILER_H_INCLUDED__
#	define __FRAMEWORK_PROFILER_H_INCLUDED__

#   ifdef __cplusplus
extern "C"
{
#   endif

struct profiler_block_t
{
    __int64		start;
    __int64		end;
    const char*	name;
    float		duration;
};

void profilerBeginFrame();
void profilerEndFrame();

size_t profilerStartCPUBlock(const char* name);
void   profilerEndCPUBlock(size_t id);

//Return readonly info about profiler blocks
void getProfilerData(size_t* numBlocks, profiler_block_t** blockData);

#   ifdef __cplusplus
}

struct ProfilerCPUAutoBlock 
{
    size_t id;

    ProfilerCPUAutoBlock(const char* name) {id = profilerStartCPUBlock(name);}
    ~ProfilerCPUAutoBlock() {profilerEndCPUBlock(id);}
};
#   endif

#   ifdef __cplusplus
#       define PROFILER_CPU_BLOCK(name) ProfilerCPUAutoBlock __PROFILER_AUTO_VAR_ ## __LINE__(name)
#   endif

#endif
