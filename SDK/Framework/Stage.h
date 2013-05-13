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

        Stage& captureMouse();
        Stage& releaseMouse();

        Stage& setProjection(float* mat4x4);

        void run();
        void close();

        bool isMouseCaptured();

    protected:
        //Debug functions and callbacks
        void addPrograms(size_t count, GLuint* programs);

        virtual void onShaderRecompile() {}
        virtual void onUpdate(float dt) {}
        virtual void onPaint() {}
        virtual void onTouch(const ButtonEvent& /*event*/) {}
        virtual void onUntouch(const ButtonEvent& /*event*/) {}
        virtual void onMotion(const MotionEvent& /*event*/) {}
        virtual void onKeyDown(const KeyEvent& /*event*/) {}
        virtual void onKeyUp(const KeyEvent& /*event*/) {}

    protected:
        bool	mFullscreen;
		float   mWidth, mHeight;

    private:
        void outlineActors();
        void renderActors();
        void handleInput();
        void frameStep();

    private:
        glm::mat4   mProjection;

#if defined(DEBUG) || defined(_DEBUG)
        int dumpPickImage:1;
#endif

        size_t              mState;

        uint64_t            mPrevTime;

        ShaderEditOverlay*  mShaderEditOverlay;
        ProfilerOverlay*    mProfilerOverlay;

        bool switchCapture;
    };
}

#endif