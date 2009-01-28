#define GLEW_STATIC
#include <gl\glew.h>
#include "UI.h"

namespace UI
{
	Actor&	Actor::queueRelayout()
	{
		getStage()->queueRelayout();
		return *this;
	}

	Stage*	Actor::getStage()
	{
		return Stage::getPtr();
	}
}