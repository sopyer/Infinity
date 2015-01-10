#pragma once

#include <new>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <opengl.h>

struct Interval
{
    const char* name;
    float       start;
    float       duration;
};

class ProfilerOverlay
{
public:
    void init();
    void fini();

    void resize(int w, int h);

    void loadProfilerData();

    void renderFullscreen();
    void updateUI(float delta);

private:
    void drawBars(uint32_t* colorArray);

private:
    size_t numThreads;
    std::vector<float>    rectData;
    std::vector<Interval> intervals;
    std::vector<uint32_t> colors;

    int width;
    int height;
    
    rect_t graphArea;

    uint32_t    startInterval, endInterval, interval;
    uint32_t    minTime, maxTime;
    const char* unitFormat;
    uint32_t    unitScale;
    float       pxIntervalStep;

    size_t mSelection;
    float  sx, sy, dx;
    bool   mDoDrag;
};
