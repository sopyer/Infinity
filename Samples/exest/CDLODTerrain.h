#ifndef __CDLODTERRAIN_H_INCLUDED__
#	define __CDLODTERRAIN_H_INCLUDED__

#include <framework.h>
#include <vector>
#include <glm/glm.h>
#include <vi.h>
#include "math.h"

class CDLODTerrain
{
	public:
		static const size_t MAX_LOD_COUNT = 8;

		size_t gridDimX, gridDimY;

		float size;//cellSizeX, cellSizeY

		float startX, startY; //offset

		float visibilityDistance;
		
		size_t LODCount;
		float detailBalance;
		float morphZoneRatio;
		size_t minPatchDimX, minPatchDimY; //patchDimX, patchDimY
		float heightScale;
		
		GLuint	terrainProgram;
		GLint	uniOffset, uniScale, uniViewPos, uniMorphParams,
				uniPatchBase, uniHMDim, uniLevel;

		glm::vec3 viewPoint;
		ml::mat4x4 sseVP;

		void generateGeometry();
		void generateBBoxData(uint8_t* data);

		float*	minmaxData;
		size_t	minmaxDataSize;

		GLuint	mHeightmapTex;

		struct LODDesc
		{
			float	rangeStart;
			size_t	width, height;
			size_t	minmaxOffset, minmaxPitch;	
		};

		float		morphParams[MAX_LOD_COUNT*2];
		LODDesc		LODs[MAX_LOD_COUNT];

		void setHeightmap(uint8_t* data, size_t width, size_t height);
		void setViewProj(glm::mat4& mat);
		void calculateLODParams();
		void calculateLODRanges(float* ranges);

		void addPatchToQueue(size_t level, size_t i, size_t j);

		int intersectViewFrustum(size_t level, size_t i, size_t j);
		bool intersectSphere(size_t level, size_t i, size_t j);
		void selectQuadsForDrawing(size_t level, size_t i, size_t j, bool skipFrustumTest=false);

		void getMinMaxZ(size_t level, size_t i, size_t j, float* minZ, float* maxZ);
		void getAABB(size_t level, size_t i, size_t j, ml::aabb* patchAABB);

		void initialize();
		void cleanup();

		struct PatchData
		{
			float baseX, baseY;
			GLint level;
		};

		std::vector<PatchData>	patchList;

		void drawTerrain();

		float getCPUTime() {return (float)cpuTime;}
		float getGPUTime() {return (float)gpuTime;}

		CPUTimer	cpuTimer;
		GPUTimer	gpuTimer;

		double cpuTime;
		double gpuTime;

		GLuint vbo, ibo;
};

#endif