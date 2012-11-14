#ifndef __STAGE_H_INCLUDED__
#	define __STAGE_H_INCLUDED__

#	include <Scheduler.h>
#	include <queue>
#   include <Events.h>

struct SDL_Surface;

class  ShaderEditOverlay;
class  ProfilerOverlay;

namespace ui
{
    class Stage: public Container, public sigslot::has_slots<>
    {
    public:
        Stage();
        ~Stage();

        Stage& queueRelayout();

        Stage& captureMouse();
        Stage& releaseMouse();

        Stage& captureFocus(Actor* focused);
        Stage& releaseFocus();

        Stage& setProjection(float* mat4x4);

        void run();
        void close();

        bool isMouseCaptured();

    protected:
        enum Phase
        {
            PHASE_DEFAULT,
            PHASE_ALLOCATE,
            PHASE_EVENT_HANDLING,
            PHASE_EVENT_HANDLING_PICK,
            PHASE_RENDERING,
        };

    protected:
        //Debug functions and callbacks
        void addPrograms(size_t count, GLuint* programs);

        virtual void onShaderRecompile() {}
        virtual void onUpdate(float dt) {}

        void createGLResources();
        void destroyGLResources();

        void processKeyDown(const KeyEvent& event);
        void processKeyUp(const KeyEvent& event);
        void processTouch(const ButtonEvent& event);
        void processUntouch(const ButtonEvent& event);
        void processMotion(const MotionEvent& event);

        void enterPhase(Phase nextPhase);// {mPhase = nextPhase;}

        Actor* doPick(uint32_t x, uint32_t y);

    protected:
        bool	mFullscreen;

    private:
        struct RenderItem
        {
            glm::mat4	transform;
            Actor*		actor;
        };

    private:
        void outlineActors();
        void renderActors();
        void updateRenderQueue(Container* container, const glm::mat4& parentTransform);
        void handleInput();
        void handleRender();
        void frameStep();

    private:
        GLuint  mPickFBO;
        GLuint  mPickIDRB;
        GLuint  mPickZRB;
        GLuint  mPickProgram;
        GLint   mColorLoc;

        bool	mDoAllocate;
        Phase	mPhase;

        Actor*	mLastVisited;
        Actor*	mFocused;

        glm::mat4	mProjection;

        std::vector<RenderItem>		mRenderQueue;

#if defined(DEBUG) || defined(_DEBUG)
        int		dumpPickImage:1;
#endif

        size_t				mState;

        uint64_t            mPrevTime;

        ShaderEditOverlay*	mShaderEditOverlay;
        ProfilerOverlay*	mProfilerOverlay;

        SDL_Surface			*mScreen;
    };
}

#endif