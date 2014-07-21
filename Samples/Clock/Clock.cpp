#include <framework.h>
#include <time.h>
#include <utils.h>
#include <graphics.h>
#include "ResourceHelpers.h"

//#define RASTER_ACTORS

#ifdef RASTER_ACTORS

enum
{
    IMG_BACKGROUND,
    IMG_HOUR_HAND,
    IMG_MINUTE_HAND,
    IMG_SECOND_HAND,
    IMG_COUNT
};

const char*	imageNames[IMG_COUNT] = 
{
    "clockFace.png",
    "hourHand.png",
    "minuteHand.png",
    "secondHand.png",
};

const float w = 540;
const float h = 540;

#else
#   include "ClockData.h"
#endif

namespace app
{
    float  bkgOffsetX;
    float  bkgOffsetY;
    bool   mIsDragging;
    float  mOffsetX;
    float  mOffsetY;
    float  mScale;

#ifdef RASTER_ACTORS
    GLuint      mTextures[IMG_COUNT];
#else
    vg::Path    mClockPaths[PATH_COUNT];

    vg::Paint   mWhite;
    vg::Paint   mRed;
    vg::Paint   mBlack;
#endif

    void init()
    {
        mOffsetX = gfx::width  / 2.0f;
        mOffsetY = gfx::height / 2.0f;
        mIsDragging = false;
#ifdef RASTER_ACTORS
        mScale = 1.0f;
        glGenTextures(IMG_COUNT, mTextures);

        for (int i=0; i<IMG_COUNT; ++i)
        {
            mTextures[i] = resources::createTexture2D(imageNames[i]);
        }
#else
        mScale = 7.0f;
        mRed   = vg::createSolidPaint(0xFF0000FF);
        mBlack = vg::createSolidPaint(0xFF000000);
        mWhite = vg::createSolidPaint(0xFFFFFFFF);

        float x1, y1, x2, y2;

        for (size_t i=PATH_CLOCK_BKG; i<PATH_COUNT; ++i)
        {
            mClockPaths[i]  = vg::createPath(
                clockVectorImages[i].numCmd,  clockVectorImages[i].cmd,
                clockVectorImages[i].numData, clockVectorImages[i].data
            );
        }

        vg::getPathBounds(mClockPaths[PATH_CLOCK_BKG], x1, y1, x2, y2);

        bkgOffsetX = -0.5f * (x2 + x1);
        bkgOffsetY = -0.5f * (y2 + y1);
#endif
      }

    void fini()
    {
#ifdef RASTER_ACTORS
        glDeleteTextures(IMG_COUNT, mTextures);
#else
        for (size_t i=0; i<PATH_COUNT; ++i)
            vg::destroyPath(mClockPaths[i]);

        vg::destroyPaint(mWhite);
        vg::destroyPaint(mBlack);
        vg::destroyPaint(mRed);
#endif
    }

    void update(float dt)
    {
        ui::processZoomAndPan(mScale, mOffsetX, mOffsetY, mIsDragging);
    }

    void render()
    {
        __time64_t  curtime;
        tm          dt;

        _time64(&curtime);
        _gmtime64_s(&dt, &curtime);

        glTranslatef(mOffsetX, mOffsetY, 0.0f);
        glScalef(mScale, mScale, 1.0f);

#ifdef RASTER_ACTORS
        glAlphaFunc(GL_GREATER, 0.0f);
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPushMatrix();
        vg::drawImage(-w/2.0f, h/2.0f, w/2.0f, -h/2.0f, mTextures[IMG_BACKGROUND]);
        glPopMatrix();

        glPushMatrix();
        glRotatef(30.0f*(dt.tm_hour%12)+90.0f, 0.0f, 0.0f, 1.0f);
        vg::drawImage(-w/2.0f, h/2.0f, w/2.0f, -h/2.0f, mTextures[IMG_HOUR_HAND]);
        glPopMatrix();

        glPushMatrix();
        glRotatef(6.0f*dt.tm_min, 0.0f, 0.0f, 1.0f);
        vg::drawImage(-w/2.0f, h/2.0f, w/2.0f, -h/2.0f, mTextures[IMG_MINUTE_HAND]);
        glPopMatrix();

        glPushMatrix();
        glRotatef(6.0f*dt.tm_sec, 0.0f, 0.0f, 1.0f);
        vg::drawImage(-w/2.0f, h/2.0f, w/2.0f, -h/2.0f, mTextures[IMG_SECOND_HAND]);
        glPopMatrix();
#else
        glPushMatrix();
        glTranslatef(bkgOffsetX, bkgOffsetY, 0.0f);
        vg::drawPathA2C(mClockPaths[PATH_CLOCK_BKG],           mWhite);
        vg::drawPathA2C(mClockPaths[PATH_MARKERS_1],           mBlack);
        vg::drawPathA2C(mClockPaths[PATH_MARKERS_2_AND_3],     mBlack);
        vg::drawPathA2C(mClockPaths[PATH_CIRCLES_AND_CIPHERS], mBlack);
        glPopMatrix();

        glPushMatrix();
        glRotatef(30.0f*(dt.tm_hour%12), 0.0f, 0.0f, 1.0f);
        vg::drawPathA2C(mClockPaths[PATH_HOUR_HAND], mBlack);
        glPopMatrix();

        glPushMatrix();
        glRotatef(6.0f*dt.tm_min-90.0f, 0.0f, 0.0f, 1.0f);
        vg::drawPathA2C(mClockPaths[PATH_MINUTE_HAND], mBlack);
        glPopMatrix();

        glPushMatrix();
        glRotatef(6.0f*dt.tm_sec-90.0f, 0.0f, 0.0f, 1.0f);
        vg::drawPathA2C(mClockPaths[PATH_SECOND_HAND], mRed);
        glPopMatrix();
#endif
    }

    void recompilePrograms() {}

    void resize(int width, int height) {}
}
