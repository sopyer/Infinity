#include "UI.h"
#include "Scheduler.h"

namespace ui
{
	void init()
	{
		scheduler::init();
	}

	void cleanup()
	{
		scheduler::cleanup();
	}
}
