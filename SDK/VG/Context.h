#pragma once

#include <vector>
#include <glm/glm.h>
#include <gl/glee.h>
#include <vg/openvg.h>

#include <VG/Path.h>
#include <VG/Paint.h>
#include <VG/VGCommon.h>

namespace vg
{
	struct Font: public Handle<FTFont*>
	{
		void	setFaceSize(VGuint size);
	};

	Font	createFont(const char* name);
	void	destroyFont(Font font);
	void	drawString(Font font, float x, float y, const char* str);
	void	drawString(Font font, float x, float y, const wchar_t* str);

	void	drawPath(Path path, Paint paint);
	void	drawPathAA(Path path, Paint paint);
	void	drawPathA2C(Path path, Paint paint);

	void	drawPathNZ(Path path, Paint paint);
	void	drawPathNZA2C(Path path, Paint paint);

	void	drawPath(Path path, VGubyte red, VGubyte green, VGubyte blue, VGubyte alpha);
}