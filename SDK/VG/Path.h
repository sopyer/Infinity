#ifndef __PATH_H_INCLUDED__
#	define __PATH_H_INCLUDED__

#include <VG/OpenVG.h>

#include <VG/DataTypes.h>
#include <VG/VGCommon.h>

namespace vg
{
	typedef impl::Path* Path;

	Path createPath(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData);
	Path createUnitQuad();
	void destroyPath(Path path);

	void getPathBounds(Path path, float& x1, float& y1, float& x2, float& y2);
}

#endif
