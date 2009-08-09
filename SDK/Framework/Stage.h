#ifndef __STAGE_H_INCLUDED__
#	define __STAGE_H_INCLUDED__

#	include <Scheduler.h>
#	include <queue>

namespace UI
{
	class Stage: public Container, public sigslot::has_slots<>
	{
		public:
			Stage();
			
			Stage& queueRelayout();
			
			Stage& captureFocus(Actor* focused);
			Stage& releaseFocus();

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
			virtual void onKeyDown(const KeyEvent& event);
			virtual void onKeyUp(const KeyEvent& event);
			virtual void onTouch(const ButtonEvent& event);
			virtual void onUntouch(const ButtonEvent& event);
			virtual void onMotion(const MotionEvent& event);

			void enterPhase(Phase nextPhase);// {mPhase = nextPhase;}

		protected:
			VG				mVG;

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

			std::vector<RenderItem>		mRenderQueue;
	};
}

#endif