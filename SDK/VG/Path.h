#pragma once

#include <VG/OpenVG.h>
#include ".\types.h"

//namespace VG
namespace vg
{
	struct PathObject;
	
	struct Path: public PtrToImpl<PathObject>
	{
		//void appendPath(Path srcPath);
		void appendData(int numSegments, const ubyte* pathSegments, const float* pathData);
		//void modifyCoords(int startIndex, int numSegments, const void* pathData);
		//void clear();
		//void bounds(float* minX,  float* minY, float* width, float* height);

		//void vgPointAlongPath(VGint startSegment, VGint numSegments, VGfloat distance,
		//					  VGfloat * x, VGfloat * y, VGfloat * tangentX, VGfloat * tangentY);
		//VGfloat vgPathLength(VGint startSegment, VGint numSegments);
	};

	//Path createPath(float scale, float bias);
	////void transformPath(Path dstPath, Path srcPath);
	////bool interpolatePath(Path dstPath, Path startPath, Path endPath, float amount);
	//void destroyPath(Path path);
}