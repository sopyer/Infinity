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
    size_t elementUnderCursor(int x, int y);
    void   addInterval(
        const char* name, uint32_t color,
        uint16_t trackID, uint32_t start,
        uint32_t duration, int depth
    );

private:
    size_t numThreads;
    std::vector<rect_t>   rectData;
    std::vector<Interval> intervals;
    std::vector<uint32_t> colors;

    std::vector<uint32_t> selection;

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

    float  sx, sy, dx;
    bool   mDoDrag;

    int      mbx, mby;
    uint64_t prevMouseMoveTime;
    bool     showTooltip;
};
