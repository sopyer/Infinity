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

		size_t gridDimX, gridDimY;
		size_t LODCount;
		size_t minPatchDimX, minPatchDimY; //patchDimX, patchDimY
		float size;//cellSizeX, cellSizeY
		float heightScale;
		float startX, startY; //offset
		float visibilityDistance;
		float detailBalance;
		float morphZoneRatio;
		
		bool		useInstancing;

		GLuint		terrainProgram;
		GLuint		mHeightmapTex;
		GLuint		mColorRampTex;
		GLuint		vaoInst, vao;

		GLsizei		uniTerrainOffset, uniViewOffset, uniGradientOffset;

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

		PatchData*	instData;
		size_t		instCount;
		GLuint		vbo, instVBO, ibo, ubo;

		ml::mat4x4	sseVP;

		float*	minmaxData;
		size_t	minmaxDataSize;

		struct LODDesc
		{
			float	rangeStart;
			size_t	width, height;
			size_t	minmaxOffset, minmaxPitch;	
		};

		LODDesc		LODs[MAX_LOD_COUNT];

		void generateGeometry();
		void generateBBoxData(uint8_t* data);

		void setHeightmap(uint8_t* data, size_t width, size_t height);
		void setSelectMatrix(glm::mat4& mat);
		void setMVPMatrix(glm::mat4& mat);

		void calculateLODParams();
		void calculateLODRanges(float* ranges);

		void addPatchToQueue(size_t level, size_t i, size_t j);

		int  intersectViewFrustum(size_t level, size_t i, size_t j);
		bool intersectSphere(size_t level, size_t i, size_t j);
		void selectQuadsForDrawing(size_t level, size_t i, size_t j, bool skipFrustumTest=false);

		void getMinMaxZ(size_t level, size_t i, size_t j, float* minZ, float* maxZ);
		void getAABB(size_t level, size_t i, size_t j, ml::aabb* patchAABB);

		void initialize();
		void cleanup();

		void drawTerrain();

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