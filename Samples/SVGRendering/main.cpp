#include "nanosvg.h"
#include <framework.h>
#include "ResourceHelpers.h"
#include <time.h>
#include <vector>

class SVGSample: public ui::Stage
{
public:
    SVGSample(const char* svgFile):
          mOffsetX(128/*mWidth/2*/),
          mOffsetY(-18/*mHeight/2*/),
          mScale(2.5f),
          mIsDragging(false),
          mDrawTimeCPU(0.0f),
          mDrawTimeGPU(0.0f)
    {
        VFS::mount("../../AppData");

        SVGPath* plist;
        plist = svgParseFromFile(svgFile);
        SVGPath* cur = plist;

        while (cur && cur->hasFill)
        {
            vg::Path path = vg::createPath(cur->count, cur->commands, cur->data);
            mPaths.push_back(path);

            unsigned int col = 0xFFFFFFFF;
            vg::Paint paint = vg::createSolidPaint(cur->fillColor);
            mPaints.push_back(paint);

            cur = cur->next;
        }

        svgDelete(plist);

        mAAEnabled = ui::checkBoxAdd(25.0f, 83.0f, 41.0f, 99.0f, FALSE);

        glGenQueries(1, &mTimeQuery);

        SDL_WM_SetCaption("GPU accelerated SVG rendering", NULL);
    }

    ui::CheckBoxID mAAEnabled;

    ~SVGSample()
    {
        glDeleteQueries(1, &mTimeQuery);

        struct DeletePath
        {void operator ()(vg::Path path) {vg::destroyPath(path);}};

        for_each(mPaths.begin(), mPaths.end(), DeletePath());

        struct DeletePaint
        {void operator ()(vg::Paint paint) {vg::destroyPaint(paint);}};

        for_each(mPaints.begin(), mPaints.end(), DeletePaint());
    }

protected:
    void onTouch(const ButtonEvent& event)
    {
        if (event.button == SDL_BUTTON_WHEELUP)
        {
            mScale *= 1.2f;
            mOffsetX -= (event.x-mOffsetX)*(1.2f - 1);
            mOffsetY -= (event.y-mOffsetY)*(1.2f - 1);
        }
        else if (event.button == SDL_BUTTON_WHEELDOWN)
        {
            mScale /= 1.2f;
            if (mScale<1.0f)
            {
                //fix it a bit
                mOffsetX -= (event.x-mOffsetX)*(1/mScale/1.2f - 1);
                mOffsetY -= (event.y-mOffsetY)*(1/mScale/1.2f - 1);
                mScale = 1.0f;
            }
            else
            {
                mOffsetX -= (event.x-mOffsetX)*(1/1.2f - 1);
                mOffsetY -= (event.y-mOffsetY)*(1/1.2f - 1);
            }
        }
        else
            mIsDragging = true;
    }

    void onUntouch(const ButtonEvent& event)
    {
        mIsDragging = false;
    }

    void onMotion(const MotionEvent& event)
    {
        if (mIsDragging)
        {
            mOffsetX += event.xrel;
            mOffsetY += event.yrel;
        }
    }

    void onPaint()
    {
        CPUTimer drawTimer;
        drawTimer.start();

        glBeginQuery(GL_TIME_ELAPSED, mTimeQuery);

        glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(mOffsetX, mOffsetY, 0);
        glScalef(mScale, mScale, 1);

        if (ui::checkBoxIsChecked(mAAEnabled))
        {
            for (size_t i=mPaths.size(); i!=0; i--)
                vg::drawPathNZA2C(mPaths[i-1], mPaints[i-1]);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
            for (size_t i=mPaths.size(); i!=0; i--)
                vg::drawPathNZ(mPaths[i-1], mPaints[i-1]);
            glEnable(GL_MULTISAMPLE);
        }

        glPopMatrix();
        glEndQuery(GL_TIME_ELAPSED);

        mDrawTimeCPU = (float)(0.8f*drawTimer.elapsed()+0.2f*mDrawTimeCPU);

        GLuint64EXT result;
        glGetQueryObjectui64v(mTimeQuery, GL_QUERY_RESULT, &result);
        mDrawTimeGPU = 0.8f*result/1000.0f/1000.0f+0.2f*mDrawTimeGPU;

        ui::displayStats(10.0f, 10.0f, 300.0f, 100.0f, mDrawTimeCPU, mDrawTimeGPU);

        glColor3f(1.0f, 1.0f, 1.0f);
        vg::drawString(ui::defaultFont, 46.0f, 96.0f, "Enable AA", 9);
    }

private:
    GLuint		mTimeQuery;
    VFS			mVFS;
    bool		mIsDragging;
    float		mOffsetX;
    float		mOffsetY;
    float		mScale;

    float		mDrawTimeCPU;
    float		mDrawTimeGPU;

    std::vector<vg::Path>	mPaths;
    std::vector<vg::Paint>	mPaints;	
};

int main(int argc, char** argv)
{
    SVGSample app(argc==2?argv[1]:"butterfly.svg");
    app.run();

    return 0;
}
