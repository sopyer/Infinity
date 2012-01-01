#ifndef __STAGE_H_INCLUDED__
#	define __STAGE_H_INCLUDED__

#	include <Scheduler.h>
#	include <queue>

namespace ui
{
    class Stage: public Container, public sigslot::has_slots<>
    {
    public:
        Stage();

        Stage& queueRelayout();

        Stage& captureFocus(Actor* focused);
        Stage& releaseFocus();

        Stage& setProjection(float* mat4x4);

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
        void processKeyDown(const KeyEvent& event);
        void processKeyUp(const KeyEvent& event);
        void processTouch(const ButtonEvent& event);
        void processUntouch(const ButtonEvent& event);
        void processMotion(const MotionEvent& event);

        void enterPhase(Phase nextPhase);// {mPhase = nextPhase;}

    protected:
        //VG				mVG;

    private:
        struct RenderItem
        {
            glm::mat4	transform;
            Actor*		actor;
        };

    private:
        Actor* doPick(u32 x, u32 y);

        void outlineActors();
        void renderActors();
        void updateRenderQueue(Container* container, const glm::mat4& parentTransform);

    private:
        bool	mDoAllocate;
        Phase	mPhase;

        Actor*	mLastVisited;
        Actor*	mFocused;

        glm::mat4	mProjection;

        std::vector<RenderItem>		mRenderQueue;
#if defined(DEBUG) || defined(_DEBUG)
        int		dumpPickImage:1;
#endif
    };
}

#endif