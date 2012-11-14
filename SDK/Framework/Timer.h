#ifndef _TIMER_H_INCLUDED_
#	define _TIMER_H_INCLUDED_

#include <windows.h>
#include <utils.h>
#include <opengl.h>

struct CPUTimer
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

#define NUM_DELAYED_FRAMES 3

struct GPUTimer
{
	GPUTimer() : frameID(0), skipFrames(NUM_DELAYED_FRAMES)
    {
        glGenQueries(NUM_DELAYED_FRAMES, mQueries);
    }
	
    ~GPUTimer()
    {
        glDeleteQueries(NUM_DELAYED_FRAMES, mQueries);
    }

	void start()
	{
		glBeginQuery(GL_TIME_ELAPSED, mQueries[frameID]);
	}

	void stop()
	{
		glEndQuery(GL_TIME_ELAPSED);
        frameID = (frameID+1)%NUM_DELAYED_FRAMES;
	}

	double getResult()
	{
        GLenum err = glGetError();
        if (err!=GL_NO_ERROR) __asm int 3; 

        if (skipFrames)
        {
            --skipFrames;
            return 0.0f;
        }

		GLuint64EXT result;
		glGetQueryObjectui64v(mQueries[frameID], GL_QUERY_RESULT, &result);
        err = glGetError();
        if (err!=GL_NO_ERROR) __asm int 3; 
        
        return result/1000.0f/1000.0f;
	}

	GLuint mQueries[NUM_DELAYED_FRAMES];
    size_t frameID;
    size_t skipFrames;
};

uint64_t timerAbsoluteTime();


#include <Scheduler.h>

#endif