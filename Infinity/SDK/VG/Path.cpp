#include <vector>
#include <glm/glm.h>
#include <Framework.h>

#include "Path.h"
#include "PathObject.h"

namespace vg
{
	Path createPath(float scale, float bias)
	{
		Path	path;

		path.mObject = new PathObject(scale, bias);

		return path;
	}

	void Path::appendData(int numSegments, const ubyte* pathSegments, const float* pathData)
	{
		size_t	numData = 0;
		size_t	segOffset = mObject->segs.size();
		size_t	dataOffset = mObject->data.size();
		//VG_RETURN_ERR_IF(!segs || !data || newSegCount <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

		/* Count number of coordinatess in appended data */
		numData = coordCountForData(numSegments, pathSegments);
		
		mObject->segs.insert(mObject->segs.end(), pathSegments, pathSegments+numSegments);
		mObject->data.insert(mObject->data.end(), pathData, pathData+numData);
		
		mObject->preprocess(segOffset, dataOffset);
		//VG_RETURN_ERR_IF(newDataCount == 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

		//VG_RETURN(VG_NO_RETVAL);
	}

	void Path::draw(ProgramRef fill)
	{
		mObject->maskPathRegion();

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glUseProgram(*fill);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 1, 1);

		glBegin(GL_QUADS);
		glVertex2f(mObject->min.x, mObject->min.y);
		glVertex2f(mObject->max.x, mObject->min.y);
		glVertex2f(mObject->max.x, mObject->max.y);
		glVertex2f(mObject->min.x, mObject->max.y);
		glEnd();

		glPopAttrib();
	}


	void destroyPath(Path path)
	{
		delete path.mObject;
		path.mObject = 0;
	}
}