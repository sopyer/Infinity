#include <vector>
#include <glm/glm.h>
#include <utils.h>

#include "Path.h"
#include "impl/Path.h"

namespace vg
{
	void Path::appendData(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData)
	{
		mHandle->appendData(numSegments, pathSegments, pathData);
		//VG_RETURN_ERR_IF(newDataCount == 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

		//VG_RETURN(VG_NO_RETVAL);
	}

	void Path::getBounds(float& x1, float& y1, float& x2, float& y2)
	{
		mHandle->bounds(&x1, &y1, &x2, &y2);
	}

	void Path::setRastPrims(VGuint prims)
	{
		mHandle->mPrims = prims;
	}

	Path Path::create(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData)
	{
		Path	newPath;
		
		newPath.mHandle = new impl::Path();
		newPath.appendData(numSegments, pathSegments, pathData);
		return newPath;
	}

	Path Path::createUnitQuad()
	{
		const VGubyte quadSegs[] = {VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL};
		const VGfloat quadData[] = {0.5f, 0.5f, -1, 0, 0, -1, 1, 0, 0, 1};
		return create(ARRAY_SIZE(quadSegs), quadSegs, quadData);
	}

	void Path::destroy(Path path)
	{
		delete path.mHandle;
		path.mHandle = 0;
	}
}