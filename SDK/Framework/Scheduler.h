#pragma once

#include <queue>
#include "utils.h"
//#include <Timer.h>
#include <Singleton.h>
#include <sigslot.h>

struct Timeout
{
	void (*mTimerFunc)(void*);
	void*  mUserData;
	u32 mScheduledTime;

	Timeout(void (*timerFunc)(void*), void* userData, u32 scheduledTime)
		:mTimerFunc(timerFunc), mUserData(userData), mScheduledTime(scheduledTime)
	{}

};

inline bool operator<(const Timeout& lhs, const Timeout& rhs)
{return lhs.mScheduledTime > rhs.mScheduledTime;}


class Scheduler: public Singleton<Scheduler>
{
public:
	Scheduler()
	{
		mTimer.start();
	}

	~Scheduler()
	{
		mTimer.stop();
	}

	void scheduleTask(void (*timerFunc)(void*), void* userData, u32 relTime)
	{
		mTaskQueue.push(Timeout(timerFunc, userData, relTime+mTimer.getTime()));
	}

	void mainLoop()
	{
		u32 time = mTimer.getTime();

		while(!mTaskQueue.empty() && mTaskQueue.top().mScheduledTime <= time)
		{
			Timeout& timeout = mTaskQueue.top();
			timeout.mTimerFunc(timeout.mUserData);
			mTaskQueue.pop();
		}
	}

private:
	std::priority_queue<Timeout>	mTaskQueue;
	Timer						mTimer;
};

//Fix case when duration > numFrames*frameTime
//Integrate Timer in this class?????
class Timeline
{
public:
	Timeline(u32 msFPS):
		mDuration(1000), mNumFrames(msFPS),
		mCurFrame(0), mFrameTime(1000/msFPS),
		mLoop(true), mIsPlaying(false)
	{
	}

	Timeline(u32 msDuration, u32 msFPS):
		mDuration(msDuration), mNumFrames(msDuration*msFPS / 1000),
		mCurFrame(0), mFrameTime(1000/msFPS),
		mLoop(false), mIsPlaying(false)
	{
	}

	void start()
	{
		if (mIsPlaying)
			return;

		if (mCurFrame == 0)
		{
			onStarted.emit();
		}
		
		u32 timeout = 0;

		if (timeout == 0)
			timeout = mFrameTime;

		scheduleTimeout(timeout);
		mIsPlaying = true;
	}

	void stop()
	{
		mIsPlaying = false;
		mCurFrame = 0;
	}

	void pause()
	{
		mIsPlaying = false;
	}
	
	u32 getElapsed() {return mCurFrame * mFrameTime;}
	u32 getDuration() {return mDuration;}

public:
	sigslot::signal1<u32>	onFrame;
	sigslot::signal0<>			onStarted;
	sigslot::signal0<>			onCompleted;

private:
	static void onTimeout(void* userData)
	{
		Timeline& self = *(Timeline*)userData;
		self.handleTimeout();
	}
	
	void scheduleTimeout(u32 deltaTime)
	{
		Scheduler::getRef().scheduleTask(&Timeline::onTimeout, this, deltaTime);
	}

	void handleTimeout()
	{
		if (!mIsPlaying)
			return;

		++mCurFrame;
		onFrame.emit(mCurFrame);
		
		if (mCurFrame == mNumFrames)
			mCurFrame = 0;

		if (mCurFrame == 0 && !mLoop)
		{
			mIsPlaying = false;
			onCompleted.emit();
			return;
		}

		scheduleTimeout(mFrameTime);
	}

private:
	u32	mCurFrame;
	u32	mFrameTime;
	u32	mDuration;
	u32	mNumFrames;

	bool	mLoop;
	bool	mIsPlaying;
};