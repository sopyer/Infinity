#ifndef __FRAMEWORK_PROFILER_H_INCLUDED__
#	define __FRAMEWORK_PROFILER_H_INCLUDED__

#	include <windows.h>

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

inline void profilerBeginFrame()
{
	profilerData.numBlocks = 0;
}

void profilerEndFrame();

inline profiler_block_id profilerStartBlock(const char* name)
{
	if (profilerData.numBlocks!=MAX_PROFILER_BLOCKS)
	{
		size_t            id    = profilerData.numBlocks++;
		profiler_block_t& block = profilerData.blocks[id];

		if (block.name!=name)
		{
			block.name = name;
			block.duration = 0.0f;
		}
		QueryPerformanceCounter((LARGE_INTEGER*)&(block.start));

		return id;
	}

	return MAX_PROFILER_BLOCKS;
}

inline void profilerEndBlock(size_t id)
{
	if (id!=MAX_PROFILER_BLOCKS)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&(profilerData.blocks[id].end));
	}
}

#endif
