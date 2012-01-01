#include <algorithm>
#include <assert.h>
#include <windows.h>
#include "profiler.h"

extern "C"
{
profiler_data_t profilerData;

void profilerBeginFrame()
{
    profilerData.numBlocks = 0;
}

void profilerEndFrame()
{

    //should use variation of arithmetic sort by thread, not std::sort.

    __int64			ticksPerUs;
    QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerUs);

    for (size_t i=0; i<profilerData.numBlocks; ++i)
    {
        profiler_block_t& b = profilerData.blocks[i];
        float dur = (b.end-b.start)*1000000/ticksPerUs/1000.0f;
        b.duration = dur;//(b.duration!=0)?(0.1f*dur+0.9f*b.duration):dur;
    }
}

profiler_block_id profilerStartCPUBlock(const char* name)
{
    if (profilerData.numBlocks!=MAX_PROFILER_BLOCKS)
    {
        size_t            id    = profilerData.numBlocks++;
        profiler_block_t& block = profilerData.blocks[id];

        block.name = name;
        QueryPerformanceCounter((LARGE_INTEGER*)&(block.start));

        return id;
    }

    return MAX_PROFILER_BLOCKS;
}

void profilerEndCPUBlock(size_t id)
{
    if (id!=MAX_PROFILER_BLOCKS)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&(profilerData.blocks[id].end));
    }
}

}