#ifndef __CDLODTERRAIN_H_INCLUDED__
#	define __CDLODTERRAIN_H_INCLUDED__

#include <framework.h>
#include <vector>
#include <glm/glm.h>
#include <vi.h>
#include "math.h"

struct PatchData;

class CDLODTerrain
{
	public:
		static const size_t MAX_LOD_COUNT = 8;
		static const size_t MAX_PATCH_COUNT = 2048;

		size_t gridDimX;
		size_t gridDimY;
		size_t LODCount;
		size_t patchDim;
		float  cellSize;

		float heightScale;
		float startX;
		float startY;
		float visibilityDistance;
		float detailBalance;
		float morphZoneRatio;
		
		bool		useInstancing;

		GLuint		prgInstancedTerrain, prgTerrain;
		GLuint		mHeightmapTex;
		GLuint		mColorRampTex;
		GLuint		vaoInst, vao;

		GLsizei		uniTerrainOffset, uniViewOffset, uniGradientOffset, uniPatchOffset;

		struct TerrainData
		{
			vec4		uHMDim;
			vec4		uOffset;
			vec4		uScale;
			vec4		uMorphParams[MAX_LOD_COUNT];
			vec4		uColors[MAX_LOD_COUNT];

		} terrainData;

		struct ViewData
		{
			ml::mat4x4	uMVP;
			vec4		uViewPoint;
		} viewData;

		struct GradientData
		{
			vec4		uStops[8];
			vec4		uScales[8];
			float		uInvStopCount;
		} gradientData;

		GLuint		geomVBO, instVBO, ibo, ubo;

		PatchData*	instData;
		PatchData*	patchDataMem;
		size_t		patchCount;
		size_t		maxPatchCount;
		GLsizei		idxCount;

		glm::vec4	viewDir;

		ml::mat4x4	sseVP;

		float*	minmaxData;
		size_t	minmaxDataSize;

		struct LODDesc
		{
			float	rangeStart;
			size_t	patchDim;
			size_t	minmaxOffset;
			size_t	minmaxPitch;	
		};

		LODDesc		LODs[MAX_LOD_COUNT];

		void generateGeometry();
		void generateBBoxData(uint16_t* data);

		void setHeightmap(uint16_t* data, size_t width, size_t height);
		void setSelectMatrix(glm::mat4& mat);
		void setMVPMatrix(glm::mat4& mat);

		void calculateLODParams();
		void calculateLODRanges(float* ranges);

		void addPatchToQueue(size_t level, size_t i, size_t j);

		void selectQuadsForDrawing(size_t level, size_t i, size_t j, bool skipFrustumTest=false);

		void getAABB(size_t level, size_t i, size_t j, ml::aabb* patchAABB);

		void initialize();
		void cleanup();

		void drawTerrain();

		void reset();

		bool drawWireframe;

		CPUTimer	cpuTimer;
		CPUTimer	cpuSelectTimer;
		CPUTimer	cpuDrawTimer;
		GPUTimer	gpuTimer;

		double cpuTime;
		double cpuSelectTime;
		double cpuDrawTime;
		double gpuTime;

		float getCPUTime() {return (float)cpuTime;}
		float getCPUSelectTime() {return (float)cpuSelectTime;}
		float getCPUDrawTime() {return (float)cpuDrawTime;}
		float getGPUTime() {return (float)gpuTime;}
};

#endif