#include "CDLODTerrain.h"
#include <profiler.h>

struct PatchData
{
    float baseX, baseY;
    float level;
    float padding;
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
    if (minmaxData) free(minmaxData);
    glDeleteProgram(prgTerrain);
    glDeleteProgram(prgInstancedTerrain);
    glDeleteTextures(1, &mHeightmapTex);
    glDeleteTextures(1, &mColorRampTex);
}

void CDLODTerrain::calculateLODRanges(float* ranges)
{
    float	curDetailBalance=1.0f,
        totalDetail = 0.0f;

    for (size_t i=0; i<LODCount; ++i)
    {
        totalDetail += curDetailBalance;
        curDetailBalance *= detailBalance;
    }

    float	minDetailDist = visibilityDistance/totalDetail;

    curDetailBalance = 1.0f;

    for (size_t i=0; i<LODCount; ++i)
    {
        float range = minDetailDist*curDetailBalance;
        ranges[i] = range;
        curDetailBalance *= detailBalance;
    }
}

void CDLODTerrain::calculateLODParams()
{
    float ranges[MAX_LOD_COUNT]; 
    assert(LODCount<=MAX_LOD_COUNT);

    calculateLODRanges(ranges);

    size_t	dim = patchDim;
    size_t	patchCountX = (gridDimX-1+dim-1)/dim;
    size_t	patchCountY = (gridDimY-1+dim-1)/dim;
    size_t	patchCount = patchCountX*patchCountY;
    float	size2=cellSize*cellSize*patchDim*patchDim,
        deltaZ2=heightScale*heightScale,
        curRange=0.0f;

    minmaxDataSize = 0;
    curRange=0.0f;

    for (size_t i=0; i<LODCount; ++i)
    {
        //To avoid cracks when height is greater then minRange,
        //minRange should take height into account too
        //minRange = 2.0f*sqrt(width^2+height^2+heightScale^2)
        float minRange = 2.0f*sqrt(size2+size2+deltaZ2);
        float range    = std::max(ranges[i], minRange);

        LODs[i].rangeStart  = curRange;
        curRange           += range;

        float rangeEnd   = curRange;
        float morphRange = range*morphZoneRatio;
        float morphStart = rangeEnd-morphRange;

        terrainData.uMorphParams[i] = vi_set(1.0f/morphRange, -morphStart/morphRange, 0, 0);

        LODs[i].patchDim     = dim;
        LODs[i].minmaxPitch  = patchCountX;
        LODs[i].minmaxOffset = minmaxDataSize;

        minmaxDataSize += patchCount*2;
        patchCountX     = (patchCountX+1)/2;
        patchCountY     = (patchCountY+1)/2;
        patchCount      = patchCountX*patchCountY;

        size2 *= 4;
        dim   *= 2;
    }
    //Fix unnecessary morph in last LOD level
    terrainData.uMorphParams[LODCount-1] = vi_load_zero();
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

void CDLODTerrain::addPatchToQueue(size_t level, size_t i, size_t j)
{
    if (patchCount<maxPatchCount)
    {
        instData->baseX = (float)i;
        instData->baseY = (float)j;
        instData->level = (float)level;

        ++instData;
        ++patchCount;
    }
}

void CDLODTerrain::getAABB(size_t level, size_t i, size_t j, ml::aabb* patchAABB)
{
    float* minmax = minmaxData+LODs[level].minmaxOffset;

    float minZ = minmax[2*((j/LODs[level].patchDim)*LODs[level].minmaxPitch+(i/LODs[level].patchDim))+0];
    float maxZ = minmax[2*((j/LODs[level].patchDim)*LODs[level].minmaxPitch+(i/LODs[level].patchDim))+1];

    assert(0<=minZ && minZ<=maxZ);
    assert(minZ<=maxZ && maxZ<=heightScale);

    float sizeX = std::min(LODs[level].patchDim,  gridDimX-i)*cellSize;
    float sizeY = std::min(LODs[level].patchDim, gridDimY-j)*cellSize;

    float baseX = startX-cellSize*i;
    float baseY = startY+cellSize*j;

    patchAABB->min = vi_set(baseX-sizeX, minZ, baseY,       1);
    patchAABB->max = vi_set(baseX,       maxZ, baseY+sizeY, 1);
}

void CDLODTerrain::selectQuadsForDrawing(size_t level, size_t i, size_t j, bool skipFrustumTest)
{
    if (i>=gridDimX-1 || j>=gridDimY-1) return;

    ml::aabb AABB;

    getAABB(level, i, j, &AABB);

    if (!skipFrustumTest)
    {
        //Check whether AABB intersects frustum
        int result = ml::intersectionTest(&AABB, &sseVP);

        if (result==ml::IT_OUTSIDE) return;
        skipFrustumTest = result==ml::IT_INSIDE;
    }

    if (level==0)
    {
        addPatchToQueue(level, i, j);
        return;
    }

    //Check whether patch intersects it's LOD sphere and thus require subdivision
    if (!ml::sphereAABBTest(&AABB, viewData.uViewPoint, LODs[level].rangeStart))
    {
        addPatchToQueue(level, i, j);
    }
    else
    {
        //TODO: add front to back sorting to minimize overdraw(optimization)
        size_t offset = LODs[level].patchDim/2;
        selectQuadsForDrawing(level-1, i,        j, skipFrustumTest);
        selectQuadsForDrawing(level-1, i+offset, j, skipFrustumTest);
        selectQuadsForDrawing(level-1, i,        j+offset, skipFrustumTest);
        selectQuadsForDrawing(level-1, i+offset, j+offset, skipFrustumTest);
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
            *ptr2++ = INDEX_FOR_LOCATION(x+1, y);
            *ptr2++ = INDEX_FOR_LOCATION(x,   y+1);
            *ptr2++ = INDEX_FOR_LOCATION(x+1, y);
            *ptr2++ = INDEX_FOR_LOCATION(x+1, y+1);
            *ptr2++ = INDEX_FOR_LOCATION(x,   y+1);
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
        size_t step  = LODs[level].patchDim;
        for (size_t j=0; j<gridDimY-1; j+=step)
            for (size_t i=0; i<gridDimX-1; i+=step)
                selectQuadsForDrawing(level, i, j);
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

    if (patchCount)
    {
        PROFILER_CPU_BLOCK("Render");
        glPushAttrib(GL_ALL_ATTRIB_BITS);

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
    startX =  0.5f*cellSize*(gridDimX-1);
    startY = -0.5f*cellSize*(gridDimY-1);

    patchDim = 8;

    visibilityDistance = 300;
    //TODO: LODCount should clamped based on minPatchDim* and gridDimX
    LODCount = 5;
    detailBalance = 2.0f;
    //TODO: morphZoneRatio should should be less then 1-patchDiag/LODDistance
    morphZoneRatio = 0.30f;
    heightScale = 65535.0f/732.0f;

    maxPatchCount = MAX_PATCH_COUNT;

    calculateLODParams();
    generateGeometry();
    generateBBoxData(data);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, data);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    terrainData.uHMDim  = vi_set((GLfloat)width, (GLfloat)height, 1.0f/width, 1.0f/height);
    terrainData.uOffset = vi_set(startX, 0.0f, startY, 0.0f);
    terrainData.uScale  = vi_set(-cellSize, heightScale, cellSize, 0.0f);
    memcpy(terrainData.uColors, colors, sizeof(terrainData.uColors));
    glBufferSubData(GL_UNIFORM_BUFFER, uniTerrainOffset, sizeof(TerrainData), &terrainData);

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
    glBufferSubData(GL_UNIFORM_BUFFER, uniGradientOffset, sizeof(GradientData), &gradientData);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

void downsample(size_t w, size_t h, const float* inData,
    size_t kx, size_t ky,
    size_t& w2, size_t& h2, float* outData)
{
    float* base = outData;
    float* cur = base;

    w2 = w/kx+!!(w%kx);
    h2 = h/ky+!!(h%ky);

    for (size_t baseY=0; baseY<w; baseY+=kx)
    {
        for (size_t baseX=0; baseX<h; baseX+=ky)
        {
            float mn=255.0, mx = 0.0;
            size_t endX=std::min(baseX+kx, w);
            size_t endY=std::min(baseY+ky, h);

            for (size_t v=baseY; v<endY; ++v)
            {
                for (size_t u=baseX; u<endX; ++u)
                {
                    mn = std::min(mn, inData[2*(v*w+u)+0]);//first is min value
                    mx = std::max(mx, inData[2*(v*w+u)+1]);//second is max value
                }
            }

            *cur++ = mn;
            *cur++ = mx;
        }
    }

    assert(cur-base==w2*h2*2);
}

void downsamplep1(size_t w, size_t h, const uint16_t* inData,
    size_t kx, size_t ky, float heightScale,
    size_t& w2, size_t& h2, float* outData)
{
    float* base = outData;
    float* cur = base;

    w2 = (w-1)/(kx-1)+!!((w-1)%(kx-1));
    h2 = (h-1)/(ky-1)+!!((h-1)%(ky-1));

    for (size_t baseY=0; baseY<h-1; baseY+=ky-1)
    {
        for (size_t baseX=0; baseX<w-1; baseX+=kx-1)
        {
            float mn=255.0, mx = 0.0;
            size_t endX=std::min(baseX+kx, w);
            size_t endY=std::min(baseY+ky, h);

            for (size_t v=baseY; v<endY; ++v)
            {
                for (size_t u=baseX; u<endX; ++u)
                {
                    float value = inData[v*w+u]/65535.0f;
                    mn = std::min(mn, value);
                    mx = std::max(mx, value);
                }
            }

            *cur++ = mn*heightScale;
            *cur++ = mx*heightScale;
        }
    }

    assert(cur-base==w2*h2*2);
}

void CDLODTerrain::generateBBoxData(uint16_t* data)
{
    minmaxData = (float*)malloc(minmaxDataSize*sizeof(float));

    size_t w, h;
    downsamplep1(gridDimX, gridDimY, data, patchDim+1, patchDim+1, heightScale, w, h, minmaxData+LODs[0].minmaxOffset);

    for (size_t i=1; i<LODCount; ++i)
    {
        assert((LODs[i].minmaxOffset-LODs[i-1].minmaxOffset) == 2*w*h);
        downsample(w, h,  minmaxData+LODs[i-1].minmaxOffset, 2, 2, w, h, minmaxData+LODs[i].minmaxOffset);
    }
    assert(LODs[LODCount-1].minmaxOffset+2*w*h==minmaxDataSize);
}
