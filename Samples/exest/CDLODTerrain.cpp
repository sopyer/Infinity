#include "CDLODTerrain.h"

struct PatchData
{
    float baseX, baseY;
    float level;
    float padding;
};

struct TerrainData
{
    vec4		uHMDim;
    vec4		uOffset;
    vec4		uScale;
    vec4		uMorphParams[CDLODTerrain::MAX_LOD_COUNT];
    vec4		uColors[CDLODTerrain::MAX_LOD_COUNT];
};

struct GradientData
{
    vec4		uStops[8];
    vec4		uScales[8];
    float		uInvStopCount;
};

#define ATTR_POSITION	0
#define ATTR_PATCH_BASE	1
#define ATTR_LEVEL		2

#define UNI_TERRAIN_BINDING  0
#define UNI_VIEW_BINDING     1
#define UNI_GRADIENT_BINDING 2
#define UNI_PATCH_BINDING    3

//TODO: Make logic based on actual size instead of i,j patch indexing - should simlify shader code and general logic

//TODO: Make proper scale logic - to easilier accomodate to left/right -handed coordinate systems

void CDLODTerrain::initialize()
{
    useInstancing = true;
    drawWireframe = false;

    GLint uniTerrain, uniPatch, uniView, uniGradient;
    GLint uniHeightmap, uniColorRamp;

    char* define;

    // Create non-instanced terrain program
    define = "#version 330\n";
    prgTerrain  = resources::createProgramFromFiles("Terrain.CDLOD.Instanced.vert", "Terrain.HeightColored.frag", 1, (const char**)&define);

    uniTerrain  = glGetUniformBlockIndex(prgTerrain, "uniTerrain");
    uniView     = glGetUniformBlockIndex(prgTerrain, "uniView");
    uniGradient = glGetUniformBlockIndex(prgTerrain, "uniGradient");
    uniPatch    = glGetUniformBlockIndex(prgTerrain, "uniPatch");

    glUniformBlockBinding(prgTerrain, uniTerrain,  UNI_TERRAIN_BINDING);
    glUniformBlockBinding(prgTerrain, uniView,     UNI_VIEW_BINDING);
    glUniformBlockBinding(prgTerrain, uniGradient, UNI_GRADIENT_BINDING);
    glUniformBlockBinding(prgTerrain, uniPatch,    UNI_PATCH_BINDING);

#ifdef _DEBUG
    {
        GLint structSize;

        glGetActiveUniformBlockiv(prgTerrain, uniTerrain, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(TerrainData));
        glGetActiveUniformBlockiv(prgTerrain, uniView, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(ViewData));
        glGetActiveUniformBlockiv(prgTerrain, uniGradient, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(GradientData));
        glGetActiveUniformBlockiv(prgTerrain, uniPatch, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(PatchData));
    }
#endif

    uniHeightmap = glGetUniformLocation(prgTerrain, "uHeightmap");
    uniColorRamp = glGetUniformLocation(prgTerrain, "samColorRamp");

    glUseProgram(prgTerrain);
    glUniform1i(uniHeightmap, 0);
    glUniform1i(uniColorRamp, 1);
    glUseProgram(0);

    // Create instanced terrain program
    define = "#version 330\n#define ENABLE_INSTANCING\n";
    prgInstancedTerrain  = resources::createProgramFromFiles("Terrain.CDLOD.Instanced.vert", "Terrain.HeightColored.frag", 1, (const char**)&define);

    uniTerrain  = glGetUniformBlockIndex(prgInstancedTerrain, "uniTerrain");
    uniView     = glGetUniformBlockIndex(prgInstancedTerrain, "uniView");
    uniGradient = glGetUniformBlockIndex(prgInstancedTerrain, "uniGradient");

    glUniformBlockBinding(prgInstancedTerrain, uniTerrain,  UNI_TERRAIN_BINDING);
    glUniformBlockBinding(prgInstancedTerrain, uniView,     UNI_VIEW_BINDING);
    glUniformBlockBinding(prgInstancedTerrain, uniGradient, UNI_GRADIENT_BINDING);

#ifdef _DEBUG
    {
        GLint structSize;

        glGetActiveUniformBlockiv(prgInstancedTerrain, uniTerrain, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(TerrainData));
        glGetActiveUniformBlockiv(prgInstancedTerrain, uniView, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(ViewData));
        glGetActiveUniformBlockiv(prgInstancedTerrain, uniGradient, GL_UNIFORM_BLOCK_DATA_SIZE, &structSize);
        assert(structSize==sizeof(GradientData));
    }
#endif

    uniHeightmap = glGetUniformLocation(prgInstancedTerrain, "uHeightmap");
    uniColorRamp = glGetUniformLocation(prgInstancedTerrain, "samColorRamp");

    glUseProgram(prgInstancedTerrain);
    glUniform1i(uniHeightmap, 0);
    glUniform1i(uniColorRamp, 1);
    glUseProgram(0);

    GLint totalSize=0;
    GLint align;

    glGenTextures(1, &mHeightmapTex);
    glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glGenTextures(1, &mColorRampTex);
    glBindTexture(GL_TEXTURE_1D, mColorRampTex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenBuffers(1, &geomVBO);
    glGenBuffers(1, &instVBO);
    glGenBuffers(1, &ibo);

    glGenVertexArrays(1, &vaoInst);
    glBindVertexArray(vaoInst);
    glBindBuffer(GL_ARRAY_BUFFER, geomVBO);
    glVertexAttribPointer(ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(ATTR_POSITION, 0);
    glBindBuffer(GL_ARRAY_BUFFER, instVBO);
    glVertexAttribPointer(ATTR_PATCH_BASE, 2, GL_FLOAT, GL_FALSE, sizeof(PatchData), (void*)0);
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
    uniGradientOffset = totalSize;	totalSize+=sizeof(GradientData);	totalSize += align; totalSize &=~align;
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
    GLint uniTerrain, uniPatch, uniView, uniGradient;
    GLint uniHeightmap, uniColorRamp;

    uniTerrain  = glGetUniformBlockIndex(prgTerrain, "uniTerrain");
    uniView     = glGetUniformBlockIndex(prgTerrain, "uniView");
    uniGradient = glGetUniformBlockIndex(prgTerrain, "uniGradient");
    uniPatch    = glGetUniformBlockIndex(prgTerrain, "uniPatch");

    glUniformBlockBinding(prgTerrain, uniTerrain,  UNI_TERRAIN_BINDING);
    glUniformBlockBinding(prgTerrain, uniView,     UNI_VIEW_BINDING);
    glUniformBlockBinding(prgTerrain, uniGradient, UNI_GRADIENT_BINDING);
    glUniformBlockBinding(prgTerrain, uniPatch,    UNI_PATCH_BINDING);

    uniHeightmap = glGetUniformLocation(prgTerrain, "uHeightmap");
    uniColorRamp = glGetUniformLocation(prgTerrain, "samColorRamp");

    glUseProgram(prgTerrain);
    glUniform1i(uniHeightmap, 0);
    glUniform1i(uniColorRamp, 1);
    glUseProgram(0);

    uniTerrain  = glGetUniformBlockIndex(prgInstancedTerrain, "uniTerrain");
    uniView     = glGetUniformBlockIndex(prgInstancedTerrain, "uniView");
    uniGradient = glGetUniformBlockIndex(prgInstancedTerrain, "uniGradient");

    glUniformBlockBinding(prgInstancedTerrain, uniTerrain,  UNI_TERRAIN_BINDING);
    glUniformBlockBinding(prgInstancedTerrain, uniView,     UNI_VIEW_BINDING);
    glUniformBlockBinding(prgInstancedTerrain, uniGradient, UNI_GRADIENT_BINDING);

    uniHeightmap = glGetUniformLocation(prgInstancedTerrain, "uHeightmap");
    uniColorRamp = glGetUniformLocation(prgInstancedTerrain, "samColorRamp");

    glUseProgram(prgInstancedTerrain);
    glUniform1i(uniHeightmap, 0);
    glUniform1i(uniColorRamp, 1);
    glUseProgram(0);
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
    glDeleteTextures(1, &mColorRampTex);
}

void CDLODTerrain::calculateLODParams(vec4* morphParams)
{
    assert(LODCount<=MAX_LOD_COUNT);

    float	dim = (float)patchDim;
    float	size2=cellSize*cellSize*patchDim*patchDim,
            range=400.0f, curRange=0.0f;

    for (size_t i=0; i<LODCount; ++i)
    {
        //To avoid cracks when height is greater then minRange,
        float minRange = 2.0f*sqrt(size2+size2);
        range    = std::max(range, minRange);

        LODRange[i]  = curRange;
        curRange    += range;

        float rangeEnd   = curRange;
        float morphRange = range*morphZoneRatio;
        float morphStart = rangeEnd-morphRange;
        float rangeEndY   = curRange+heightScale/2.0f;
        float morphRangeY = range*morphZoneRatio;
        float morphStartY = rangeEndY-morphRangeY;

        morphParams[i] = vi_set(1.0f/morphRange, -morphStart/morphRange, 1.0f/morphRangeY, -morphStartY/morphRangeY);

        patchSize[i]     = dim;

        range *= 1.5f;

        size2 *= 4;
        dim   *= 2;
    }

    //Fix unnecessary morph in last LOD level
   morphParams[LODCount-1] = vi_load_zero();
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

void CDLODTerrain::addPatchToQueue(size_t level, float bx, float by)
{
    if (patchCount<MAX_PATCH_COUNT)
    {
        instData->baseX = bx;
        instData->baseY = by;
        instData->level = (float)level;

        ++instData;
        ++patchCount;
    }
}

void CDLODTerrain::selectQuadsForDrawing(size_t level, float bx, float by, bool skipFrustumTest)
{
    if (bx>=gridDimX-1 || by>=gridDimY-1) return;

    ml::aabb AABB;

    float sizeX = std::min<float>(patchSize[level], gridDimX-bx)*cellSize;
    float sizeY = std::min<float>(patchSize[level], gridDimY-by)*cellSize;

    float minX = startX+cellSize*bx;
    float minY = startY+cellSize*by;
    float maxX = minX+sizeX;
    float maxY = minY+sizeY;

    AABB.min = vi_set(minX, 0.0f,        minY, 1.0f);
    AABB.max = vi_set(maxX, heightScale, maxY, 1.0f);

    if (!skipFrustumTest)
    {
        //Check whether AABB intersects frustum
        int result = ml::intersectionTest(&AABB, &sseVP);

        if (result==ml::IT_OUTSIDE) return;
        skipFrustumTest = result==ml::IT_INSIDE;
    }

    if (level==0 || (LODRange[level]+heightScale<viewPoint.y/*-uOffset.y*/))
    {
        addPatchToQueue(level, bx, by);
        return;
    }

    //Check whether patch intersects it's LOD sphere and thus require subdivision
    glm::vec3 vp2d = viewPoint;
    vp2d.y = 0.0f;
    ml::aabb AABB2D = AABB;
    AABB2D.min.m128_f32[1] = 0.0f;
    AABB2D.max.m128_f32[1] = 0.0f;
    vec4 vp = vi_set(vp2d.x, vp2d.y, vp2d.z, 0.0);

    vec4 vdist;
        
    vdist = ml::distanceToAABB(&AABB, vi_set(viewPoint.x, viewPoint.y, viewPoint.z, 0.0));
    vdist = vi_shuffle<VI_SHUFFLE_MASK(VI_A_X, VI_B_X, VI_A_Z, VI_B_X)>(vdist, vi_load_zero());
    vdist = vi_dot3(vdist, vdist);

    float d2;
    _mm_store_ss(&d2, vdist);
        
    bool intersect = d2<LODRange[level]*LODRange[level];

    assert(intersect == ml::sphereAABBTest(&AABB2D, vp, LODRange[level]));

    if (!intersect
        /*maxX-vp2d.x<-LODs[level].rangeStart ||
        minX-vp2d.x>LODs[level].rangeStart ||
        maxY-vp2d.z<-LODs[level].rangeStart ||
        minY-vp2d.z>LODs[level].rangeStart*/
        )
    {
        addPatchToQueue(level, bx, by);
    }
    else
    {
        //TODO: add front to back sorting to minimize overdraw(optimization)
        float offset = patchSize[level]/2;
        selectQuadsForDrawing(level-1, bx,        by,        skipFrustumTest);
        selectQuadsForDrawing(level-1, bx+offset, by,        skipFrustumTest);
        selectQuadsForDrawing(level-1, bx,        by+offset, skipFrustumTest);
        selectQuadsForDrawing(level-1, bx+offset, by+offset, skipFrustumTest);
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

void CDLODTerrain::generateGeometry()
{
    glBindBuffer(GL_ARRAY_BUFFER, geomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*(patchDim+1)*(patchDim+1), 0, GL_STATIC_DRAW);
    glm::vec2* ptr = (glm::vec2*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (size_t y=0; y<=patchDim; ++y)
    {
        for (size_t x=0; x<=patchDim; ++x)
        {
            *ptr++ = glm::vec2(x, y);
        }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

#define INDEX_FOR_LOCATION(x, y) ((y)*(patchDim+1)+(x))

    idxCount = patchDim*patchDim*6;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)*idxCount, 0, GL_STATIC_DRAW);
    uint16_t* ptr2 = (uint16_t*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (size_t y=0; y<patchDim; ++y)
    {
        for (size_t x=0; x<patchDim; ++x)
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

    {
        PROFILER_CPU_BLOCK("Select");
        cpuSelectTimer.start();
        patchCount = 0;
        size_t level = LODCount-1;
        float step  = patchSize[level];
        for (float by=0; by<gridDimY-1; by+=step)
            for (float bx=0; bx<gridDimX-1; bx+=step)
                selectQuadsForDrawing(level, bx, by);
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

        float vertDistToTerrain = abs(viewPoint.y/*-uOffset.y*/-heightScale/2.0f);
        viewData.uLODViewK = vi_set(startX - viewPoint.x, vertDistToTerrain>heightScale/2.0?vertDistToTerrain-heightScale/2.0f:0.0f, startY - viewPoint.z, 0.0f);

        glBindBufferRange(GL_UNIFORM_BUFFER, UNI_TERRAIN_BINDING,  ubo, uniTerrainOffset,  sizeof(TerrainData));
        glBindBufferRange(GL_UNIFORM_BUFFER, UNI_VIEW_BINDING,     ubo, uniViewOffset,     sizeof(ViewData));
        glBindBufferRange(GL_UNIFORM_BUFFER, UNI_GRADIENT_BINDING, ubo, uniGradientOffset, sizeof(GradientData));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_1D, mColorRampTex);

        //TODO: Enable triangle culling and fix mesh(Partially done)
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if (drawWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(useInstancing?prgInstancedTerrain:prgTerrain);

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
    gridDimX = width;
    gridDimY = height;

    cellSize = 4.0f;
    startX = -0.5f*cellSize*(gridDimX-1);
    startY = -0.5f*cellSize*(gridDimY-1);

    patchDim = 8;

    //TODO: LODCount should clamped based on minPatchDim* and gridDimX
    LODCount = 5;
    //TODO: morphZoneRatio should should be less then 1-patchDiag/LODDistance
    morphZoneRatio = 0.30f;
    heightScale = 65535.0f/732.0f;

    maxPatchCount = MAX_PATCH_COUNT;

    generateGeometry();

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    uint8_t*      uniforms     = (uint8_t*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    TerrainData&  terrainData  = *(TerrainData*) (uniforms+uniTerrainOffset);
    GradientData& gradientData = *(GradientData*)(uniforms+uniGradientOffset);

    calculateLODParams(terrainData.uMorphParams);

    terrainData.uHMDim  = vi_set((GLfloat)width, (GLfloat)height, 1.0f/width, 1.0f/height);
    terrainData.uOffset = vi_set(startX, 0.0f, startY, 0.0f);
    terrainData.uScale  = vi_set(cellSize, heightScale, cellSize, 0.0f);
    memcpy(terrainData.uColors, colors, sizeof(terrainData.uColors));

    // Terrain Gradient
    const int stopCount = 3;
    float stops[stopCount] = {0.0000f, 0.500f, 1.000f};

    float stopsData[8*4], scalesData[8*4];

    for (int i=0; i<8*4; ++i)
    {
        stopsData[i]  = 1.0f;
        scalesData[i] = 0.0f;
    }

    memcpy(stopsData, stops, stopCount*4);

    for (int i=0; i<stopCount-1; ++i)
    {
        float delta = stops[i+1]-stops[i];
        assert(delta>=0);
        scalesData[i] = (delta>0)?(1.0f/delta):FLT_MAX;
    }

    memcpy(gradientData.uStops,  stopsData,  sizeof(gradientData.uStops));
    memcpy(gradientData.uScales, scalesData, sizeof(gradientData.uScales));
    gradientData.uInvStopCount = 1.0f/stopCount;

    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    unsigned char c[] = {
        0,   0, 255, 255,
        0, 255,   0, 255,
        255,   0,   0, 255,
    };

    glBindTexture(GL_TEXTURE_1D, mColorRampTex);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, stopCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, c);

    GLenum err = glGetError();
    assert(err==GL_NO_ERROR);
}
