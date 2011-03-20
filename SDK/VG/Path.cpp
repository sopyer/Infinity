#include <vector>
#include <glm/glm.h>
#include <utils.h>

#include "Path.h"
#include "impl/Path.h"

namespace vg
{
	void getPathBounds(Path path, float& x1, float& y1, float& x2, float& y2)
	{
		path->bounds(&x1, &y1, &x2, &y2);
	}

	Path createPath(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData)
	{
		Path	newPath;
		
		newPath = new impl::Path();
		newPath->appendData(numSegments, pathSegments, pathData);
		return newPath;
	}

	Path createUnitQuad()
	{
		const VGubyte quadSegs[] = {VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL};
		const VGfloat quadData[] = {0.5f, 0.5f, -1, 0, 0, -1, 1, 0, 0, 1};
		return createPath(ARRAY_SIZE(quadSegs), quadSegs, quadData);
	}

	void destroyPath(Path path)
	{
		delete path;
	}
}