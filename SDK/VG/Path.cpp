#include <vector>
#include <glm/glm.h>
#include <Framework.h>

#include "Path.h"
#include "PathObject.h"

namespace vg
{
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
}