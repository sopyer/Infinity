#ifndef __STAGE_H_INCLUDED__
#   define __STAGE_H_INCLUDED__

#   include <Scheduler.h>
#   include <queue>
#   include <ml.h>

namespace ui
{
    class Stage
    {
    public:
        Stage();

        void run();
        void setCaption(const char* caption);

    protected:
        //Debug functions and callbacks
        void addPrograms(size_t count, GLuint* programs);

        virtual void onShaderRecompile() {}
        virtual void onUpdate(float /*dt*/) {}
        virtual void onPaint() {}

    protected:
        bool    mFullscreen;
        float   mWidth, mHeight;

    private:
        void renderActors();

    private:

        uint64_t            mPrevTime;
    };
}

#endif