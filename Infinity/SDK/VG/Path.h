#pragma once

#include <VG/OpenVG.h>
#include ".\types.h"
//namespace VG
namespace vg
{
	template<typename T>
	struct PtrToImpl
	{
		public:
			T*	mObject;
	};

	struct PathObject;
	
	void init(ProgramRef maskQuad, ProgramRef maskCubic, ProgramRef maskArc);
	void deinit();

	struct Path: public PtrToImpl<PathObject>
	{
		void appendPath(Path srcPath);
		void appendData(int numSegments, const ubyte* pathSegments, const float* pathData);
		void modifyCoords(int startIndex, int numSegments, const void* pathData);
		void clear();
		void bounds(float* minX,  float* minY, float* width, float* height);
		//void vgPointAlongPath(VGint startSegment, VGint numSegments, VGfloat distance,
		//					  VGfloat * x, VGfloat * y, VGfloat * tangentX, VGfloat * tangentY);
		//VGfloat vgPathLength(VGint startSegment, VGint numSegments);

		//!!!Temporary!!!
		void draw(ProgramRef fill);
	};

	Path createPath(float scale, float bias);
	void transformPath(Path dstPath, Path srcPath);
	bool interpolatePath(Path dstPath, Path startPath, Path endPath, float amount);
	void destroyPath(Path path);
}