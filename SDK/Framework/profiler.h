#ifndef __FRAMEWORK_PROFILER_H_INCLUDED__
#	define __FRAMEWORK_PROFILER_H_INCLUDED__

#   ifdef __cplusplus
extern "C"
{
#   endif


#	define MAX_PROFILER_BLOCKS			16384

typedef size_t	profiler_block_id;

struct profiler_block_t
{
    __int64		start;
    __int64		end;
    const char*	name;
    float		duration;
};

struct profiler_data_t
{
    size_t				numBlocks;
    profiler_block_t	blocks[MAX_PROFILER_BLOCKS];
};

extern profiler_data_t	profilerData;

void profilerBeginFrame();
void profilerEndFrame();

size_t profilerStartCPUBlock(const char* name);
void   profilerEndCPUBlock(size_t id);


#   ifdef __cplusplus
}

struct ProfilerCPUAutoBlock 
{
    profiler_block_id id;

    ProfilerCPUAutoBlock(const char* name) {id = profilerStartCPUBlock(name);}
    ~ProfilerCPUAutoBlock() {profilerEndCPUBlock(id);}
};

#       define PROFILER_CPU_BLOCK(name) ProfilerCPUAutoBlock __PROFILER_AUTO_VAR_ ## __LINE__(name)

#   endif

#endif
