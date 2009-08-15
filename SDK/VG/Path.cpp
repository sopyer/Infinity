#include <vector>
#include <glm/glm.h>
#include <Framework.h>

#include "Path.h"
#include "PathObject.h"

namespace vg
{
	size_t coordCountForData(size_t segCount, const ubyte *segs);

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

	const int coordsPerCommand[] =
	{
	  0, /* VG_CLOSE_PATH */
	  2, /* VG_MOTE_TO */
	  2, /* VG_LINE_TO */
	  1, /* VG_HLINE_TO */
	  1, /* VG_VLINE_TO */
	  4, /* VG_QUAD_TO */
	  6, /* VG_CUBIC_TO */
	  2, /* VG_SQUAD_TO */
	  4, /* VG_SCUBIC_TO */
	  5, /* VG_SCCWARC_TO */
	  5, /* VG_SCWARC_TO */
	  5, /* VG_LCCWARC_TO */
	  5  /* VG_LCWARC_TO */
	};

	size_t coordCountForData(size_t segCount, const ubyte *segs)
	{
		size_t	count = 0;

		for (size_t s=0; s<segCount; ++s)
		{
			ubyte command = ((segs[s] & 0x1E) >> 1);
			if (!isValidCommand(command))
				return 0;
			count += coordsPerCommand[command];
		}

		return count;
	}
}