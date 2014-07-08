#ifndef __CDLODTERRAIN_H_INCLUDED__
#	define __CDLODTERRAIN_H_INCLUDED__

#include <framework.h>
#include <ml.h>
#include <graphics.h>

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

    ml::mat4x4 selectionMVP; //matrix has changed column order (x, z, y, w) in order to simplify simd calculations
    ml::vec3   viewPoint;
    float      vertDistToTerrain;
    size_t     maxLevel;

    PatchData* instData;
    size_t     patchCount;
    size_t     maxPatchCount;
    GLsizei    idxCount;

    GLuint prgTerrain;
    gfx::ubo_desc_t ubufView;

    GLuint  ibo, ubo;
    GLuint  mHeightmapTex;
    GLuint  mipTexture;
    GLuint  vao;

    void generateGeometry(size_t vertexCount);
    void setHeightmap(uint16_t* data, size_t width, size_t height);

    void selectQuadsForDrawing(size_t level, float bx, float bz, float patchSize, bool skipFrustumTest=false);

    void initialize();
    void cleanup();
    void reset();

    void setSelectMatrix(v128 m[4]);
    void drawTerrain();

    CPUTimer cpuTimer;
    CPUTimer cpuSelectTimer;
    CPUTimer cpuDrawTimer;
    GPUTimer gpuTimer;

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