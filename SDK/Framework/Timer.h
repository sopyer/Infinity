#ifndef _TIMER_H_INCLUDED_
#   define _TIMER_H_INCLUDED_

#include <SDL2/SDL.h>
#include <utils.h>
#include <opengl.h>

struct CPUTimer
{
    void start()
    {
        startTime = SDL_GetPerformanceCounter();
    }

    void stop()
    {
        startTime = 0;
    }

    double elapsed()
    {
        uint64_t freq;
        uint64_t time;

        time = SDL_GetPerformanceCounter();
        freq = SDL_GetPerformanceFrequency();

        return (time - startTime) * 1000.0 / freq;
    }

    uint64_t startTime;
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