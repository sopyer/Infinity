#include <fwk/fwk.h>
#include <time.h>

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

const char* imageNames[IMG_COUNT] = 
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
        glCreateTextures(IMG_COUNT, mTextures);

        for (int i=0; i<IMG_COUNT; ++i)
        {
            mTextures[i] = res::createTexture2D(imageNames[i]);
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

        gfx::set2DStates();
        gfx::setUIMatrices();

        v128 mv[4] = {
            vi_set(  mScale,     0.0f, 0.0f, 0.0f),
            vi_set(    0.0f,   mScale, 0.0f, 0.0f),
            vi_set(    0.0f,     0.0f, 1.0f, 0.0f),
            vi_set(mOffsetX, mOffsetY, 0.0f, 1.0f),
        };

        gfx::setModelViewMatrix(mv);

        v128 mvt[4], t[4];

#ifdef RASTER_ACTORS
        glAlphaFunc(GL_GREATER, 0.0f);
        glEnable(GL_ALPHA_TEST);

        vg::drawImage(-w/2.0f, h/2.0f, w/2.0f, -h/2.0f, mTextures[IMG_BACKGROUND]);

        ml::make_rotation_mat4(t, (30.0f*(dt.tm_hour%12)-90.0f) * FLT_DEG_TO_RAD_SCALE, 0.0f, 0.0f, 1.0f);
        ml::mul_mat4(mvt, mv, t);
        gfx::setModelViewMatrix(mvt);
        vg::drawImage(-w/2.0f, h/2.0f, w/2.0f, -h/2.0f, mTextures[IMG_HOUR_HAND]);

        ml::make_rotation_mat4(t, (6.0f*dt.tm_min - 180.0f) * FLT_DEG_TO_RAD_SCALE, 0.0f, 0.0f, 1.0f);
        ml::mul_mat4(mvt, mv, t);
        gfx::setModelViewMatrix(mvt);
        vg::drawImage(-w/2.0f, h/2.0f, w/2.0f, -h/2.0f, mTextures[IMG_MINUTE_HAND]);

        ml::make_rotation_mat4(t, (6.0f*dt.tm_sec - 180.0f) * FLT_DEG_TO_RAD_SCALE, 0.0f, 0.0f, 1.0f);
        ml::mul_mat4(mvt, mv, t);
        gfx::setModelViewMatrix(mvt);
        vg::drawImage(-w/2.0f, h/2.0f, w/2.0f, -h/2.0f, mTextures[IMG_SECOND_HAND]);
#else
        ml::make_translation_mat4(t, bkgOffsetX, bkgOffsetY, 0.0f);
        ml::mul_mat4(mvt, mv, t);
        gfx::setModelViewMatrix(mvt);
        vg::drawPath(mClockPaths[PATH_CLOCK_BKG],           mWhite, false, true);
        vg::drawPath(mClockPaths[PATH_MARKERS_1],           mBlack, false, true);
        vg::drawPath(mClockPaths[PATH_MARKERS_2_AND_3],     mBlack, false, true);
        vg::drawPath(mClockPaths[PATH_CIRCLES_AND_CIPHERS], mBlack, false, true);

        ml::make_rotation_mat4(t, 30.0f*(dt.tm_hour%12) * FLT_DEG_TO_RAD_SCALE, 0.0f, 0.0f, 1.0f);
        ml::mul_mat4(mvt, mv, t);
        gfx::setModelViewMatrix(mvt);
        vg::drawPath(mClockPaths[PATH_HOUR_HAND], mBlack, false, true);

        ml::make_rotation_mat4(t, (6.0f*dt.tm_min-90.0f) * FLT_DEG_TO_RAD_SCALE, 0.0f, 0.0f, 1.0f);
        ml::mul_mat4(mvt, mv, t);
        gfx::setModelViewMatrix(mvt);
        vg::drawPath(mClockPaths[PATH_MINUTE_HAND], mBlack, false, true);

        ml::make_rotation_mat4(t, (6.0f*dt.tm_sec-90.0f) * FLT_DEG_TO_RAD_SCALE, 0.0f, 0.0f, 1.0f);
        ml::mul_mat4(mvt, mv, t);
        gfx::setModelViewMatrix(mvt);
        vg::drawPath(mClockPaths[PATH_SECOND_HAND], mRed, false, true);
#endif

        ml::make_identity_mat4(mv);
        gfx::setModelViewMatrix(mv);

    }

    void recompilePrograms() {}

    void resize(int width, int height) {}
}
