#include "CDLODTerrain.h"

void CDLODTerrain::setupTerrainParams()
{
	terrainProgram = resources::createProgramFromFiles("Terrain.CDLOD.vert", "FF.Color.frag");
	uniOffset = glGetUniformLocation(terrainProgram, "uOffset");
	uniScale = glGetUniformLocation(terrainProgram, "uScale");
	uniViewPos = glGetUniformLocation(terrainProgram, "uViewPos");
	uniMorphParams = glGetUniformLocation(terrainProgram, "uMorphParams");

	gridDimX = 257;
	gridDimY = 257;
	size = 20;
	startX = -0.5f*size*(gridDimX-1);
	startY = -0.5f*size*(gridDimY-1);
	endX = -startX;
	endY = -startY;
	minPatchDimX = 4;
	minPatchDimY = 4;

	visibilityDistance = size*50;
	LODCount = 4;
	detailBalance = 2.0f;
	morphZoneRatio = 0.30f;

	setupLODParams();
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

	size_t dimX = minPatchDimX, dimY = minPatchDimY;
	float minDetailDist = visibilityDistance/totalDetail;
	curDetailBalance = 1.0f;

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
				
		dimX *= 2;
		dimY *= 2;
		curDetailBalance *= 2.0f;
	}
}
	
void CDLODTerrain::addPatchToQueue(size_t level, size_t i, size_t j)
{
	size_t index = patchList.size();
	patchList.resize(index+1);

	float scaleX = LODs[level].scaleX;
	float scaleY = LODs[level].scaleY;

	patchList[index].baseX = startX+i*size;
	patchList[index].baseY = startY+j*size;
	patchList[index].scaleX = scaleX;
	patchList[index].scaleY = scaleY;
	patchList[index].level = level;
}

bool CDLODTerrain::intersectViewFrustum(size_t level, size_t i, size_t j)
{
	float sizeX = LODs[level].patchDimX*size;
	float sizeY = LODs[level].patchDimY*size;

	float baseX = startX+i*size;
	float baseY = startY+j*size;

	glm::vec4 pts[4] = {
		glm::vec4(baseX, 0, baseY, 1),
		glm::vec4(baseX+sizeX, 0, baseY, 1),
		glm::vec4(baseX, 0, baseY+sizeY, 1),
		glm::vec4(baseX+sizeX, 0, baseY+sizeY, 1)
	};

	bool outside = false;

	for (size_t i=0; i<4; ++i)
	{
		glm::vec4 p = VP*pts[i];
		outside |= p.w>0 &&
					-p.w<=p.x && p.x<=p.w &&
					/*-p.w>p.y || p.y>p.w ||*/
					-p.w<=p.z && p.z<=p.w;
	}

	return outside;
}

bool CDLODTerrain::intersectSphere(size_t level, size_t i, size_t j)
{
	float baseX = startX+i*size;
	float baseY = startY+j*size;

	float sizeX = LODs[level].patchDimX*size;
	float sizeY = LODs[level].patchDimY*size;

	//find the square of the distance
	//from the sphere to the box
	glm::vec3 bmin = glm::vec3(baseX, 0, baseY);
	glm::vec3 bmax = glm::vec3(baseX+sizeX, 0, baseY+sizeY);
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

	float r = LODs[level].rangeStart;

	return d <= r*r;
}

//TODO: implement min rendering dist - essentially startRange
//TODO: list of high level quads.
//TODO: when rendring terrain - pyramid of min, max height
//TODO: optimize - skip frustum test if parent inside
void CDLODTerrain::selectQuadsForDrawing(size_t level, size_t i, size_t j)
{
	if (!intersectViewFrustum(level, i, j))
		return;

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
		selectQuadsForDrawing(level-1, i, j);
		selectQuadsForDrawing(level-1, i+offsetX, j);
		selectQuadsForDrawing(level-1, i, j+offsetY);
		selectQuadsForDrawing(level-1, i+offsetX, j+offsetY);
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

void CDLODTerrain::drawPatch(float baseX, float baseY, float scaleX, float scaleY, int level)
{
	glUseProgram(terrainProgram);

	//glColor3f(1.0f, 1.0f, 0.0f);
	glColor3fv(colors[level]);

	glUniform2f(uniOffset, baseX, baseY);
	glUniform3fv(uniViewPos, 1, viewPoint);
	glUniform2f(uniScale, LODs[level].scaleX, LODs[level].scaleY);
			
	//TODO: HACK!!!!
	glDisable(GL_DEPTH_TEST);

	//Implement in proper way - acces to this params on per level basis
	float startMorph = 2.0f*size;
	float endMorph = 4.0f*size;
	glUniform2f(uniMorphParams, LODs[level].morphInvRange, LODs[level].morphStartRatio);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
		for (float y=0; y<minPatchDimY; y+=1.0f)
		{
			for (float x=0; x<minPatchDimX; x+=1.0f)
			{
				glVertex2f(x,	y);
				glVertex2f(x+1,	y);
				glVertex2f(x,	y+1);
				glVertex2f(x+1,	y);
				glVertex2f(x,	y+1);
				glVertex2f(x+1,	y+1);
			}
		}
	glEnd();
	glPopAttrib();
}

void CDLODTerrain::drawTerrain()
{
	patchList.reserve(2048);
	patchList.clear();

	size_t level = 2;
	for (size_t j=0; j<gridDimY; j+=LODs[level].patchDimY)
		for (size_t i=0; i<gridDimX; i+=LODs[level].patchDimX)
			selectQuadsForDrawing(level, i, j);

	auto	it  = patchList.begin(),
			end = patchList.end();

	it  = patchList.begin();
	for(; it!=end; ++it)
	{
		drawPatch(it->baseX, it->baseY, it->scaleX, it->scaleY, it->level);
	}
}
