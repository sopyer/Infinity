#include "profiler.h"

#include <algorithm>

profiler_data_t profilerData;

void profilerEndFrame()
{
	struct
	{
		bool operator () (const profiler_block_t& lhs, const profiler_block_t& rhs)
		{
			return lhs.start<rhs.start;
		}
	} sortPred;

	std::sort(profilerData.blocks, profilerData.blocks+profilerData.numBlocks, sortPred);
	
	__int64			ticksPerUs;
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerUs);

	for (size_t i=0; i<profilerData.numBlocks; ++i)
	{
		profiler_block_t& b = profilerData.blocks[i];
		float dur = (b.end-b.start)*1000000/ticksPerUs/1000.0f;
		b.duration = (b.duration!=0)?0.5f*(dur+b.duration):dur;
	}
}
