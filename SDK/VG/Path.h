#ifndef __PATH_H_INCLUDED__
#	define __PATH_H_INCLUDED__

#include <VG/OpenVG.h>

#include <DataTypes.h>
#include <VGCommon.h>

namespace vg
{
	enum
	{
		PRIM_TYPE_TRI = 1,
		PRIM_TYPE_ARC = 2,
		PRIM_TYPE_QUAD = 4,
		PRIM_TYPE_CUBIC = 8,
		PRIM_TYPE_ALL = 15,
	};

	struct Path: public Handle<impl::Path*>
	{
		void appendData(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData);
		void setRastPrims(VGuint prims);

		static Path create(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData);
		static Path createUnitQuad();
		static void destroy(Path path);

		//void appendPath(Path srcPath);
		//void modifyCoords(int startIndex, int numSegments, const void* pathData);
		//void clear();
		//void bounds(float* minX,  float* minY, float* width, float* height);
		//void vgPointAlongPath(VGint startSegment, VGint numSegments, VGfloat distance,
		//					  VGfloat * x, VGfloat * y, VGfloat * tangentX, VGfloat * tangentY);
		//VGfloat vgPathLength(VGint startSegment, VGint numSegments);
	};

	//Path createPath(float scale, float bias);
	//void destroyPath(Path path);

	////void transformPath(Path dstPath, Path srcPath);
	////bool interpolatePath(Path dstPath, Path startPath, Path endPath, float amount);
}

#endif
