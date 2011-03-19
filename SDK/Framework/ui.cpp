#include "UI.h"
#include "Scheduler.h"

#include "DefaultFontData.h"

namespace ui
{
	sui::Font defaultFont;

	void init()
	{
		defaultFont = sui::createFont(anonymousProBTTF, sizeof(anonymousProBTTF), 16);
	}

	void cleanup()
	{
		sui::destroyFont(defaultFont);
	}
}
