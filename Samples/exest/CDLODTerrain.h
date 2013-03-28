#ifndef __CDLODTERRAIN_H_INCLUDED__
#	define __CDLODTERRAIN_H_INCLUDED__

#include <framework.h>
#include <vector>
#include <glm/glm.h>
#include <vi.h>
#include <ml.h>

struct PatchData;

class CDLODTerrain
{
public:
    static const size_t MAX_LOD_COUNT = 8;
    static const size_t MAX_PATCH_COUNT = 4096;

    size_t patchDim;
    size_t LODCount;
    float  chunkSize;
    float  minX, maxX;
    float  minY, maxY;
    float  minZ, maxZ;

    float LODRange  [MAX_LOD_COUNT];

    struct ViewData
    {
        ml::mat4x4	uMVP;
        v128		uLODViewK;
    } viewData;

    glm::vec4	viewDir;
    glm::vec3   viewPoint;
    ml::mat4x4	sseVP; //matrix has changed column order (x, z, y, w) in order to simplify simd calculations
    float       vertDistToTerrain;
    size_t      maxLevel;

    PatchData*	instData;
    PatchData*	patchDataMem;
    size_t		patchCount;
    size_t		maxPatchCount;
    GLsizei		idxCount;

    bool useInstancing;
    bool drawWireframe;

    GLuint		prgInstancedTerrain, prgTerrain;
    GLuint		geomVBO, instVBO, ibo, ubo;
    GLuint		mHeightmapTex;
    GLuint      mipTexture;
    GLuint      compositeFBO;
    GLuint		vaoInst, vao;
    GLsizei		uniTerrainOffset, uniViewOffset, uniPatchOffset;

    void generateGeometry(size_t vertexCount);
    void setHeightmap(uint16_t* data, size_t width, size_t height);

    void addPatchToQueue(size_t level, float bx, float bz);
    void selectQuadsForDrawing(size_t level, float bx, float bz, float patchSize, bool skipFrustumTest=false);

    void initialize();
    void cleanup();
    void reset();

    void setSelectMatrix(glm::mat4& mat);
    void setMVPMatrix(glm::mat4& mat);
    void drawTerrain();

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