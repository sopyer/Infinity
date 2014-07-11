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
    float       duration;
};

class ProfilerOverlay
{
public:
    void init(int w, int h);
    void fini();

    void loadProfilerData();

    void renderFullscreen();
    void updateUI(float delta);

private:
    void drawBars(uint32_t* colorArray);

private:
    std::vector<float>    rectData;
    std::vector<Interval> intervals;
    std::vector<uint32_t> colors;
    std::vector<uint32_t> ids;
    float mWidth;
    float mHeight;

    size_t mSelection;
    float  mScale;
    bool   mDoDrag;
    float  mOffsetX;

    GLuint vboRectData;
    GLuint vboColorData;
    GLint  uniMVP;
    GLuint vao;
};
