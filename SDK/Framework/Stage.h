#ifndef __STAGE_H_INCLUDED__
#   define __STAGE_H_INCLUDED__

#   include <Scheduler.h>
#   include <queue>
#   include <ml.h>

struct SDL_Surface;

class  ShaderEditOverlay;
class  ProfilerOverlay;

namespace ui
{
    class Stage
    {
    public:
        Stage();
        ~Stage();

        void run();
        void close();
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
        void handleInput();

    private:
        v128 mProj[4];
        bool mRunLoop;

        size_t              mState;
        size_t              mProfilerState;

        uint64_t            mPrevTime;

        ShaderEditOverlay*  mShaderEditOverlay;
        ProfilerOverlay*    mProfilerOverlay;
    };
}

#endif