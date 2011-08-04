#include "CDLODTerrain.h"

struct PatchData
{
	float baseX, baseY;
	GLint level;
};

#define ATTR_POSITION	0
#define ATTR_PATCH_BASE	1
#define ATTR_LEVEL		2

void CDLODTerrain::initialize()
{
	drawWireframe = false;

	terrainProgram  = resources::createProgramFromFiles("Terrain.CDLOD.Instanced.vert", "Terrain.HeightColored.frag");
	uniColors       = glGetUniformLocation(terrainProgram, "uColors");
	uniOffset       = glGetUniformLocation(terrainProgram, "uOffset");
	uniScale        = glGetUniformLocation(terrainProgram, "uScale");
	uniViewPos      = glGetUniformLocation(terrainProgram, "uViewPos");
	uniMVP          = glGetUniformLocation(terrainProgram, "uMVP");
	uniHMDim        = glGetUniformLocation(terrainProgram, "uHMDim");
	uniMorphParams  = glGetUniformLocation(terrainProgram, "uMorphParams");
	uniStops        = glGetUniformLocation(terrainProgram, "uStops");
	uniScales       = glGetUniformLocation(terrainProgram, "uScales");
	uniInvStopCount = glGetUniformLocation(terrainProgram, "uInvStopCount");

	GLint uniHeightmap = glGetUniformLocation(terrainProgram, "uHeightmap");
	GLint uniColorRamp = glGetUniformLocation(terrainProgram, "samColorRamp");

	glUseProgram(terrainProgram);
	glUniform1i(uniHeightmap, 0);
	glUniform1i(uniColorRamp, 1);

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

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &instVBO);
	glGenBuffers(1, &ibo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(ATTR_POSITION, 0);
	glBindBuffer(GL_ARRAY_BUFFER, instVBO);
	glVertexAttribPointer(ATTR_PATCH_BASE, 2, GL_FLOAT, GL_FALSE, sizeof(PatchData), (void*)0);
	glVertexAttribDivisor(ATTR_PATCH_BASE, 1);
	glVertexAttribPointer(ATTR_LEVEL, 1, GL_INT, GL_FALSE, sizeof(PatchData), (void*)8);
	glVertexAttribDivisor(ATTR_LEVEL, 1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glEnableVertexAttribArray(ATTR_POSITION);
	glEnableVertexAttribArray(ATTR_LEVEL);
	glEnableVertexAttribArray(ATTR_PATCH_BASE);
	glBindVertexArray(0);
	
	GLenum err = glGetError();
	assert(err==GL_NO_ERROR);
}

void CDLODTerrain::cleanup()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &instVBO);
	glDeleteBuffers(1, &ibo);
	if (minmaxData) free(minmaxData);
	glDeleteProgram(terrainProgram);
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

	size_t	dimX = minPatchDimX, dimY = minPatchDimY;
	size_t	patchCountX = (gridDimX+dimX-1)/dimX;
	size_t	patchCountY = (gridDimY+dimY-1)/dimY;
	size_t	patchCount = patchCountX*patchCountY;
	float	width2=size*size*minPatchDimX*minPatchDimX,
			height2=size*size*minPatchDimY*minPatchDimY,
			deltaZ2=heightScale*heightScale,
			curRange=0.0f;
			
	minmaxDataSize = 0;
	curRange=0.0f;
	float*	morphParamsIt = morphParams;

	for (size_t i=0; i<LODCount; ++i)
	{
		//To avoid cracks when height is greater then minRange,
		//minRange should take height into account too
		//minRange = 2.0f*sqrt(width^2+height^2+heightScale^2)
		float minRange = 2.0f*sqrt(width2+height2+deltaZ2);
		float range = std::max(ranges[i], minRange);

		LODs[i].rangeStart = curRange;
		curRange += range;
		
		float rangeEnd = curRange;
		float morphRange = range*morphZoneRatio;
		float morphStart = rangeEnd-morphRange;
		
		*morphParamsIt++ = 1.0f/morphRange; //morphInvRange
		*morphParamsIt++ = -morphStart/morphRange; //morphStartRatio

		LODs[i].width = dimX;
		LODs[i].height = dimY;
		LODs[i].minmaxPitch = patchCountX;
		LODs[i].minmaxOffset = minmaxDataSize;

		minmaxDataSize += patchCount*2;
		patchCountX = (patchCountX+1)/2;
		patchCountY = (patchCountY+1)/2;
		patchCount = patchCountX*patchCountY;
		width2 *= 4;
		height2 *= 4;
		dimX *= 2;
		dimY *= 2;
	}

	assert(morphParamsIt-morphParams==LODCount*2);
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
	sseMVP.r0 = vi_loadu(mat[0]);
	sseMVP.r1 = vi_loadu(mat[1]);
	sseMVP.r2 = vi_loadu(mat[2]);
	sseMVP.r3 = vi_loadu(mat[3]);
}

void CDLODTerrain::addPatchToQueue(size_t level, size_t i, size_t j)
{
	assert(instCount<MAX_PATCH_COUNT);
	instData->baseX = (float)i;
	instData->baseY = (float)j;
	instData->level = level;

	++instData;
	++instCount;
}

void CDLODTerrain::getMinMaxZ(size_t level, size_t i, size_t j, float* minZ, float* maxZ)
{
	float* minmax = minmaxData+LODs[level].minmaxOffset;

	*minZ = minmax[2*((j/LODs[level].height)*LODs[level].minmaxPitch+(i/LODs[level].width))+0];
	*maxZ = minmax[2*((j/LODs[level].height)*LODs[level].minmaxPitch+(i/LODs[level].width))+1];

	assert(0<=*minZ && *minZ<=*maxZ);
	assert(*minZ<=*maxZ && *maxZ<=64.0f);
}

void CDLODTerrain::getAABB(size_t level, size_t i, size_t j, ml::aabb* patchAABB)
{
	float* minmax = minmaxData+LODs[level].minmaxOffset;

	float minZ = minmax[2*((j/LODs[level].height)*LODs[level].minmaxPitch+(i/LODs[level].width))+0];
	float maxZ = minmax[2*((j/LODs[level].height)*LODs[level].minmaxPitch+(i/LODs[level].width))+1];

	assert(0<=minZ && minZ<=maxZ);
	assert(minZ<=maxZ && maxZ<=heightScale);

	float sizeX = LODs[level].width*size;
	float sizeY = LODs[level].height*size;

	float baseX = startX+i*size;
	float baseY = startY+j*size;

	patchAABB->min = vi_set(baseX, minZ, baseY, 1);
	patchAABB->max = vi_set(baseX+sizeX, maxZ, baseY+sizeY, 1);
}


int CDLODTerrain::intersectViewFrustum(size_t level, size_t i, size_t j)
{
	ml::aabb AABB;

	getAABB(level, i, j, &AABB);

	return ml::intersectionTest(&AABB, &sseVP);
}

bool CDLODTerrain::intersectSphere(size_t level, size_t i, size_t j)
{
	float baseX = startX+i*size;
	float baseY = startY+j*size;

	float sizeX = LODs[level].width*size;
	float sizeY = LODs[level].height*size;

	float r = LODs[level].rangeStart;

	ml::aabb bb;
	getAABB(level, i, j, &bb);
	bool b1 = ml::sphereAABBTest(&bb, vi_set(viewPoint.x, viewPoint.y, viewPoint.z, 0), r); 

	//TODO: remove scalar version
	//find the square of the distance
	//from the sphere to the box
	float minZ, maxZ;
	getMinMaxZ(level, i, j, &minZ, &maxZ);
	glm::vec3 bmin = glm::vec3(baseX, minZ, baseY);
	glm::vec3 bmax = glm::vec3(baseX+sizeX, maxZ, baseY+sizeY);
	float s, d = 0;

	for (size_t i=0 ; i<3 ; i++) 
	{
		if (viewPoint[i] < bmin[i])
		{
			s = viewPoint[i] - bmin[i];
			d += s*s;
		}
		else if (viewPoint[i] > bmax[i])
		{
			s = viewPoint[i] - bmax[i];
			d += s*s;
		}
	}

	bool b2 = d <= r*r;

	assert(b1==b2);

	return b1;
}

void CDLODTerrain::selectQuadsForDrawing(size_t level, size_t i, size_t j, bool skipFrustumTest)
{
	if (!skipFrustumTest)
	{
		int result = intersectViewFrustum(level, i, j);

		if (result==ml::IT_OUTSIDE) return;
		skipFrustumTest = result==ml::IT_INSIDE;
	}

	if (level==0)
	{
		addPatchToQueue(level, i, j);
		return;
	}

	if (!intersectSphere(level, i, j))
		addPatchToQueue(level, i, j);
	else
	{
		//TODO: add front to back sorting to minimize overdraw(optimization)
		size_t offsetX = LODs[level].width/2;
		size_t offsetY = LODs[level].height/2;
		selectQuadsForDrawing(level-1, i, j, skipFrustumTest);
		selectQuadsForDrawing(level-1, i+offsetX, j, skipFrustumTest);
		selectQuadsForDrawing(level-1, i, j+offsetY, skipFrustumTest);
		selectQuadsForDrawing(level-1, i+offsetX, j+offsetY, skipFrustumTest);
	}
}

glm::vec3 colors[] = 
{
	glm::vec3(0.5f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 0.6f, 0.6f),
	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 0.2f, 0.2f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 1.0f)
};

void CDLODTerrain::generateGeometry()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*(minPatchDimY+1)*(minPatchDimX+1), 0, GL_STATIC_DRAW);
	glm::vec2* ptr = (glm::vec2*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (size_t y=0; y<=minPatchDimY; ++y)
	{
		for (size_t x=0; x<=minPatchDimX; ++x)
		{
			*ptr++ = glm::vec2(x, y);
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

#define INDEX_FOR_LOCATION(x, y) ((y)*(minPatchDimY+1)+(x))

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)*(minPatchDimY)*(minPatchDimX)*6, 0, GL_STATIC_DRAW);
	uint16_t* ptr2 = (uint16_t*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (size_t y=0; y<minPatchDimY; ++y)
	{
		for (size_t x=0; x<minPatchDimX; ++x)
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
	cpuTimer.start();

	glBindBuffer(GL_ARRAY_BUFFER, instVBO);
	//Discard data from previous frame
	glBufferData(GL_ARRAY_BUFFER, MAX_PATCH_COUNT*sizeof(PatchData), 0, GL_STREAM_DRAW);
	instData = (PatchData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	instCount = 0;

	cpuSelectTimer.start();
	size_t level = LODCount-1;
	for (size_t j=0; j<gridDimY; j+=LODs[level].height)
		for (size_t i=0; i<gridDimX; i+=LODs[level].width)
			selectQuadsForDrawing(level, i, j);
	cpuSelectTime = cpuSelectTimer.elapsed();
	cpuSelectTimer.stop();

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	cpuDrawTimer.start();
	gpuTimer.start();

	if (instCount)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glUseProgram(terrainProgram);
		glUniform3fv(uniViewPos, 1, viewPoint);

		glUniformMatrix4fv(uniMVP, 1, false, (float*)&sseMVP);

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

		glBindVertexArray(vao);
		glDrawElementsInstancedARB(GL_TRIANGLES, (GLsizei)(minPatchDimY)*(minPatchDimX)*6, GL_UNSIGNED_SHORT, 0, instCount);
		glBindVertexArray(0);

		glUseProgram(0);
		glDisableClientState(GL_VERTEX_ARRAY);
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

void CDLODTerrain::setHeightmap(uint8_t* data, size_t width, size_t height)
{
	gridDimX = width;
	gridDimY = height;

	size = 1.0f;
	startX = -0.5f*size*(gridDimX-1);
	startY = -0.5f*size*(gridDimY-1);

	minPatchDimX = 8;
	minPatchDimY = 8;

	visibilityDistance = 600;
	//TODO: LODCount should clamped based on minPatchDim* and gridDimX
	LODCount = 5;
	detailBalance = 2.0f;
	//TODO: morphZoneRatio should should be less then 1-patchDiag/LODDistance
	morphZoneRatio = 0.30f;
	heightScale = 32.0f;

	calculateLODParams();
	generateGeometry();
	generateBBoxData(data);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	glUseProgram(terrainProgram);
	glUniform4f(uniHMDim, (GLfloat)width, (GLfloat)height, 1.0f/width, 1.0f/height);
	glUniform3f(uniOffset, startX, 0.0f, startY);
	glUniform3f(uniScale, size, heightScale, size);
	glUniform2fv(uniMorphParams, LODCount, morphParams);
	glUniform3fv(uniColors, 8, colors[0]);

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

	glUniform4fv(uniStops,  1, stopsData);
	glUniform4fv(uniScales, 1, scalesData);
	glUniform1f(uniInvStopCount, 1.0f/stopCount);

	unsigned char c[] = {
		  0,   0, 255, 255,
		  0, 255,   0, 255,
		255,   0,   0, 255,
	};

	glBindTexture(GL_TEXTURE_1D, mColorRampTex);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, stopCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, c);

	glUseProgram(0);

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

void downsamplep1(size_t w, size_t h, const unsigned char* inData,
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
					float value = inData[v*w+u]/255.0f;
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

void CDLODTerrain::generateBBoxData(uint8_t* data)
{
	minmaxData = (float*)malloc(minmaxDataSize*sizeof(float));

	size_t w, h;
	downsamplep1(gridDimX, gridDimY, data, minPatchDimX+1, minPatchDimY+1, heightScale, w, h, minmaxData+LODs[0].minmaxOffset);

	for (size_t i=1; i<LODCount; ++i)
	{
		assert((LODs[i].minmaxOffset-LODs[i-1].minmaxOffset) == 2*w*h);
		downsample(w, h,  minmaxData+LODs[i-1].minmaxOffset, 2, 2, w, h, minmaxData+LODs[i].minmaxOffset);
	}
	assert(LODs[LODCount-1].minmaxOffset+2*w*h==minmaxDataSize);
}
