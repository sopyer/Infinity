#ifndef __PATH_H_INCLUDED__
#	define __PATH_H_INCLUDED__

#include <VG/OpenVG.h>

namespace impl
{
	struct Geometry;
}

namespace vg
{
	typedef impl::Geometry* Path;

	Path createPath(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData);
	Path createUnitQuad();
	void destroyPath(Path path);

	void getPathBounds(Path path, float& x1, float& y1, float& x2, float& y2);
}

#endif
