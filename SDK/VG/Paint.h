#ifndef __PAINT_H_INCLUDED__
#	define __PAINT_H_INCLUDED__

#include <VG/OpenVG.h>
#include <VG/DataTypes.h>

#include <VG/VGCommon.h>

namespace vg
{
	typedef GLuint Paint;

	Paint	createSolidPaint(float* color4f);
	Paint	createSolidPaint(unsigned int color);
	void	destroyPaint(Paint paint);

	void	applyPaintAsGLProgram(Paint paint);
}

#endif