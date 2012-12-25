#pragma once

#include <UI.h>
#include <cstdlib>
#include <scheduler.h>
#include <vg/vg.h>

namespace ui
{
	inline bool isSame(ui::Actor* sender, ui::Actor* actor) {return sender==actor;}
	
	class Label: public ui::Actor
	{
		public:
			Label(): mColor(1.0f, 1.0f, 1.0f, 1.0f), mFont(ui::defaultFont) {}
			
			Label& setFont(vg::Font font) {mFont = font; queueRelayout(); return *this;}
			Label& setColor(const glm::vec4& color) {mColor = color; return *this;}
			Label& setText(const wchar_t* text) {mText = text; queueRelayout(); return *this;}

		protected:
			virtual void onPaint();
			virtual void onAllocate();

		protected:
			glm::vec4		mColor;
			vg::Font		mFont;
			std::wstring	mText;
	};

	class Button: public Label
	{
		public:
			Button(): mIsPressed(false), mIsHover(false) {}
			
		public:
			sigslot::signal1<Actor*>	onClicked;

		protected:
			virtual void onTouch(const ButtonEvent& event)
			{mIsPressed = true; onClicked.emit(this);}

			virtual void onUntouch(const ButtonEvent& event)
			{mIsPressed = false;}

			virtual void onEnter()
			{mIsHover = true;}

			virtual void onLeave()
			{mIsHover = false; mIsPressed = false;}

			virtual void onPaint();

		protected:
			bool		mIsHover;
			bool		mIsPressed;
	};

	class CheckBox: public Button
	{
		public:
			CheckBox(): mIsChecked(false) {}
			
			bool isChecked() {return mIsChecked;}
			CheckBox& setChecked(bool checked) {mIsChecked = checked; return *this;}

		protected:
			virtual void onTouch(const ButtonEvent& event);
			virtual void onPaint();

		private:
			bool		mIsChecked;
	};

}