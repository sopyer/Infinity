//#define GLEW_STATIC
#include <gl\glee.h>
#include "UI.h"
#include "Actor.h"
#include "Container.h"
#include "Stage.h"

namespace UI
{
	Actor&	Actor::queueRelayout()
	{
		if (Stage* stage = getStage())
			stage->queueRelayout();

		return *this;
	}

	Stage*	Actor::getStage()
	{
		Actor* parent = getParent();
		Actor* p = 0;

		while (parent && (p = parent->getParent()))
			parent = p;

		return dynamic_cast<Stage*>(parent);
	}

	Actor&	Actor::setParent(Actor* parent)
	{
		if (mParent)
			dynamic_cast<Container*>(mParent)->remove(this);

		mParent = parent;

		return *this;
	}

	void Actor::onPick(Color color)
	{
		glColor4ub(color.red, color.green, color.blue, color.alpha);
		glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(mWidth,0);
		glVertex2f(mWidth,mHeight);
		glVertex2f(0,mHeight);
		glEnd();
	}
}