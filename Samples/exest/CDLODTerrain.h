#ifndef __CDLODTERRAIN_H_INCLUDED__
#	define __CDLODTERRAIN_H_INCLUDED__

#include <gfx/gfx.h>

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

    cpu_timer_t      cpuTimer;
    cpu_timer_t      cpuSelectTimer;
    cpu_timer_t      cpuRenderTimer;
    gfx::gpu_timer_t gpuTimer;
};

#endif