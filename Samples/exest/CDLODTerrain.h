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
		
		GLint LODCount;
		float detailBalance;
		float morphZoneRatio;
		size_t minPatchDimX, minPatchDimY; //patchDimX, patchDimY
		
		GLuint	terrainProgram;
		GLint	uniOffset, uniScale, uniViewPos, uniMorphParams, uniPatchBase, uniPatchDim;
		std::vector<glm::vec2>	mTerrainVtx;
		std::vector<uint16_t>	mTerrainIdx;

		glm::vec3 viewPoint;
		ml::mat4x4 sseVP;

		void generateGeometry();

		void setupTerrainParams();
		
		GLuint	mHeightmapTex;

		struct LODDesc
		{
			float scaleX, scaleY;
			float rangeStart, rangeEnd;
			float minRange;
			size_t patchDimX, patchDimY;
			float morphStart;
			float morphInvRange, morphStartRatio;
		};

		LODDesc	LODs[16];

		void setHeightmap(GLuint tex) {mHeightmapTex = tex;}
		void setViewProj(glm::mat4& mat);
		void setupLODParams();
		void addPatchToQueue(size_t level, size_t i, size_t j);
		bool intersectViewFrustum(size_t level, size_t i, size_t j);
		bool intersectSphere(size_t level, size_t i, size_t j);
		void selectQuadsForDrawing(size_t level, size_t i, size_t j);
		void drawPatch(float baseX, float baseY, float scaleX, float scaleY, int level);

		struct PatchData
		{
			float baseX, baseY;
			float scaleX, scaleY; //TODO: Can be accessed from level???
			GLint level;
		};

		std::vector<PatchData>	patchList;

		void drawTerrain();
};

#endif