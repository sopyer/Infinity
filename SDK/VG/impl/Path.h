#ifndef __IMPL_PATH_H_INCLUDED__
#	define	__IMPL_PATH_H_INCLUDED__

#include <vg/openvg.h>
#include <DataTypes.h>
#include "Rasterizer.h"

namespace impl
{
	struct Path
	{
		void appendData(const VGint    numSegments,
						const VGubyte* pathSegments,
						const VGfloat* pathData);

		void bounds(VGfloat* minX,
					VGfloat* minY,
					VGfloat* maxX,
					VGfloat* maxY);
		
		void prepareGeom();

		//User path data
		Array<VGubyte>	mSegments;
		Array<float>	mData;
			
		//Private path data
		FillGeometry	mFillGeom;
		StrokeGeometry	mStrokeGeom;
			
		bool	mRebuildGeometry;
	};
}

#endif
