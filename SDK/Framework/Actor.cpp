#include <opengl.h>
#include "UI.h"
#include "Actor.h"
#include "Container.h"
#include "Stage.h"

namespace ui
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
		mParent = parent;

		//if (mParent)
		//	dynamic_cast<Container*>(mParent)->remove(this);

		return *this;
	}

	void Actor::onPick(Color color)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glColor4ub(color.red, color.green, color.blue, color.alpha);
		glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(mWidth,0);
		glVertex2f(mWidth,mHeight);
		glVertex2f(0,mHeight);
		glEnd();
		glPopAttrib();
	}
}