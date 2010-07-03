#include "Path.h"

#include <climits>
#include <cassert>
#include <glm/glm.h>
#include <vector>

namespace impl
{
	size_t pathDataSize(const VGint segCount, const VGubyte *segs);

	////////////////////////////////////////////////////
	//			Public methods
	////////////////////////////////////////////////////
	Path::Path():mPrims(15)
	{
	}


	void Path::appendData(const VGint    numSegments,
						  const VGubyte* pathSegments,
						  const VGfloat* pathData)
	{
		mSegments.insert(mSegments.std::vector<VGubyte>::end(), pathSegments, pathSegments+numSegments);
		mData.insert(mData.std::vector<float>::end(), pathData, pathData+pathDataSize(numSegments, pathSegments));

		mRebuildGeometry = true;
		prepareGeom();
	}

	void Path::bounds(VGfloat* minX,
					  VGfloat* minY,
					  VGfloat* maxX,
					  VGfloat* maxY)
	{
		*minX = mFillGeom.mMin.x;
		*minY = mFillGeom.mMin.y;
		*maxX = mFillGeom.mMax.x;
		*maxY = mFillGeom.mMax.y;
	}

	void Path::rasterizeFill()
	{
		prepareGeom();
		RasterizeEvenOdd(mFillGeom, mPrims);
	}

	void Path::prepareGeom()
	{	
		if (mRebuildGeometry)
		{
			assert(mSegments.size()<VG_MAXINT);
			impl::buildFillGeometry(mFillGeom, (VGint)mSegments.size(), mSegments.begin(), mData.begin());
		}

		mRebuildGeometry = false;
	}

	////////////////////////////////////////////////////
	//			Helper functions
	////////////////////////////////////////////////////
	bool isValidCommand(int c)
	{
		return c>=(VG_CLOSE_PATH >> 1) && c<=(VG_LCWARC_TO >> 1);
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

	size_t pathDataSize(const VGint segCount, const VGubyte *segs)
	{
		assert(segCount>=0);
		size_t	count = 0;

		for (size_t s=0; s<(size_t)segCount; ++s)
		{
			VGubyte command = ((segs[s] & 0x1E) >> 1);
			if (!isValidCommand(command))
				return 0;
			count += coordsPerCommand[command];
		}

		return count;
	}
}

