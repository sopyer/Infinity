#pragma once

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
    void   layoutUI(int w, int h);
    void   drawBars(uint32_t* colorArray);
    size_t elementUnderCursor(int x, int y);

private:
    size_t numThreads;
    std::vector<float>    rectData;
    std::vector<Interval> intervals;
    std::vector<uint32_t> colors;

    int width;
    int height;
    
    rect_t graphArea;
    rect_t helpArea;
    rect_t statArea;

    uint32_t    startInterval, endInterval, interval;
    uint32_t    minTime, maxTime;
    const char* unitFormat;
    uint32_t    unitScale;
    float       pxIntervalStep;

    size_t mSelection;
    float  sx, sy, dx;
    bool   mDoDrag;

    int      mbx, mby;
    uint64_t prevMouseMoveTime;
    bool     showTooltip;
};
