#include "CDLODTerrain.h"

struct PatchData
{
    float baseX, baseZ;
    int   level;
    float padding;
};

struct TerrainData
{
    v128        uAABB;
    v128		uUVXform;
    v128		uHeightXform;
    v128		uPatchScales[CDLODTerrain::MAX_LOD_COUNT];
    v128		uMorphParams[CDLODTerrain::MAX_LOD_COUNT];
    v128		uColors[CDLODTerrain::MAX_LOD_COUNT];
};


#define ATTR_POSITION	0
#define ATTR_PATCH_BASE	1
#define ATTR_LEVEL		2

#define UNI_TERRAIN_BINDING  0
#define UNI_VIEW_BINDING     1
#define UNI_PATCH_BINDING    2

void CDLODTerrain::initialize()
{
    useInstancing = true;
    drawWireframe = false;

    char* define;

    // Create non-instanced terrain program
    define = "#version 330\n";
    prgTerrain  = resources::createProgramFromFiles("Terrain.CDLOD.Instanced.vert", "Terrain.SHLighting.frag", 1, (const char**)&define);

#ifdef _DEBUG
    {
        GLint structSize;
        GLint uniTerrain, uniPatch, uniView;

        uniTerrain  = glGetUniformBlockIndex(prgTerrain, "uniTerrain");
        uniView     = glGetUniformBlockIndex(prgTerrain, "uniView");
        uniPatch    = glGetUniformBlockIndex(prgTerrain, "uniPatch");

        glGetActiveUniformBlockiv(prgTerrain, uniTerrain, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(TerrainData));
        glGetActiveUniformBlockiv(prgTerrain, uniView, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(ViewData));
        glGetActiveUniformBlockiv(prgTerrain, uniPatch, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(PatchData));
    }
#endif

    // Create instanced terrain program
    define = "#version 330\n#define ENABLE_INSTANCING\n";
    prgInstancedTerrain  = resources::createProgramFromFiles("Terrain.CDLOD.Instanced.vert", "Terrain.SHLighting.frag", 1, (const char**)&define);

#ifdef _DEBUG
    {
        GLint structSize;
        GLint uniTerrain, uniView;

        uniTerrain  = glGetUniformBlockIndex(prgInstancedTerrain, "uniTerrain");
        uniView     = glGetUniformBlockIndex(prgInstancedTerrain, "uniView");

        glGetActiveUniformBlockiv(prgInstancedTerrain, uniTerrain, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(TerrainData));
        glGetActiveUniformBlockiv(prgInstancedTerrain, uniView, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(ViewData));
    }
#endif

    GLint totalSize=0;
    GLint align;

    glGenTextures(1, &mHeightmapTex);
    glBindTexture(GL_TEXTURE_2D, mHeightmapTex);

    glGenTextures(1, &mipTexture);
    glBindTexture(GL_TEXTURE_2D, mipTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexImage2D(GL_TEXTURE_2D, 2, GL_RGBA,  8,  8, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexImage2D(GL_TEXTURE_2D, 3, GL_RGBA,  4,  4, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexImage2D(GL_TEXTURE_2D, 4, GL_RGBA,  2,  2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexImage2D(GL_TEXTURE_2D, 5, GL_RGBA,  1,  1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glGenFramebuffers(1, &compositeFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, compositeFBO);
    float levelColors[6][4] = 
    {
        {0.0f, 0.0f, 1.0f, 0.8f},
        {0.0f, 0.5f, 1.0f, 0.4f},
        {1.0f, 1.0f, 1.0f, 0.0f},
        {1.0f, 0.7f, 0.0f, 0.2f},
        {1.0f, 0.3f, 0.0f, 0.6f},
        {1.0f, 0.0f, 0.0f, 0.8f}
    };
    for (GLint i=0; i<6; ++i)
    {
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mipTexture, i);
        glClearBufferfv(GL_COLOR, 0, levelColors[i]);
    }
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glGenBuffers(1, &geomVBO);
    glGenBuffers(1, &instVBO);
    glGenBuffers(1, &ibo);

    glGenVertexArrays(1, &vaoInst);
    glBindVertexArray(vaoInst);
    glBindBuffer(GL_ARRAY_BUFFER, geomVBO);
    glVertexAttribPointer(ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(ATTR_POSITION, 0);
    glBindBuffer(GL_ARRAY_BUFFER, instVBO);
    glVertexAttribIPointer(ATTR_PATCH_BASE, 2, GL_INT, sizeof(PatchData), (void*)0);
    glVertexAttribDivisor(ATTR_PATCH_BASE, 1);
    glVertexAttribPointer(ATTR_LEVEL, 2, GL_FLOAT, GL_FALSE, sizeof(PatchData), (void*)8);
    glVertexAttribDivisor(ATTR_LEVEL, 1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(ATTR_POSITION);
    glEnableVertexAttribArray(ATTR_LEVEL);
    glEnableVertexAttribArray(ATTR_PATCH_BASE);
    glBindVertexArray(0);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, geomVBO);
    glVertexAttribPointer(ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(ATTR_POSITION, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(ATTR_POSITION);
    glBindVertexArray(0);

    // Assumption - align is power of 2
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
    --align;

    uniTerrainOffset  = totalSize;	totalSize+=sizeof(TerrainData);		totalSize += align; totalSize &=~align;
    uniViewOffset     = totalSize;	totalSize+=sizeof(ViewData);		totalSize += align; totalSize &=~align;
    uniPatchOffset    = totalSize;	totalSize+=sizeof(PatchData);		totalSize += align; totalSize &=~align;

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, totalSize, 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    patchDataMem = (PatchData*)malloc(MAX_PATCH_COUNT*sizeof(PatchData));

    GLenum err = glGetError();
    assert(err==GL_NO_ERROR);
}

void CDLODTerrain::reset()
{
}


void CDLODTerrain::cleanup()
{
    free(patchDataMem);
    glDeleteBuffers(1, &ubo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &geomVBO);
    glDeleteBuffers(1, &instVBO);
    glDeleteBuffers(1, &ibo);
    glDeleteProgram(prgTerrain);
    glDeleteProgram(prgInstancedTerrain);
    glDeleteTextures(1, &mHeightmapTex);
    glDeleteTextures(1, &mipTexture);
    glDeleteFramebuffers(1, &compositeFBO);
}

void CDLODTerrain::setSelectMatrix(glm::mat4& mat)
{
    sseVP.r0 = vi_loadu(mat[0]);
    sseVP.r1 = vi_loadu(mat[1]);
    sseVP.r2 = vi_loadu(mat[2]);
    sseVP.r3 = vi_loadu(mat[3]);
}

void CDLODTerrain::setMVPMatrix(glm::mat4& mat)
{
    viewData.uMVP.r0 = vi_loadu(mat[0]);
    viewData.uMVP.r1 = vi_loadu(mat[1]);
    viewData.uMVP.r2 = vi_loadu(mat[2]);
    viewData.uMVP.r3 = vi_loadu(mat[3]);
}

void CDLODTerrain::addPatchToQueue(size_t level, float bx, float bz)
{
    if (patchCount<MAX_PATCH_COUNT)
    {
        instData->baseX = bx;
        instData->baseZ = bz;
        instData->level = level;

        ++instData;
        ++patchCount;
    }
}

void CDLODTerrain::selectQuadsForDrawing(size_t level, float bx, float bz, float patchSize, bool skipFrustumTest)
{
    if (bx>=maxX || bz>=maxZ) return;

    ml::aabb AABB;

    float sizeX = std::min(patchSize, maxX-bx);
    float sizeZ = std::min(patchSize, maxZ-bz);

    float minX = bx;
    float minZ = bz;
    float maxX = minX+sizeX;
    float maxZ = minZ+sizeZ;

    AABB.min = vi_set(minX, minY, minZ, 1.0f);
    AABB.max = vi_set(maxX, maxY, maxZ, 1.0f);

    if (!skipFrustumTest)
    {
        //Check whether AABB intersects frustum
        int result = ml::intersectionTest(&AABB, &sseVP);

        if (result==ml::IT_OUTSIDE) return;
        skipFrustumTest = result==ml::IT_INSIDE;
    }

    if (level==0 || vertDistToTerrain>LODRange[level])
    {
        addPatchToQueue(level, bx, bz);
        return;
    }

    //Check whether patch intersects it's LOD sphere and thus require subdivision
    glm::vec3 vp2d = viewPoint;
    vp2d.y = 0.0f;
    ml::aabb AABB2D = AABB;
    AABB2D.min.m128_f32[1] = 0.0f;
    AABB2D.max.m128_f32[1] = 0.0f;
    v128 vp = vi_set(vp2d.x, vp2d.y, vp2d.z, 0.0);

    v128 vdist;

    vdist = ml::distanceToAABB(&AABB, vi_set(viewPoint.x, viewPoint.y, viewPoint.z, 0.0));
    vdist = vi_shuffle<VI_SHUFFLE_MASK(VI_A_X, VI_B_X, VI_A_Z, VI_B_X)>(vdist, vi_load_zero());
    vdist = vi_dot3(vdist, vdist);

    float d2;
    _mm_store_ss(&d2, vdist);

    bool intersect = d2<=LODRange[level]*LODRange[level];

    assert(intersect == ml::sphereAABBTest(&AABB2D, vp, LODRange[level]));

    if (!intersect
        /*maxX-vp2d.x<-LODs[level].rangeStart ||
        minX-vp2d.x>LODs[level].rangeStart ||
        maxY-vp2d.z<-LODs[level].rangeStart ||
        minY-vp2d.z>LODs[level].rangeStart*/
        )
    {
        addPatchToQueue(level, bx, bz);
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

glm::vec4 colors[] = 
{
    glm::vec4(0.5f, 0.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
    glm::vec4(1.0f, 0.6f, 0.6f, 1.0f),
    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, 0.2f, 0.2f, 1.0f),
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
    glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
};

void CDLODTerrain::generateGeometry(size_t vertexCount)
{
    glBindBuffer(GL_ARRAY_BUFFER, geomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*vertexCount*vertexCount, 0, GL_STATIC_DRAW);
    float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (size_t y=0; y<vertexCount; ++y)
    {
        for (size_t x=0; x<vertexCount; ++x)
        {
            *ptr++ = (float)x;
            *ptr++ = (float)y;
        }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

#define INDEX_FOR_LOCATION(x, y) ((y)*vertexCount+(x))

    size_t quadsInRow = vertexCount-1;

    idxCount = quadsInRow*quadsInRow*6;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)*idxCount, 0, GL_STATIC_DRAW);
    uint16_t* ptr2 = (uint16_t*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (size_t y=0; y<quadsInRow; ++y)
    {
        for (size_t x=0; x<quadsInRow; ++x)
        {
            *ptr2++ = INDEX_FOR_LOCATION(x,   y);
            *ptr2++ = INDEX_FOR_LOCATION(x,   y+1);
            *ptr2++ = INDEX_FOR_LOCATION(x+1, y);
            *ptr2++ = INDEX_FOR_LOCATION(x+1, y);
            *ptr2++ = INDEX_FOR_LOCATION(x,   y+1);
            *ptr2++ = INDEX_FOR_LOCATION(x+1, y+1);
        }
    }

#undef INDEX_FOR_LOCATION

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CDLODTerrain::drawTerrain()
{
    PROFILER_CPU_BLOCK("CDLODTerrain");
    cpuTimer.start();

    if (useInstancing)
    {
        glBindBuffer(GL_ARRAY_BUFFER, instVBO);
        //Discard data from previous frame
        glBufferData(GL_ARRAY_BUFFER, MAX_PATCH_COUNT*sizeof(PatchData), 0, GL_STREAM_DRAW);
        instData = (PatchData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }
    else
    {
        instData = patchDataMem;
    }

    vertDistToTerrain = std::max(viewPoint.y-maxY, minY-viewPoint.y);
    vertDistToTerrain = std::max(vertDistToTerrain, 0.0f);

    {
        PROFILER_CPU_BLOCK("Select");
        cpuSelectTimer.start();
        patchCount = 0;
        size_t level = LODCount-1;
        for (float bz=minZ; bz<maxZ; bz+=chunkSize)
            for (float bx=minX; bx<maxX; bx+=chunkSize)
                selectQuadsForDrawing(level, bx, bz, chunkSize);
        cpuSelectTime = cpuSelectTimer.elapsed();
        cpuSelectTimer.stop();
    }

    if (useInstancing)
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    cpuDrawTimer.start();
    gpuTimer.start();

    patchCount = std::min(patchCount, maxPatchCount);
    if (patchCount)
    {
        PROFILER_CPU_BLOCK("Render");

        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glBindBufferRange(GL_UNIFORM_BUFFER, UNI_TERRAIN_BINDING,  ubo, uniTerrainOffset,  sizeof(TerrainData));
        glBindBufferRange(GL_UNIFORM_BUFFER, UNI_VIEW_BINDING,     ubo, uniViewOffset,     sizeof(ViewData));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mipTexture);

        //TODO: Enable triangle culling and fix mesh(Partially done)
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if (drawWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(useInstancing?prgInstancedTerrain:prgTerrain);

        viewData.uLODViewK = vi_set(-viewPoint.x, vertDistToTerrain, -viewPoint.z, 0.0f);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, uniViewOffset, sizeof(ViewData), &viewData);

        if (useInstancing)
        {
            glBindVertexArray(vaoInst);
            glDrawElementsInstanced(GL_TRIANGLES, idxCount, GL_UNSIGNED_SHORT, 0, patchCount);
        }
        else
        {
            glBindBufferRange(GL_UNIFORM_BUFFER, UNI_PATCH_BINDING, ubo, uniPatchOffset, sizeof(PatchData));
            glBindVertexArray(vao);
            for (size_t i=0; i<patchCount; ++i)
            {
                glBufferSubData(GL_UNIFORM_BUFFER, uniPatchOffset, sizeof(PatchData), patchDataMem+i);
                glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_SHORT, 0);
            }
        }

        glUseProgram(0);
        glBindVertexArray(0);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glPopAttrib();
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

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    uint8_t*      uniforms     = (uint8_t*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    TerrainData&  terrainData  = *(TerrainData*) (uniforms+uniTerrainOffset);

    assert(LODCount<=MAX_LOD_COUNT);

    float	size2=cellSize*cellSize*patchDim*patchDim,
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
        terrainData.uPatchScales[i] = vi_set_xxxx(cellSize);

        range    *= 1.5f;
        size2    *= 4.0f;
        cellSize *= 2.0f;
    }

    //Fix unnecessary morph in last LOD level
    terrainData.uMorphParams[LODCount-1] = vi_load_zero();

    terrainData.uAABB        = vi_set(minX, minZ, maxX, maxZ);
    terrainData.uUVXform     = vi_set(pixelsPerMeter*du, pixelsPerMeter*dv, (-minX*pixelsPerMeter+0.5f)*du, (-minZ*pixelsPerMeter+0.5f)*dv);
    terrainData.uHeightXform = vi_set(heightScale, minY, 0.0f, 0.0f);
    memcpy(terrainData.uColors, colors, sizeof(terrainData.uColors));

    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    GLenum err = glGetError();
    assert(err==GL_NO_ERROR);
}
