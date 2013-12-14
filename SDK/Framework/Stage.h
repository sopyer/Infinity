#ifndef __STAGE_H_INCLUDED__
#	define __STAGE_H_INCLUDED__

#	include <Scheduler.h>
#	include <queue>
#   include <Events.h>
#   include <glm/glm.h>
#   include <glm/glmext.h>

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

        Stage& setProjection(float* mat4x4);

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
        bool	mFullscreen;
		float   mWidth, mHeight;

    private:
        void outlineActors();
        void renderActors();
        void handleInput();

    private:
        glm::mat4   mProjection;
        bool        mRunLoop;
#if defined(DEBUG) || defined(_DEBUG)
        int dumpPickImage:1;
#endif

        size_t              mState;

        uint64_t            mPrevTime;

        ShaderEditOverlay*  mShaderEditOverlay;
        ProfilerOverlay*    mProfilerOverlay;

        bool doFrameCapture;
        bool doTimesliceCapture;
    };
}

#endif