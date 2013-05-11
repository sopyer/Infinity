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

#include <SDL.h>
#include <opengl.h>

struct Vertex
{
    float    x, y;
};

struct Interval
{
    const char* name;
    float       duration;
    float       xmin, ymin;
    float       xmax, ymax;
};

class ProfilerOverlay
{
public:
    void initialise(int w, int h);
    void loadProfilerData();

    void renderFullscreen();
    void updateUI();

private:
    void drawBars(uint32_t* colorArray);

private:
    std::vector<Vertex> drawData;
    std::vector<Interval> intervals;
    std::vector<uint32_t> colors;
    std::vector<uint32_t> ids;
    float mWidth;
    float mHeight;

    int   mSelection;
    float mScale;
    bool  mDoDrag;
    float mOffsetX;
};
