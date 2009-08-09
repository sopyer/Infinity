#pragma once

#include <UI.h>
#include <glm\glm.h>

class Interpolator: public sigslot::has_slots<>
{
public:
	virtual float getValue(float value)
	{
		return glm::clamp(value, 0.0f, 1.0f);
	}
};

class Animation: public sigslot::has_slots<>
{
public:
	Animation& apply(UI::Actor* actor)
	{
		mAnimatedActors.push_back(actor); return *this;
	}

	Animation& setTimeline(Timeline* timeline)
	{
		disconnect_all();
		mTimeline = timeline;

		if (!mTimeline)
			return *this;

		mTimeline->onFrame.connect(this, &Animation::onFrame);
		mTimeline->onStarted.connect(this, &Animation::onStarted);
		mTimeline->onCompleted.connect(this, &Animation::onCompleted);

		return *this;
	}

	Animation& setInterpolator(Interpolator* interpolator)
	{
		mInterpolator = interpolator; return *this;
	}

protected:
	virtual void applyBehaviour(UI::Actor* actor, float step) = 0;

private:
	void onFrame(u32 frame)
	{
		float step = (float)mTimeline->getElapsed() / mTimeline->getDuration();
		forEach(step);
	}

	void onCompleted()
	{
		float step = (float)mTimeline->getElapsed() / mTimeline->getDuration();
		//forEach(/*1.0f*/step);
	}

	void onStarted()
	{
		forEach(0.0f);
	}

	void forEach(float step)
	{
		std::vector<UI::Actor*>::iterator	it  = mAnimatedActors.begin(),
											end = mAnimatedActors.end();
		for(; it != end; ++it)
		{
			applyBehaviour(*it, step);
		}
	}

private:
	std::vector<UI::Actor*>	mAnimatedActors;
	Interpolator*		mInterpolator;
	Timeline*			mTimeline;
};

class OffsetAnimation: public Animation
{
public:
	OffsetAnimation& setOffset(float dx, float dy)
	{
		mDX = dx;
		mDY = dy;
		return *this;
	}

protected:
	virtual void applyBehaviour(UI::Actor* actor, float step)
	{
		if (step == 0)
		{
			mStartX = actor->getX();
			mStartY = actor->getY();
			return;
		}

		actor->setPos(mStartX + mDX*step, mStartY + mDY*step);
	}

private:
	float	mDX, mDY;
	float	mStartX, mStartY;
};