#include "CDLODTerrain.h"
#include <Framework/graphics.h>

struct PatchData
{
    float baseX, baseZ;
    int   level;
    float padding;
};

struct TerrainData
{
    v128 uAABB;
    v128 uUVXform;
    v128 uHeightXform;
    v128 uPatchScales[CDLODTerrain::MAX_LOD_COUNT];
    v128 uMorphParams[CDLODTerrain::MAX_LOD_COUNT];
    v128 uColors[CDLODTerrain::MAX_LOD_COUNT];
};

struct ViewData
{
    ml::mat4x4 uMVP;
    v128       uLODViewK;
};

#define ATTR_PATCH_BASE 0
#define ATTR_LEVEL      1

#define UNI_TERRAIN_BINDING 0
#define UNI_VIEW_BINDING    1
#define UNI_PATCH_BINDING   2

void CDLODTerrain::initialize()
{
    // Create instanced terrain program
    prgTerrain  = resources::createProgramFromFiles("Terrain.CDLOD.Instanced.vert", "Terrain.SHLighting.frag");

#ifdef _DEBUG
    {
        GLint structSize;
        GLint uniTerrain, uniView;

        uniTerrain  = glGetUniformBlockIndex(prgTerrain, "uniTerrain");
        uniView     = glGetUniformBlockIndex(prgTerrain, "uniView");

        glGetActiveUniformBlockiv(prgTerrain, uniTerrain, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(TerrainData));
        glGetActiveUniformBlockiv(prgTerrain, uniView, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(ViewData));
    }
#endif

    glGenTextures(1, &mHeightmapTex);

    glGenTextures(1, &mipTexture);
    glTextureParameteriEXT(mipTexture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteriEXT(mipTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteriEXT(mipTexture, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteriEXT(mipTexture, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2DEXT(mipTexture, GL_TEXTURE_2D, 6, GL_RGBA8, 32, 32);

    uint32_t levelColor[6] = 
    {
        0xCCFF0000,
        0x66FF8000,
        0x00FFFFFF,
        0x3300B3FF,
        0x66004DFF,
        0xCC0000FF
    };

    for (GLint i=0; i<6; ++i)
    {
        glClearTexImage(mipTexture, i, GL_RGBA, GL_UNSIGNED_BYTE, &levelColor[i]);
    }

    glGenBuffers(1, &ibo);

    graphics::vertex_element_t ve[2] = {
        {0, 0, ATTR_PATCH_BASE, GL_INT,   2, GL_TRUE,  GL_FALSE},
        {0, 8, ATTR_LEVEL,      GL_FLOAT, 1, GL_FALSE, GL_FALSE}
    };

    GLuint div = 1;

    vao = graphics::createVAO(2, ve, 1, &div);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindVertexArray(0);

    glGenBuffers(1, &ubo);
    glNamedBufferStorageEXT(ubo, sizeof(TerrainData), 0, GL_MAP_WRITE_BIT);

    ubufView = graphics::ubufCreateDesc(prgTerrain, "uniView");

    GL_CHECK_ERROR();
}

void CDLODTerrain::reset()
{
}

void CDLODTerrain::cleanup()
{
    glDeleteBuffers(1, &ubo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ibo);
    glDeleteProgram(prgTerrain);
    glDeleteTextures(1, &mHeightmapTex);
    glDeleteTextures(1, &mipTexture);

    graphics::ubufDestroyDesc(ubufView);
}

void CDLODTerrain::setSelectMatrix(v128 m[4])
{
    // Attention! Column order is changed!
    selectionMVP.r0 = m[0];
    selectionMVP.r1 = m[2];
    selectionMVP.r2 = m[1];
    selectionMVP.r3 = m[3];
}

inline bool patchIntersectsCircle(v128 vmin, v128 vmax, v128 vcenter, float radius)
{
    v128  vdist;
    float d2;

    vdist = ml::distanceToAABB(vmin, vmax, vcenter);
    vdist = vi_dot2(vdist, vdist);

    d2 = _mm_cvtss_f32(vdist);

    return d2<=radius*radius;
}

void CDLODTerrain::selectQuadsForDrawing(size_t level, float bx, float bz, float patchSize, bool skipFrustumTest)
{
    if (bx>=maxX || bz>=maxZ) return;

    if (patchCount>=MAX_PATCH_COUNT) return;

    ml::aabb AABB;

    v128 vp = vi_set(viewPoint.x, viewPoint.z, 0.0f, 0.0f);

    float sizeX = std::min(patchSize, maxX-bx);
    float sizeZ = std::min(patchSize, maxZ-bz);

    float minX = bx;
    float minZ = bz;
    float maxX = minX+sizeX;
    float maxZ = minZ+sizeZ;

    AABB.min = vi_set(minX, minZ, minY, 1.0f);
    AABB.max = vi_set(maxX, maxZ, maxY, 1.0f);

    if (!skipFrustumTest)
    {
        //Check whether AABB intersects frustum
        int result = ml::intersectionTest(&AABB, &selectionMVP);

        if (result==ml::IT_OUTSIDE) return;
        skipFrustumTest = result==ml::IT_INSIDE;
    }

    if (level==maxLevel || !patchIntersectsCircle(AABB.min, AABB.max, vp, LODRange[level]))
    {
        instData->baseX = bx;
        instData->baseZ = bz;
        instData->level = level;

        ++instData;
        ++patchCount;
    }
    else
    {
        //TODO: add front to back sorting to minimize overdraw(optimization)
        size_t levelNext = level-1;
        float  offset    = patchSize/2;
        selectQuadsForDrawing(levelNext, bx,        bz,        offset, skipFrustumTest);
        selectQuadsForDrawing(levelNext, bx+offset, bz,        offset, skipFrustumTest);
        selectQuadsForDrawing(levelNext, bx,        bz+offset, offset, skipFrustumTest);
        selectQuadsForDrawing(levelNext, bx+offset, bz+offset, offset, skipFrustumTest);
    }
}

ml::vec4 colors[] = 
{
    {0.5f, 0.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, 0.6f, 0.6f, 1.0f},
    {1.0f, 0.0f, 1.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
    {1.0f, 0.2f, 0.2f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f}
};

void CDLODTerrain::generateGeometry(size_t vertexCount)
{
#define INDEX_FOR_LOCATION(x, y) ((y)*vertexCount+(x))

    size_t quadsInRow = (vertexCount-1);

    idxCount = quadsInRow*quadsInRow*6;
    glNamedBufferDataEXT(ibo, sizeof(uint16_t)*idxCount, 0, GL_STATIC_DRAW);
    uint16_t* ptr2 = (uint16_t*)glMapNamedBufferRangeEXT(ibo, 0, sizeof(uint16_t)*idxCount, GL_MAP_WRITE_BIT);
    uint16_t vtx0 = 0;
    uint16_t vtx1 = 1;
    uint16_t vtx2 = vertexCount;
    uint16_t vtx3 = vertexCount + 1;
    for (size_t y=0; y<quadsInRow; ++y)
    {
        for (size_t x=0; x<quadsInRow; ++x)
        {
            if ((x + y) % 2 == 0)
            {
                *ptr2++ = vtx0; *ptr2++ = vtx2; *ptr2++ = vtx1;
                *ptr2++ = vtx1; *ptr2++ = vtx2; *ptr2++ = vtx3;
            }
            else
            {
                *ptr2++ = vtx2; *ptr2++ = vtx3; *ptr2++ = vtx0;
                *ptr2++ = vtx0; *ptr2++ = vtx3; *ptr2++ = vtx1;
            }
            ++vtx0;
            ++vtx1;
            ++vtx2;
            ++vtx3;
        }
        ++vtx0;
        ++vtx1;
        ++vtx2;
        ++vtx3;
    }

#undef INDEX_FOR_LOCATION

    glUnmapNamedBufferEXT(ibo);
}

void CDLODTerrain::drawTerrain()
{
    PROFILER_CPU_TIMESLICE("CDLODTerrain");
    cpuTimer.start();

    GLuint baseInstance;

    instData = (PatchData*)graphics::dynbufAllocVert(MAX_PATCH_COUNT*sizeof(PatchData), sizeof(PatchData), &baseInstance);

    vertDistToTerrain = std::max(viewPoint.y-maxY, minY-viewPoint.y);
    vertDistToTerrain = std::max(vertDistToTerrain, 0.0f);

    maxLevel = 1;
    while (maxLevel<LODCount && vertDistToTerrain>LODRange[maxLevel])
        maxLevel++;
    --maxLevel;

    {
        PROFILER_CPU_TIMESLICE("Select");
        cpuSelectTimer.start();
        patchCount = 0;
        size_t level = LODCount-1;
        for (float bz=minZ; bz<maxZ; bz+=chunkSize)
            for (float bx=minX; bx<maxX; bx+=chunkSize)
                selectQuadsForDrawing(level, bx, bz, chunkSize);
        cpuSelectTime = cpuSelectTimer.elapsed();
        cpuSelectTimer.stop();
    }

    cpuDrawTimer.start();
    gpuTimer.start();

    patchCount = std::min(patchCount, maxPatchCount);
    if (patchCount)
    {
        PROFILER_CPU_TIMESLICE("Render");

        {
            PROFILER_CPU_TIMESLICE("BindProgram");
            glUseProgram(prgTerrain);
        }

        {
            PROFILER_CPU_TIMESLICE("SetTextures");
            GLuint textureSet[2] = {mHeightmapTex, mipTexture};
            glBindTextures(0, 2, textureSet);
        }

        {
            PROFILER_CPU_TIMESLICE("Uniforms");
            GLuint    offset;
            ViewData* ptrViewData;
            
            ptrViewData = (ViewData*) graphics::dynbufAllocMem(sizeof(ViewData), graphics::caps.uboAlignment, &offset);
            //memcpy(&ptrViewData->uMVP, &viewMVP, sizeof(viewMVP));
            graphics::ubufUpdateData(ubufView, ptrViewData, sizeof(ViewData));
            ptrViewData->uLODViewK = vi_set(-viewPoint.x, vertDistToTerrain, -viewPoint.z, 0.0f);

            glBindBufferRange(GL_UNIFORM_BUFFER, UNI_TERRAIN_BINDING, ubo,                 0,      sizeof(TerrainData));
            glBindBufferRange(GL_UNIFORM_BUFFER, UNI_VIEW_BINDING,    graphics::dynBuffer, offset, sizeof(ViewData));
        }

        {
            PROFILER_CPU_TIMESLICE("BindGeom");
            glBindVertexArray(vao);
            glBindVertexBuffer(0, graphics::dynBuffer, 0, sizeof(PatchData));
        }

        {
            PROFILER_CPU_TIMESLICE("DIP");
            glDrawElementsInstancedBaseInstance(GL_TRIANGLES, idxCount, GL_UNSIGNED_SHORT, 0, patchCount, baseInstance);
        }

        {
            PROFILER_CPU_TIMESLICE("ClearBindings");
            glUseProgram(0);
            glBindVertexArray(0);
       }
    }

    cpuDrawTime = cpuDrawTimer.elapsed();
    cpuDrawTimer.stop();
    gpuTimer.stop();
    cpuTime = cpuTimer.elapsed();
    gpuTime = gpuTimer.getResult();
    cpuTimer.stop();
}

void CDLODTerrain::setHeightmap(uint16_t* data, size_t width, size_t height)
{
    //TODO: LODCount should clamped based on pixelsPerChunk
    LODCount  = 5;
    patchDim  = 8;
    chunkSize = 100;//patchSize[LODCount-1]*cellSize;

    float  pixelsPerChunk = 129;
    float  heightScale;
    float  cellSize = chunkSize/(pixelsPerChunk-1);
    //TODO: morphZoneRatio should should be less then 1-patchDiag/LODDistance
    float  morphZoneRatio = 0.30f;

    maxPatchCount = MAX_PATCH_COUNT;

    minX = -0.5f*cellSize*(width-1);
    minZ = -0.5f*cellSize*(height-1);
    maxX = minX+(width-1)*cellSize;
    maxZ = minZ+(height-1)*cellSize;

    heightScale = 65535.0f/732.0f;
    minY        = -39.938129f;//0.0f;
    maxY        =  133.064011f;//heightScale;
    heightScale = maxY-minY;

    float du = 1.0f/width;
    float dv = 1.0f/height;

    float pixelsPerMeter = (pixelsPerChunk-1)/chunkSize;

    generateGeometry(patchDim+1);

    TerrainData& terrainData = *(TerrainData*) glMapNamedBufferRangeEXT(ubo, 0, sizeof(TerrainData), GL_MAP_WRITE_BIT);

    assert(LODCount<=MAX_LOD_COUNT);

    float size2=cellSize*cellSize*patchDim*patchDim,
          range=100.0f, curRange=0.0f;

    for (size_t i=0; i<LODCount; ++i)
    {
        //To avoid cracks when diagonal is greater then minRange,
        float minRange = 2.0f*sqrt(size2+size2);
        range = std::max(range, minRange);

        LODRange[i]  = curRange;
        curRange    += range;

        float rangeEnd   = curRange;
        float morphRange = range*morphZoneRatio;
        float morphStart = rangeEnd-morphRange;

        terrainData.uMorphParams[i] = vi_set(1.0f/morphRange, -morphStart/morphRange, 0.0f, 0.0f);
        terrainData.uPatchScales[i] = vi_set_ffff(cellSize);

        range    *= 1.5f;
        size2    *= 4.0f;
        cellSize *= 2.0f;
    }

    //Fix unnecessary morph in last LOD level
    terrainData.uMorphParams[LODCount-1] = vi_set_0000();

    terrainData.uAABB        = vi_set(minX, minZ, maxX, maxZ);
    terrainData.uUVXform     = vi_set(pixelsPerMeter*du, pixelsPerMeter*dv, (-minX*pixelsPerMeter+0.5f)*du, (-minZ*pixelsPerMeter+0.5f)*dv);
    terrainData.uHeightXform = vi_set(heightScale, minY, 0.0f, 0.0f);
    memcpy(terrainData.uColors, colors, sizeof(terrainData.uColors));

    glUnmapNamedBufferEXT(ubo);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureImage2DEXT(mHeightmapTex, GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, data);
    glTextureParameteriEXT(mHeightmapTex, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteriEXT(mHeightmapTex, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteriEXT(mHeightmapTex, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteriEXT(mHeightmapTex, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteriEXT(mHeightmapTex, GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    GLenum err = glGetError();
    assert(err==GL_NO_ERROR);
}
