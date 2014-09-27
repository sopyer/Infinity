#include "nanosvg.h"
#include <fwk/fwk.h>
#include <vector>
#include <algorithm>

namespace app
{
    cpu_timer_t      cpuTimer;
    gfx::gpu_timer_t gpuTimer;

    ui::CheckBoxID mAAEnabled;

    bool  mIsDragging = false;
    float mOffsetX = 128.0f;
    float mOffsetY = -18.0f;
    float mScale   = 2.5f;

    std::vector<vg::Path>   mPaths;
    std::vector<vg::Paint>  mPaints;

    void init()
    {
        const char* svgFile = "butterfly.svg";
        SVGPath* plist;
        plist = svgParseFromFile(svgFile);
        SVGPath* cur = plist;

        while (cur && cur->hasFill)
        {
            vg::Path path = vg::createPath(cur->numCmd, cur->cmd, cur->numData, cur->data);
            mPaths.push_back(path);

            vg::Paint paint = vg::createSolidPaint(cur->fillColor);
            mPaints.push_back(paint);

            cur = cur->next;
        }

        svgDelete(plist);

        mAAEnabled = ui::checkBoxAdd(25.0f, 83.0f, 41.0f, 99.0f, FALSE);

        gfx::gpu_timer_init(&gpuTimer);

        fwk::setCaption("GPU accelerated SVG rendering");
    }

    void fini()
    {
        gfx::gpu_timer_fini(&gpuTimer);

        struct DeletePath
        {void operator ()(vg::Path path) {vg::destroyPath(path);}};

        for_each(mPaths.begin(), mPaths.end(), DeletePath());

        struct DeletePaint
        {void operator ()(vg::Paint paint) {vg::destroyPaint(paint);}};

        for_each(mPaints.begin(), mPaints.end(), DeletePaint());
    }

    void update(float /*dt*/)
    {
        ui::processZoomAndPan(mScale, mOffsetX, mOffsetY, mIsDragging);
    }

    void render()
    {
        cpu_timer_start(&cpuTimer);
        gfx::gpu_timer_start(&gpuTimer);

        glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gfx::set2DStates();
        gfx::setUIMatrices();

        v128 mv[4] = {
            vi_set(  mScale,     0.0f, 0.0f, 0.0f),
            vi_set(    0.0f,   mScale, 0.0f, 0.0f),
            vi_set(    0.0f,     0.0f, 1.0f, 0.0f),
            vi_set(mOffsetX, mOffsetY, 0.0f, 1.0f),
        };

        gfx::setModelViewMatrix(mv);

        if (ui::checkBoxIsChecked(mAAEnabled))
        {
            for (size_t i=mPaths.size(); i!=0; i--)
                vg::drawPath(mPaths[i-1], mPaints[i-1], true, true);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
            for (size_t i=mPaths.size(); i!=0; i--)
                vg::drawPath(mPaths[i-1], mPaints[i-1], true, false);
            glEnable(GL_MULTISAMPLE);
        }

        cpu_timer_stop(&cpuTimer);
        gfx::gpu_timer_stop(&gpuTimer);

        ml::make_identity_mat4(mv);
        gfx::setModelViewMatrix(mv);

        ui::displayStats(
            10.0f, 10.0f, 300.0f, 100.0f,
            cpu_timer_measured(&cpuTimer) / 1000.0f,
            gfx::gpu_timer_measured(&gpuTimer) / 1000.0f
        );

        vg::drawString(vg::defaultFont, 46.0f, 96.0f, 0xFFFFFFFF, "Enable AA", 9);
    }

    void recompilePrograms() {}

    void resize(int width, int height) {}
}
