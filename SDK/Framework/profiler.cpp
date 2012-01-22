#include <algorithm>
#include <assert.h>
#include <windows.h>
#include "profiler.h"

#define MAX_PROFILER_BLOCKS     16384
#define MAX_PROFILER_THREADS    64

extern "C"
{
    static volatile long		numBlocks;
    static profiler_block_t	    blocks[MAX_PROFILER_BLOCKS];
    static size_t               threadCount;
    static size_t               threadBlocksCount[MAX_PROFILER_THREADS];
    static __int64              threadIDs[MAX_PROFILER_THREADS];
    void profilerBeginFrame()
    {
        numBlocks = 0;
    }

    void profilerEndFrame()
    {

        //should use variation of arithmetic sort by thread, not std::sort.

        __int64			ticksPerUs;
        QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerUs);

        threadCount = 0;
        memset(threadBlocksCount, 0, sizeof(threadBlocksCount));
        //calculated duration and thread histogram
        for (size_t i=0; i<numBlocks; ++i)
        {
            profiler_block_t& b = blocks[i];
            float dur = (b.end-b.start)*1000000/ticksPerUs/1000.0f;
            b.duration = dur;//(b.duration!=0)?(0.1f*dur+0.9f*b.duration):dur;

            size_t t=0;
            for (; t<threadCount && threadIDs[t]!=b.threadID; ++t);
            if (t==threadCount && threadCount<MAX_PROFILER_THREADS)
            {
                t = threadCount++;
                threadIDs[t] = b.threadID;
            }
            if (t<MAX_PROFILER_THREADS)
            {
                ++threadBlocksCount[t];
            }
        }
    }

    size_t profilerStartCPUBlock(const char* name)
    {
        size_t count = _InterlockedIncrement(&numBlocks);
        if (count<=MAX_PROFILER_BLOCKS)
        {
            size_t              id    = count-1;
            profiler_block_t&   block = blocks[id];

            block.name = name;
            block.threadID = GetCurrentThreadId();
            QueryPerformanceCounter((LARGE_INTEGER*)&(block.start));

            return id;
        }
        else
            _InterlockedDecrement(&numBlocks);

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