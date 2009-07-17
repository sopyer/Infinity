#ifndef _TIMER_H_INCLUDED_
#	define _TIMER_H_INCLUDED_

#include <windows.h>

class Timer
{
public:
	Timer(): mStartTime(0), mFrameStartTime(0) {QueryPerformanceFrequency(&mFrequency);}
	~Timer() {}

	void resetFrame(){mFrameStartTime = getAbsTime();}

	u32 getFrameTime() {return mFrameStartTime ? getAbsTime() - mFrameStartTime : 0;}
	u32 getTime() {return mStartTime ? getAbsTime() - mStartTime : 0;}

	void start() {mStartTime = getAbsTime();}
	void stop() {mStartTime = 0; mFrameStartTime = 0;}

private:
	u32 getAbsTime()
	{
		LARGE_INTEGER	time;
		QueryPerformanceCounter(&time);
		return u32(time.QuadPart * 1000 / mFrequency.QuadPart);
	}

private:
	u32 mStartTime;
	u32 mFrameStartTime;

	LARGE_INTEGER mFrequency;
};

#include <Scheduler.h>

#endif