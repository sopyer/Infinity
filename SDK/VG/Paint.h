#ifndef __PAINT_H_INCLUDED__
#	define __PAINT_H_INCLUDED__

#include <VG/OpenVG.h>
#include <DataTypes.h>

#include <VGCommon.h>

namespace vg
{
	struct Paint: public Handle<GLuint>
	{
		static Paint createSolid(float* color4f);
		static Paint createPattern();
		static void  destroy(Paint paint);
	};
}

#endif