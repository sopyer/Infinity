#include <vector>
#include <glm/glm.h>
#include <Framework.h>

#include "Path.h"
#include "PathImpl.h"

namespace vg
{
	Path createPath(float scale, float bias)
	{
		Path	path;

		path.mImpl = new PathImpl(scale, bias);

		return path;
	}

	void Path::appendData(int numSegments, const ubyte* pathSegments, const float* pathData)
	{
		size_t	numData = 0;
		size_t	segOffset = mImpl->segs.size();
		size_t	dataOffset = mImpl->data.size();
		//VG_RETURN_ERR_IF(!segs || !data || newSegCount <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

		/* Count number of coordinatess in appended data */
		numData = coordCountForData(numSegments, pathSegments);
		
		mImpl->segs.insert(mImpl->segs.end(), pathSegments, pathSegments+numSegments);
		mImpl->data.insert(mImpl->data.end(), pathData, pathData+numData);
		
		mImpl->preprocess(segOffset, dataOffset);
		//VG_RETURN_ERR_IF(newDataCount == 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

		//VG_RETURN(VG_NO_RETVAL);
	}

	void Path::draw(ProgramRef fill)
	{
		mImpl->maskPathRegion();

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glUseProgram(*fill);

		glEnable(GL_STENCIL_TEST);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 1, 1);

		//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glBegin(GL_QUADS);
		glVertex2f(mImpl->min.x, mImpl->min.y);
		glVertex2f(mImpl->max.x, mImpl->min.y);
		glVertex2f(mImpl->max.x, mImpl->max.y);
		glVertex2f(mImpl->min.x, mImpl->max.y);
		glEnd();

		glPopAttrib();
	}


	void destroyPath(Path path)
	{
		delete path.mImpl;
		path.mImpl = 0;
	}
}