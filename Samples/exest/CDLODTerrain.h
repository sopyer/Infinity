#ifndef __CDLODTERRAIN_H_INCLUDED__
#	define __CDLODTERRAIN_H_INCLUDED__

#include <framework.h>
#include <vector>
#include <glm/glm.h>

class CDLODTerrain
{
	public:
	//private:
		size_t gridDimX;
		size_t gridDimY;
		float size;
		float startX;
		float startY;
		float endX;
		float endY;
		float visibilityDistance;
		GLint LODCount;
		float detailBalance;
		float morphZoneRatio;
		size_t minPatchDimX, minPatchDimY;
		GLuint	terrainProgram;
		GLint	uniOffset, uniScale, uniViewPos, uniMorphParams;
		
		glm::vec3 viewPoint;
		glm::mat4 VP;

		void setupTerrainParams();
	
		struct LODDesc
		{
			float scaleX, scaleY;
			float rangeStart;
			float rangeEnd;
			float minRange;
			size_t patchDimX, patchDimY;
			float morphStart;
			float morphInvRange, morphStartRatio;
		};

		LODDesc	LODs[16];

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