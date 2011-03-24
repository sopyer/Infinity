#pragma once

#include <UI.h>
#include <cstdlib>
#include <scheduler.h>
#include <vg/vg.h>

namespace ui
{
	inline bool isSame(ui::Actor* sender, ui::Actor* actor) {return sender==actor;}
	
	class Group: public ui::Container
	{
	};

	class Image: public ui::Actor
	{
		public:
			Image()  {}
			~Image() {}
			
			Image&	setTexture(GLuint texture) {mTexture = texture; return *this;}
			GLuint	getTexture() {return mTexture;}

		protected:
			virtual void onPaint();

		protected:
			GLuint		mTexture;
	};

	class Rectangle: public ui::Actor
	{
		protected:
			virtual void onPaint();
	};

	class Label: public ui::Actor
	{
		public:
			Label(): mColor(1.0f, 1.0f, 1.0f, 1.0f), mFont(ui::defaultFont) {}
			
			Label& setFont(sui::Font font) {mFont = font; queueRelayout(); return *this;}
			Label& setColor(const glm::vec4& color) {mColor = color; return *this;}
			Label& setText(const wchar_t* text) {mText = text; queueRelayout(); return *this;}

		protected:
			virtual void onPaint();
			virtual void onAllocate();

		protected:
			glm::vec4		mColor;
			sui::Font		mFont;
			std::wstring	mText;
	};

	class Edit: public Label, public sigslot::has_slots<>
	{
		public:
			Edit(): mCaretPos(0), mShowCursor(false), mBlinkTask(0)
			{
			}
		
			~Edit()
			{
				hideCursor();
			}

		protected:
			void onBlink()
			{
				mShowCursor = !mShowCursor;
			}

			virtual void onTouch(const ButtonEvent& event)
			{
				getStage()->captureFocus(this);
			}
			
			virtual void onFocusIn()
			{
				showCursor();
			}

			virtual void onFocusOut()
			{
				hideCursor();
			}

			virtual void onKeyDown(const KeyEvent& event);
			virtual void onPaint();

			void showCursor();
			void hideCursor();
			void resetCursor();

		private:
			mt::Task	mBlinkTask;
			
			bool	mShowCursor;
			int		mCaretPos;
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

	class HGroup: public ui::Container
	{
		public:
			HGroup()
			{
				setSize(100, 30);

				m1.setText(L"Menu1")
					.setSize(100, 20);
				m2.setText(L"Menu2")
					.setSize(100, 20);
				m3.setText(L"Menu3")
					.setSize(100, 20);

				add(m1).add(m2).add(m3);
			}

		protected:
			void onAllocate()
			{
				float x = 0, y = 0, w = getWidth();
				ChildsVector::iterator	it = mChilds.begin(),
										end = mChilds.end();
				for (; it != end; ++it)
				{
					Actor&	actor = *(*it);
					actor.setPos(0, y)
						.setWidth(w);
					y += actor.getHeight();
				}
				setSize(w, y);
			}

		private:
			Button	m1;
			Button	m2;
			Button	m3;
	};

	class Layout: public ui::Container, public sigslot::has_slots<>
	{
		public:
			Layout()
			{
				setSize(100, 30);

				add
				(
					mEdit.setText(L"Edit me!!!")
						.setSize(80, 20)
						.setPos(0,0)
				)
				.
				add
				(
					mButton.setText(L"^")
						.setSize(20, 20)
						.setPos(80,0)
				)
				.
				add
				(
					mListBox.setSize(100, 60)
						.setPos(0,20)
						.hide()
				);

				mButton.onClicked.connect(this, &Layout::expandCollapseList);
			}

			void expandCollapseList(Actor*)
			{
				if (mListBox.isVisible())
				{
					mListBox.hide();
				}
				else
				{
					mListBox.show();
				}
			}

			void onAllocate()
			{
				float w = getWidth();
				float h = getHeight();

				mButton.setSize(h,h)
					.setPos(w-h, 0);
				mEdit.setSize(w-h, h);
				mListBox.setWidth(w)
					.setPos(0, h);

				ui::Container::onAllocate();
			}

		private:
			Button	mButton;
			Edit	mEdit;
			HGroup	mListBox;
	};

	class VBox: public ui::Container
	{
		public:
			enum Align {Left, Center, Right};
		
		public:
			VBox():mSpacing(0), mHMargin(0), mVMargin(0), mAlign(Left)
			{}

			VBox& setSpacing(float spacing) {mSpacing = spacing; return *this;}
			VBox& setHMargin(float hmargin) {mHMargin = hmargin; return *this;}
			VBox& setVMargin(float vmargin) {mVMargin = vmargin; return *this;}
			VBox& setAlign(Align align) {mAlign = align; return *this;}

		protected:
			virtual void onAllocate();
			virtual void onPaint();

		private:
			float	mSpacing, mHMargin, mVMargin;
			Align	mAlign;
	};

	class ProfileStatsBox: public VBox
	{
		public:
			ProfileStatsBox();

			ProfileStatsBox& setFont(sui::Font font);
			ProfileStatsBox& setStats(float cpuTime, float gpuTime);

		private:
			ui::Label	mVCNameLabel;
			ui::Label	mCPUTimeLabel;
			ui::Label	mGPUTimeLabel;
	};
}