#include "UI.h"
#include "Scheduler.h"

#include "DefaultFontData.h"

namespace ui
{
	vg::Font defaultFont;

	void init()
	{
		defaultFont = vg::createFont(anonymousProBTTF, sizeof(anonymousProBTTF), 16);
	}

	void cleanup()
	{
		vg::destroyFont(defaultFont);
	}
}
