#ifndef _TIMER_H_INCLUDED_
#	define _TIMER_H_INCLUDED_

#include <windows.h>
//#include <stdint.h>

//class Timer
//{
//public:
//	Timer(): mStartTime(0), mFrameStartTime(0) {QueryPerformanceFrequency(&mFrequency);}
//	~Timer() {}
//
//	void resetFrame(){mFrameStartTime = getAbsTime();}
//
//	uint32_t getFrameTime() {return mFrameStartTime ? getAbsTime() - mFrameStartTime : 0;}
//	uint32_t getTime() {return mStartTime ? getAbsTime() - mStartTime : 0;}
//
//	void start() {mStartTime = getAbsTime();}
//	void stop() {mStartTime = 0; mFrameStartTime = 0;}
//
//private:
//	uint32_t getAbsTime()
//	{
//		LARGE_INTEGER	time;
//		QueryPerformanceCounter(&time);
//		return uint32_t(time.QuadPart * 1000 / mFrequency.QuadPart);
//	}
//
//private:
//	uint32_t mStartTime;
//	uint32_t mFrameStartTime;
//
//	LARGE_INTEGER mFrequency;
//};
//
struct Timer2
{
	void start()
	{
		QueryPerformanceCounter(&startTime);
	}

	void stop()
	{
		startTime.QuadPart = 0;
	}

	double elapsed()
	{
		LARGE_INTEGER	frequency;
		LARGE_INTEGER	time;

		QueryPerformanceCounter(&time);
		QueryPerformanceFrequency(&frequency);

		return (time.QuadPart-startTime.QuadPart)*1000.0/frequency.QuadPart;
	}

	LARGE_INTEGER startTime;
};

#include <Scheduler.h>

#endif