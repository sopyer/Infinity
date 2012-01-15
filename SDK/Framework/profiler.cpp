#include <algorithm>
#include <assert.h>
#include <windows.h>
#include "profiler.h"

#define MAX_PROFILER_BLOCKS			16384

extern "C"
{
    static size_t			numBlocks;
    static profiler_block_t	blocks[MAX_PROFILER_BLOCKS];

    void profilerBeginFrame()
    {
        numBlocks = 0;
    }

    void profilerEndFrame()
    {

        //should use variation of arithmetic sort by thread, not std::sort.

        __int64			ticksPerUs;
        QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerUs);

        for (size_t i=0; i<numBlocks; ++i)
        {
            profiler_block_t& b = blocks[i];
            float dur = (b.end-b.start)*1000000/ticksPerUs/1000.0f;
            b.duration = dur;//(b.duration!=0)?(0.1f*dur+0.9f*b.duration):dur;
        }
    }

    size_t profilerStartCPUBlock(const char* name)
    {
        if (numBlocks!=MAX_PROFILER_BLOCKS)
        {
            size_t            id    = numBlocks++;
            profiler_block_t& block = blocks[id];

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
            QueryPerformanceCounter((LARGE_INTEGER*)&(blocks[id].end));
        }
    }

    void getProfilerData(size_t* numEntries, profiler_block_t** blockData)
    {
        *numEntries = numBlocks;
        *blockData  = blocks;
    }
}