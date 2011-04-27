#pragma once

#include <vg/openvg.h>

namespace impl
{
	struct Geometry;
}

namespace vg
{
	struct FontOpaque;
	struct PaintOpaque;

	typedef FontOpaque*		Font;
	typedef PaintOpaque*	Paint;
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
	Paint	createLinearGradientPaint(float x0, float y0, float x1, float y1,
									  size_t stopCount, float stops[],
									  unsigned int colorRamp[]);
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

 //   void drawCircle( const Rect& rect, int fillColorId, int borderColorId );
}
