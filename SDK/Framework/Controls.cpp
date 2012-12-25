#include "Controls.h"

#pragma once

#include <UI.h>
#include <cstdlib>
#include <scheduler.h>
#include <VG/VG.h>

namespace ui
{
	void Label::onPaint()
	{
		glColor4fv(mColor);
		float y = vg::getTextAscender(mFont)+vg::getTextDescender(mFont);
		vg::drawString(mFont, 0, y, mText.c_str(), mText.length());
	}

	void Label::onAllocate()
	{
		mHeight = vg::getTextVExtent(mFont);
		mWidth  = vg::getTextHExtent(mFont, mText.c_str());
	}

	void Button::onPaint()
	{
		static const uint32_t colors[] = {
			0xB2595959,
			0xCCA6A6A6,
			0x7F3CE4D4,
			0x7F7FEDE3,
		};
		vg::drawRoundedRect(0, 0, mWidth, mHeight, 5, 5, colors[(mIsPressed<<1)+mIsHover], 0xFFFFFFFF);
		glColor4fv(mColor);
		float x = vg::getTextHExtent(mFont, mText.c_str());
		float y = vg::getTextAscender(mFont)+vg::getTextDescender(mFont);
		vg::drawString(mFont, (mWidth-x)/2, (mHeight+y)/2, mText.c_str(), mText.length());
	}

	void CheckBox::onTouch(const ButtonEvent& event)
	{
		mIsChecked = !mIsChecked;
		Button::onTouch(event);
	}

	void CheckBox::onPaint()
	{
		static const uint32_t colors[] = {
			0xFF232563,
			0xFF4547A5,
			0xFF3CE484,
			0xFF7FFDA3,
		};
		vg::drawRoundedRect(0, 0, 16, 16, 2.5f, 2.5f, colors[(mIsChecked<<1)+mIsHover], 0xFF00FF32);
		glColor4fv(mColor);
		float x = 20;
		float y = vg::getTextAscender(mFont)+vg::getTextDescender(mFont);
		y = (16+y)/2;
		vg::drawString(mFont, x, y, mText.c_str(), mText.length());
	}
}