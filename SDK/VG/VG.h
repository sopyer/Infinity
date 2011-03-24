#pragma once

#include <vg/openvg.h>

#include <VG/Path.h>
#include <VG/Paint.h>
#include <VG/SUI.h>

namespace vg
{
	void init();
	void cleanup();

	void drawPath(Path path, Paint paint);
	void drawPathAA(Path path, Paint paint);
	void drawPathA2C(Path path, Paint paint);
	void drawPathNZ(Path path, Paint paint);
	void drawPathNZA2C(Path path, Paint paint);
	void drawPath(Path path, VGubyte red, VGubyte green, VGubyte blue, VGubyte alpha);
}
