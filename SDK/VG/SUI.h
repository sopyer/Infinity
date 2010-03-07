#ifndef __SUI_H_INCLUDED__
#	define __SUI_H_INCLUDED__

#	include <VG/VGCommon.h>

namespace sui
{
	enum Color
	{
		cBase = 0,
		cBool = 4,
		cOutline = 8,
		cFont = 12,
		cFontBack = 16,
		cTranslucent = 20,
		cNbColors = 24,
	};
	
	enum TextAlign
	{
		TALIGN_LEFT = 0,
		TALIGN_RIGHT = 1,
		TALIGN_HCENTER = 2,
		TALIGN_TOP = 0,
		TALIGN_BOTTOM = 4,
		TALIGN_VCENTER = 8,
	};

	struct Font
	{
		void setSize(VGuint size);

		FTFont*	mHandle;
	};

	GLfloat	getTextAscender(Font font);
	GLfloat	getTextDescender(Font font);
	GLfloat	getTextVExtent(Font font);
	GLfloat	getTextHExtent(Font font, const wchar_t* text);
	
	void	calcTextBasePt(
		Font font, float w, float h, VGuint flags,
		const wchar_t* text, float& bx, float& by
	);

	Font	createFont(const char* name);
	void	destroyFont(Font font);

	void drawText(Font font, GLfloat x, GLfloat y, const wchar_t* text);

	void drawRect(
		float x0, float y0, float x1, float y1,
		int fillColor, int borderColor
	);
	void drawFrame(
		float x0, float y0, float x1, float y1, float cx, float cy,
		bool isHover = false, bool isOn = false, bool isFocus = false
	);
	void drawBoolFrame(
		float x0, float y0, float x1, float y1, float cx, float cy,
		bool isHover = false, bool isOn = false, bool isFocus = false
	);
    void drawRoundedRect(
		float x0, float y0, float x1, float y1, float cx, float cy,
		int fillColorId, int borderColorId
	);
	void drawRoundedRectOutline(
		float x0, float y0, float x1, float y1, float cx, float cy,
		int borderColor);
 //   void drawCircle( const Rect& rect, int fillColorId, int borderColorId );
 //   void drawMinus( const Rect& rect, int width, int fillColorId, int borderColorId );
 //   void drawPlus( const Rect& rect, int width, int fillColorId, int borderColorId );
 //   void drawDownArrow( const Rect& rect, int width, int fillColorId, int borderColorId );
 //   void drawUpArrow( const Rect& rect, int width, int fillColorId, int borderColorId );
}

#endif