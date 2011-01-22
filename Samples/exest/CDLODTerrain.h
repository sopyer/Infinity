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
	//private:
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
				uniPatchBase, uniPatchDim, uniHMDim;
		std::vector<glm::vec2>	mTerrainVtx;
		std::vector<uint16_t>	mTerrainIdx;

		glm::vec3 viewPoint;
		ml::mat4x4 sseVP;

		void generateGeometry();
		void generateBBoxData(uint8_t* data);

		float*	bboxZData;
		size_t	bboxZDataSize;

		GLuint	mHeightmapTex;

		struct LODDesc
		{
			float	rangeStart;
			size_t	width, height;
			float	morphInvRange, morphStartRatio;
			size_t	bboxZDataOffset, pitch;	
		};

		LODDesc	LODs[16];

		void setHeightmap(uint8_t* data, size_t width, size_t height);
		void setViewProj(glm::mat4& mat);
		void setupLODParams();

		void addPatchToQueue(size_t level, size_t i, size_t j);
		void drawPatch(float baseX, float baseY, int level);

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

		CPUTimer	cpuTimer;
		GPUTimer	gpuTimer;

		double cpuTime;
		double gpuTime;

		GLuint vbo, ibo;
};

#endif