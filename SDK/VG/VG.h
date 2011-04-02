#pragma once

#include <vg/openvg.h>

namespace impl
{
	struct Geometry;
}

namespace vg
{
	struct FontOpaque;

	typedef FontOpaque*		Font;
	typedef VGuint			Paint;
	typedef impl::Geometry* Path;

	enum TextAlign
	{
		TALIGN_LEFT = 0,
		TALIGN_RIGHT = 1,
		TALIGN_HCENTER = 2,
		TALIGN_TOP = 0,
		TALIGN_BOTTOM = 4,
		TALIGN_VCENTER = 8,
	};

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

	//Startup/shutdown API
	void init();
	void cleanup();

	//Font API
	Font	createFont(const char* fontPath, size_t faceSize);
	Font	createFont(const unsigned char* fontData, size_t dataSize, size_t faceSize);
	void	destroyFont(Font font);
	float	getTextAscender(Font font);
	float	getTextDescender(Font font);
	float	getTextVExtent(Font font);

	template<typename T> float	getTextHExtent(Font font, const T* str);
	template<typename T> void	calcTextBasePt(Font font, float w, float h, size_t flags, const T* str, float& bx, float& by);
	template<typename T> void	getBounds(Font font, const T* str, float& xmin, float& ymin, float& xmax, float& ymax);
	template<typename T> void	drawString(Font font, float x, float y, const T* str);

	//Paint API
	Paint	createSolidPaint(float* color4f);
	Paint	createSolidPaint(unsigned int color);
	void	destroyPaint(Paint paint);
	void	applyPaintAsGLProgram(Paint paint);

	//Path API
	Path createPath(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData);
	Path createUnitQuad();
	void destroyPath(Path path);
	void getPathBounds(Path path, float& x1, float& y1, float& x2, float& y2);

	//Drawing API
	void drawPath(Path path, Paint paint);
	void drawPathAA(Path path, Paint paint);
	void drawPathA2C(Path path, Paint paint);
	void drawPathNZ(Path path, Paint paint);
	void drawPathNZA2C(Path path, Paint paint);
	void drawPath(Path path, VGubyte red, VGubyte green, VGubyte blue, VGubyte alpha);

	//SUI API
	void drawRect(float x0, float y0, float x1, float y1, VGuint fillColor, VGuint borderColor);
    void drawRoundedRect(float x0, float y0, float x1, float y1, float cx, float cy, VGuint fillColor, VGuint borderColor);
	void drawRoundedRectOutline(float x0, float y0, float x1, float y1, float cx, float cy, VGuint borderColor);

	void drawRectCOLORID(float x0, float y0, float x1, float y1, int fillColor, int borderColor);
	void drawFrame(float x0, float y0, float x1, float y1, float cx, float cy, bool isHover = false, bool isOn = false, bool isFocus = false);
	void drawBoolFrame(float x0, float y0, float x1, float y1, float cx, float cy, bool isHover = false, bool isOn = false, bool isFocus = false);
    void drawRoundedRectCOLORID(float x0, float y0, float x1, float y1, float cx, float cy, int fillColorId, int borderColorId);
	void drawRoundedRectOutlineCOLORID(float x0, float y0, float x1, float y1, float cx, float cy, int borderColor);
 //   void drawCircle( const Rect& rect, int fillColorId, int borderColorId );
 //   void drawMinus( const Rect& rect, int width, int fillColorId, int borderColorId );
 //   void drawPlus( const Rect& rect, int width, int fillColorId, int borderColorId );
 //   void drawDownArrow( const Rect& rect, int width, int fillColorId, int borderColorId );
 //   void drawUpArrow( const Rect& rect, int width, int fillColorId, int borderColorId );
}
