#pragma once

#include <vg/openvg.h>

#include <VG/Path.h>
#include <VG/Paint.h>
#include <VG/SUI.h>

namespace vg
{
	struct FontOpaque;
	typedef FontOpaque* Font;

	enum TextAlign
	{
		TALIGN_LEFT = 0,
		TALIGN_RIGHT = 1,
		TALIGN_HCENTER = 2,
		TALIGN_TOP = 0,
		TALIGN_BOTTOM = 4,
		TALIGN_VCENTER = 8,
	};

	void init();
	void cleanup();

	Font createFont(const char* fontPath, size_t faceSize);
	Font createFont(const unsigned char* fontData, size_t dataSize, size_t faceSize);
	void destroyFont(Font font);

	float getTextAscender(Font font);
	float getTextDescender(Font font);
	float getTextVExtent(Font font);

	template<typename T> float	getTextHExtent(Font font, const T* str);
	template<typename T> void	calcTextBasePt(Font font, float w, float h, size_t flags, const T* str, float& bx, float& by);
	template<typename T> void	getBounds(Font font, const T* str, float& xmin, float& ymin, float& xmax, float& ymax);
	template<typename T> void	drawString(Font font, float x, float y, const T* str);

	void drawPath(Path path, Paint paint);
	void drawPathAA(Path path, Paint paint);
	void drawPathA2C(Path path, Paint paint);
	void drawPathNZ(Path path, Paint paint);
	void drawPathNZA2C(Path path, Paint paint);
	void drawPath(Path path, VGubyte red, VGubyte green, VGubyte blue, VGubyte alpha);
}
