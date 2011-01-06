#include "CDLODTerrain.h"

void CDLODTerrain::initialize()
{
	terrainProgram = resources::createProgramFromFiles("Terrain.CDLOD.vert", "FF.Color.frag");
	uniPatchBase = glGetUniformLocation(terrainProgram, "uPatchBase");
	uniPatchDim = glGetUniformLocation(terrainProgram, "uPatchDim");
	uniOffset = glGetUniformLocation(terrainProgram, "uOffset");
	uniScale = glGetUniformLocation(terrainProgram, "uScale");
	uniViewPos = glGetUniformLocation(terrainProgram, "uViewPos");
	uniInvHMSize = glGetUniformLocation(terrainProgram, "uInvHMSize");
	uniHeightScale = glGetUniformLocation(terrainProgram, "uHeightScale");
	uniMorphParams = glGetUniformLocation(terrainProgram, "uMorphParams");

	GLint uniHeightmap = glGetUniformLocation(terrainProgram, "uHeightmap");

	glUseProgram(terrainProgram);
	glUniform1i(uniHeightmap, 0);

	glGenTextures(1, &mHeightmapTex);
	glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
}

void CDLODTerrain::cleanup()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	if (bboxZData) free(bboxZData);
	glDeleteProgram(terrainProgram);
	glDeleteTextures(1, &mHeightmapTex);
}

void CDLODTerrain::setupLODParams()
{
	float	curScale=size,
			curDetailBalance=1.0f,
			totalDetail = 0.0f,
			curRange=0.0f;

	for (GLint i=0; i<LODCount; ++i)
	{
		LODs[i].scaleX = curScale;
		LODs[i].scaleY = curScale;
		//Multiply by to to ensure at least one could be morphed into higher level quad, as another one can be subdivided
		LODs[i].minRange = 2.0f*sqrt(LODs[i].scaleX*LODs[i].scaleX*minPatchDimX*minPatchDimX+LODs[i].scaleY*LODs[i].scaleY*minPatchDimY*minPatchDimY);

		curScale *= 2;
		totalDetail += curDetailBalance;
		curDetailBalance *= detailBalance;
	}

	size_t	dimX = minPatchDimX, dimY = minPatchDimY;
	size_t	patchCountX = (gridDimX+dimX-1)/dimX;
	size_t	patchCountY = (gridDimY+dimY-1)/dimY;
	size_t	patchCount = patchCountX*patchCountY;
	float	minDetailDist = visibilityDistance/totalDetail;
	curDetailBalance = 1.0f;

	bboxZDataSize = 0;
	for (GLint i=0; i<LODCount; ++i)
	{
		float range = std::max(minDetailDist*curDetailBalance, LODs[i].minRange);

		LODs[i].rangeStart = curRange;
		curRange += range;
		LODs[i].rangeEnd = curRange;

		float morphRange = range*morphZoneRatio;

		LODs[i].morphStart = LODs[i].rangeEnd-morphRange;
		LODs[i].morphInvRange = 1.0f/morphRange;
		LODs[i].morphStartRatio = -LODs[i].morphStart/morphRange;
		LODs[i].patchDimX = dimX;
		LODs[i].patchDimY = dimY;
		LODs[i].pitch = patchCountX;
		LODs[i].bboxZDataOffset = bboxZDataSize;

		bboxZDataSize += patchCount*2;
		patchCountX = (patchCountX+1)/2;
		patchCountY = (patchCountY+1)/2;
		patchCount = patchCountX*patchCountY;
		dimX *= 2;
		dimY *= 2;
		curDetailBalance *= 2.0f;
	}
}

void CDLODTerrain::setViewProj(glm::mat4& mat)
{
	sseVP.r0 = vi_loadu(mat[0]);
	sseVP.r1 = vi_loadu(mat[1]);
	sseVP.r2 = vi_loadu(mat[2]);
	sseVP.r3 = vi_loadu(mat[3]);
}

void CDLODTerrain::addPatchToQueue(size_t level, size_t i, size_t j)
{
	size_t index = patchList.size();
	patchList.resize(index+1);

	float scaleX = LODs[level].scaleX;
	float scaleY = LODs[level].scaleY;

	patchList[index].baseX = i;
	patchList[index].baseY = j;
	patchList[index].scaleX = scaleX;
	patchList[index].scaleY = scaleY;
	patchList[index].level = level;
}

void CDLODTerrain::getMinMaxZ(size_t level, size_t i, size_t j, float* minZ, float* maxZ)
{
	float* minmax = bboxZData+LODs[level].bboxZDataOffset;

	*minZ = minmax[2*((j/LODs[level].patchDimY)*LODs[level].pitch+(i/LODs[level].patchDimX))+0];
	*maxZ = minmax[2*((j/LODs[level].patchDimY)*LODs[level].pitch+(i/LODs[level].patchDimX))+1];

	assert(0<=*minZ && *minZ<=*maxZ);
	assert(*minZ<=*maxZ && *maxZ<=64.0f);
}

void CDLODTerrain::getAABB(size_t level, size_t i, size_t j, ml::aabb* patchAABB)
{
	float* minmax = bboxZData+LODs[level].bboxZDataOffset;

	float minZ = minmax[2*((j/LODs[level].patchDimY)*LODs[level].pitch+(i/LODs[level].patchDimX))+0];
	float maxZ = minmax[2*((j/LODs[level].patchDimY)*LODs[level].pitch+(i/LODs[level].patchDimX))+1];

	assert(0<=minZ && minZ<=maxZ);
	assert(minZ<=maxZ && maxZ<=heightScale);

	float sizeX = LODs[level].patchDimX*size;
	float sizeY = LODs[level].patchDimY*size;

	float baseX = startX+i*size;
	float baseY = startY+j*size;

	patchAABB->min = vi_set(baseX, minZ, baseY, 1);
	patchAABB->max = vi_set(baseX+sizeX, maxZ, baseY+sizeY, 1);
}


int CDLODTerrain::intersectViewFrustum(size_t level, size_t i, size_t j)
{
	//float sizeX = LODs[level].patchDimX*size;
	//float sizeY = LODs[level].patchDimY*size;

	//float baseX = startX+i*size;
	//float baseY = startY+j*size;

	ml::aabb AABB;
	//float minZ, maxZ;

	//getMinMaxZ(level, i, j, &minZ, &maxZ);
	//AABB.min = vi_set(baseX, minZ, baseY, 1);
	//AABB.max = vi_set(baseX+sizeX, maxZ, baseY+sizeY, 1);

	getAABB(level, i, j, &AABB);

	return ml::intersectionTest(&AABB, &sseVP);
}

bool CDLODTerrain::intersectSphere(size_t level, size_t i, size_t j)
{
	float baseX = startX+i*size;
	float baseY = startY+j*size;

	float sizeX = LODs[level].patchDimX*size;
	float sizeY = LODs[level].patchDimY*size;

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
		size_t offsetX = LODs[level].patchDimX/2;
		size_t offsetY = LODs[level].patchDimY/2;
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
			*ptr2++ = INDEX_FOR_LOCATION(x+1, y);
			*ptr2++ = INDEX_FOR_LOCATION(x,   y+1);
			*ptr2++ = INDEX_FOR_LOCATION(x+1, y);
			*ptr2++ = INDEX_FOR_LOCATION(x,   y+1);
			*ptr2++ = INDEX_FOR_LOCATION(x+1, y+1);
		}
	}

#undef INDEX_FOR_LOCATION

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CDLODTerrain::drawPatch(float baseX, float baseY, float scaleX, float scaleY, int level)
{
	glUseProgram(terrainProgram);

	glColor3fv(colors[level]);

	glUniform2f(uniPatchBase, baseX, baseY);
	glUniform2f(uniPatchDim, 1<<level, 1<<level);
	glUniform3fv(uniViewPos, 1, viewPoint);
	glUniform2f(uniScale, size, size);

	glUniform2f(uniMorphParams, LODs[level].morphInvRange, LODs[level].morphStartRatio);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(2, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, (GLsizei)(minPatchDimY)*(minPatchDimX)*6, GL_UNSIGNED_SHORT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPopAttrib();
}

void CDLODTerrain::drawTerrain()
{
	cpuTimer.start();
	patchList.reserve(2048);
	patchList.clear();

	size_t level = 5;
	for (size_t j=0; j<gridDimY; j+=LODs[level].patchDimY)
		for (size_t i=0; i<gridDimX; i+=LODs[level].patchDimX)
			selectQuadsForDrawing(level, i, j);

	auto	it  = patchList.begin(),
			end = patchList.end();

	gpuTimer.start();
	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
	it  = patchList.begin();
	for(; it!=end; ++it)
	{
		drawPatch(it->baseX, it->baseY, it->scaleX, it->scaleY, it->level);
	}
	gpuTimer.stop();
	cpuTime = cpuTimer.elapsed();
	gpuTime = gpuTimer.getResult();
	cpuTimer.stop();
}

void CDLODTerrain::setHeightmap(uint8_t* data, size_t width, size_t height)
{
	gridDimX = width;
	gridDimY = height;

	size = 1;
	startX = -0.5f*size*(gridDimX-1);
	startY = -0.5f*size*(gridDimY-1);

	minPatchDimX = 4;
	minPatchDimY = 4;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, mHeightmapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	visibilityDistance = size*50;
	LODCount = 6;
	detailBalance = 2.0f;
	morphZoneRatio = 0.30f;
	heightScale = 64;

	glUseProgram(terrainProgram);
	glUniform2f(uniInvHMSize, 1.0f/width, 1.0f/height);
	glUniform2f(uniOffset, startX, startY);
	glUniform1f(uniHeightScale, heightScale);
	glUseProgram(0);

	setupLODParams();
	generateGeometry();
	generateBBoxData(data);
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
	bboxZData = (float*)malloc(bboxZDataSize*sizeof(float));

	size_t w, h;
	downsamplep1(gridDimX, gridDimY, data, minPatchDimX+1, minPatchDimY+1, heightScale, w, h, bboxZData+LODs[0].bboxZDataOffset);

	for (size_t i=1; i<LODCount; ++i)
	{
		assert((LODs[i].bboxZDataOffset-LODs[i-1].bboxZDataOffset) == 2*w*h);
		downsample(w, h,  bboxZData+LODs[i-1].bboxZDataOffset, 2, 2, w, h, bboxZData+LODs[i].bboxZDataOffset);
	}
	assert(LODs[LODCount-1].bboxZDataOffset+2*w*h==bboxZDataSize);
}
