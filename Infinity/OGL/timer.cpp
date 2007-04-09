//-----------------------------------------------------------------------------
// Copyright (c) 2000 Andreas Jönsson
//-----------------------------------------------------------------------------
#include "timer.h"

//-----------------------------------------------------------------------------
// CTimer()
//-----------------------------------------------------------------------------
Timer::Timer()
{
	// We need to know how often the clock is updated
    if( !QueryPerformanceFrequency((LARGE_INTEGER *)&m_TicksPerSecond) )
        m_TicksPerSecond = 1000;

	m_fFps = 0;
	m_bRunning = false;
}


//-----------------------------------------------------------------------------
// Start()
// Reset counter and start the timer
//-----------------------------------------------------------------------------
void Timer::Start()
{
	// Get the current time so we know when we started
    if( !QueryPerformanceCounter((LARGE_INTEGER *)&m_BaseTicks) )
    {
        m_BaseTicks = (UINT64)timeGetTime();
    }

	m_bRunning = true;

	m_fLastUpdate = 0;
	m_dwNumFrames = 0;

	m_fFrameTime = 0;
	m_fDeltaTime = 0;
}


//-----------------------------------------------------------------------------
// Stop()
// Stop the timer
//-----------------------------------------------------------------------------
void Timer::Stop()
{
	if( m_bRunning )
	{
		// Remember when we stopped so we can know how long we have been paused
		if( !QueryPerformanceCounter((LARGE_INTEGER *)&m_StopTicks) )
		{
			m_StopTicks = (UINT64)timeGetTime();
		}

		m_bRunning = false;
	}
}



//-----------------------------------------------------------------------------
// Continue()
// Start the timer without resetting
//-----------------------------------------------------------------------------
void Timer::Continue()
{
	if( !m_bRunning )
	{
		UINT64 Ticks;

		// Get the current time
		if( !QueryPerformanceCounter((LARGE_INTEGER *)&Ticks) )
		{
			Ticks = (UINT64)timeGetTime();
		}

		// Increase baseticks to reflect the time we were paused
		m_BaseTicks += Ticks - m_StopTicks;

		m_bRunning = true;
	}
}




//-----------------------------------------------------------------------------
// GetTime()
// Get the current time
//-----------------------------------------------------------------------------
float Timer::GetTime()
{
	UINT64 Ticks;

	if( m_bRunning )
	{
		if( !QueryPerformanceCounter((LARGE_INTEGER *)&Ticks) )
		{
			Ticks = (UINT64)timeGetTime();
		}
	}
	else
		Ticks = m_StopTicks;

	// Subtract the time when we started to get 
	// the time our timer has been running
	Ticks -= m_BaseTicks;

	return (float)(__int64)Ticks/(float)(__int64)m_TicksPerSecond;
}



//-----------------------------------------------------------------------------
// Frame()
// Call this once per frame
//-----------------------------------------------------------------------------
void Timer::Frame()
{
	m_fDeltaTime = GetTime() - m_fFrameTime;
	m_fFrameTime += m_fDeltaTime;

	// Update frames per second counter
	m_dwNumFrames++;
	if( m_fFrameTime - m_fLastUpdate > FPS_INTERVAL )
	{
		m_fFps = m_dwNumFrames / (m_fFrameTime - m_fLastUpdate);
		m_dwNumFrames = 0;
		m_fLastUpdate = m_fFrameTime;
	}
}


//-----------------------------------------------------------------------------
// GetFps()
//-----------------------------------------------------------------------------
float Timer::GetFps()
{
	return m_fFps;
}


//-----------------------------------------------------------------------------
// GetFrameTime()
// This is the time when Frame() was called last
//-----------------------------------------------------------------------------
float Timer::GetFrameTime()
{
	return m_fFrameTime;
}

//-----------------------------------------------------------------------------
// GetDeltaTime()
// This is the time that passed between the last to calls to Frame()
//-----------------------------------------------------------------------------
float Timer::GetDeltaTime()
{
	return m_fDeltaTime;
}

//-----------------------------------------------------------------------------
// IsRunning()
//-----------------------------------------------------------------------------
bool Timer::IsRunning()
{
	return m_bRunning;
}