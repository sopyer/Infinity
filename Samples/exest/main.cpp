#define GLEW_STATIC
#include <framework.h>
#include <glm\glmext.h>
#include <cassert>
#include <ResourceHelpers.h>

#ifdef _DEBUG
#	include <string>
#	define START_PROFILE(section) sec=section; time_ = glfwGetTime()
#	define END_PROFILE() print("Section %s in %f sec\n", sec.c_str(), glfwGetTime()-time_)
static double time_=0;
static std::string sec;
#else
#	define START_PROFILE(section)
#	define END_PROFILE()
#endif

class FirstPersonCamera1
{
	public:
		glm::mat4	viewMatrix;
		glm::vec3	pos;
	public:
		FirstPersonCamera1()
		{
			viewMatrix = glm::mat4(1,0,0,0,
								   0,1,0,0,
								   0,0,1,0,
								   0,0,0,1);
		}

		void set(const glm::vec3& _pos, const glm::vec3& lookAt)
		{
			//pos = _pos;
			//viewMatrix = glm::lookAtGTX(pos, lookAt, glm::vec3(0, 1, 0));
		}

		void rotateLR(float angle)
		{
			viewMatrix = glm::mat4GTX(glm::angleAxisGTX(-angle, 0.0f, 1.0f, 0.0f)) * viewMatrix;
		}
		void rotateUD(float angle)
		{
			viewMatrix = glm::mat4GTX(glm::angleAxisGTX(angle, /*1.0f, 0.0f, 0.0f)*/glm::vec3(viewMatrix[0]))) * viewMatrix;
		}
		glm::mat4 getViewMatrix()
		{
			return /*glm::inverseGTX(glm::translate3DGTX(viewMatrix, -1.0f*pos));//*/glm::inverseGTX(viewMatrix);
			//return viewMatrix;
		}
		void moveFB(float dist)
		{
			pos+=glm::vec3(viewMatrix[2])*(-dist);
		}
		void moveLR(float dist)
		{
			pos+=glm::vec3(viewMatrix[0])*(-dist);
		}
		void moveUD(float dist)
		{
			pos+=glm::vec3(viewMatrix[1])*(-dist);
		}

		//void move(glm::vec3 vec)
		//{
		//}

		void rotate(float psi, float phi)
		{
			rotateLR(psi);
			rotateUD(phi);
		}
};

//#include <DataPtr.h>

#define MESH_INDICES_MAGIC '10im'

//VertexDecl SubMesh::decl_[3]=
//{
//	{USAGE_POSITION, 3, GL_FLOAT, 0, 8*4},
//	{USAGE_NORMAL, 3, GL_FLOAT, 3*4, 8*4},
//	{USAGE_TEXCOORD0, 2, GL_FLOAT, 6*4, 8*4}
//};



//MeshPtr loadMesh1(const char* path)
//{
//	int magic;
//	MeshPtr mesh;
//	DataStreamPtr stream = vfsMapFile(path);
//	if(!stream)
//		return mesh;
//	
//	DataPtr ptr(stream->MapStream(), stream->GetSize());
//
//	ptr.read(magic);
//	
//	if(magic == MESH_INDICES_MAGIC) 
//	{
//		int num_surfaces;
//		ptr.read(num_surfaces);
//		mesh = MeshPtr(new Mesh);
//		for(int i = 0; i < num_surfaces; i++) {
//			SubMesh *s = new SubMesh();
//			
//			ptr.move(128);
//
//			ptr.read(s->numVert_);
//			s->vert_.setData(8*4*s->numVert_, ptr.move(8*4*s->numVert_));
//
//			ptr.read(s->numInd_);
//			s->numInd_*=3;
//			s->ind_.setData(s->numInd_*4, ptr.move(s->numInd_*4));
//			mesh->push_back(SubMeshPtr(s));
//		}
//	}
//	return mesh;
//}

VertexDecl decl = {USAGE_POSITION, 3, GL_FLOAT, 0, 0};

static const char* rectProgramSrc =
		"uniform vec4 uFillColor;														\n"
		"uniform vec4 uBorderColor;														\n"
		"uniform float uWidth;														\n"
		"																				\n"
		"void main()																	\n"
		"{																				\n"
		"	float dist = min(gl_TexCoord[0].x, gl_TexCoord[0].y)/uWidth;		\n"
		"	float a  = sqrt(smoothstep(0.0, 1.0, 1+dist));									\n"
		"	float b  = sqrt(smoothstep(0.0, 1.0, dist));										\n"
		"	gl_FragColor = mix(uBorderColor, uFillColor, b);							\n"
		"	gl_FragColor.a *= a;														\n"
		"}																				\n";

//NOTE: Unoptimized versions to simplify understanding
static const char gradientVertSrc[] =
		"uniform vec2	uP1;															\n"
		"uniform vec2	uP2;															\n"
		"varying float	vT;																\n"
		"																				\n"
		"void main()																	\n"
		"{																				\n"
		"	vec2 ray = uP2-uP1;															\n"
		"	float norm = dot(ray, ray);													\n"
		"	bool degenerate = norm==0.0;											\n"
		"	//Project point onto segment												\n"
		"	vT = degenerate?1.0:dot(gl_Vertex.xy-uP1, ray)/norm;						\n"
		"	gl_Position = ftransform();													\n"
		"}																				\n";

static const char gradientFragSrc[] =
		"varying float		vT;															\n"
		"uniform vec4		uColorStops[4];												\n"
		"uniform vec4		uTimeStops;													\n"
		"																				\n"
		"void main()																	\n"
		"{																				\n"
		"	float t = clamp(vT, 0.0, 1.0);												\n"
        "	vec3 delta  = uTimeStops.yzw - uTimeStops.xyz;								\n"
		"	vec3 tempMixVal = clamp((vec3(t)-uTimeStops.xyz)/delta, 0.0, 1.0);			\n"
		"	vec3 mixVal = mix(vec3(0.0), tempMixVal, notEqual(delta, vec3(0.0)));		\n"
		"	vec4 color = mix(uColorStops[0], uColorStops[1], mixVal.x);					\n"
		"	color = mix(color, uColorStops[2], mixVal.y);								\n"
		"	color = mix(color, uColorStops[3], mixVal.z);								\n"
		"	gl_FragColor = color;														\n"
		"}																				\n";

struct ShaderDef
{
	GLenum		type;
	GLsizei		srcLen;
	const char*	src;
};

static const char terrainVertSrc[] =
		"uniform vec2	uOffset;														\n"
		"uniform vec2	uScale;															\n"
		"uniform vec2	uViewPos;															\n"
		"uniform vec2	uMorphParams;													\n"
		"																				\n"
		"void main()																	\n"
		"{																				\n"
		"	vec2 pos = uOffset+uScale*gl_Vertex.xy;										\n"
		"	float d = length(pos-uViewPos);												\n"
		"	float morphK = clamp(d*uMorphParams.x+uMorphParams.y, 0.0, 1.0);			\n"
		"	vec2 dir = fract(gl_Vertex.xy*vec2(0.5))*vec2(2.0);					\n"
		"	pos += dir*vec2(morphK*uScale);"
		"	gl_Position = gl_ModelViewProjectionMatrix*vec4(pos, 0, 1);					\n"
		"	gl_FrontColor=gl_BackColor = gl_Color;"
		"}																				\n";

static const char terrainFragSrc[] =
		"void main()																	\n"
		"{																				\n"
		"	gl_FragColor = gl_Color;													\n"
		"}																				\n";


#define MAX_INFOLOG_LENGTH 256
GLuint createProgram(size_t shaderCount, ShaderDef* shaderList)
{
	GLchar	logStr[MAX_INFOLOG_LENGTH];
	GLint	status, length;

	GLuint program = glCreateProgram();

	while (shaderCount--)
	{
		resources::compileAndAttachShader(program, shaderList->type, shaderList->srcLen, shaderList->src);
		++shaderList;
	}

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (!status)
	{
		glGetProgramInfoLog(program, MAX_INFOLOG_LENGTH, &length, logStr);
		printf("%s", logStr);
		glDeleteProgram(program);

		return 0;
	}

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

	if (!status)
	{
		glGetProgramInfoLog(program, MAX_INFOLOG_LENGTH, &length, logStr);
		printf("%s", logStr);
	}
	
	return program;
}

class Exest: public UI::SDLStage
{
	private:
		//TextureManager	mTexMgr;
		//ShaderManager	mShaderMgr;
		//ProgramManager	mProgMgr;
		
		GLint timeLoc, tex1Loc, tex0Loc;
		glRenderer	renderer_;
		//TextureRef	mImage[2];
		//ProgramRef	mProg;
		//MeshPtr		mesh;
		FirstPersonCamera1	camera;
		GLuint	rectProgram;
		GLint	uniWidth, uniFillColor, uniBorderColor;

		GLuint	gradProgram;
		GLint	uniP1, uniP2, uniTimeStops, uniColorStops;
		GLuint	gradDisplayList;

		GLuint	terrainProgram;
		GLint	uniOffset, uniScale, uniViewPos, uniMorphParams;

		VFS				mVFS;

		glm::mat4	mProj;

		float vertAngle, horzAngle;

	public:
		Exest()
		{
			VFS::mount("..\\..\\AppData");
			rectProgram = resources::createProgram(GL_FRAGMENT_SHADER, rectProgramSrc);
			uniWidth = glGetUniformLocation(rectProgram, "uWidth");
			uniFillColor = glGetUniformLocation(rectProgram, "uFillColor");
			uniBorderColor = glGetUniformLocation(rectProgram, "uBorderColor");

			ShaderDef	gradProgramDef[] = {
				{GL_VERTEX_SHADER,   ARRAY_SIZE(gradientVertSrc), gradientVertSrc},
				{GL_FRAGMENT_SHADER, ARRAY_SIZE(gradientFragSrc), gradientFragSrc}
			};

			gradProgram = createProgram(ARRAY_SIZE(gradProgramDef), gradProgramDef);
			uniP1 = glGetUniformLocation(gradProgram, "uP1");
			uniP2 = glGetUniformLocation(gradProgram, "uP2");
			uniTimeStops = glGetUniformLocation(gradProgram, "uTimeStops");
			uniColorStops = glGetUniformLocation(gradProgram, "uColorStops");

			ShaderDef	terrainProgramDef[] = {
				{GL_VERTEX_SHADER,   ARRAY_SIZE(terrainVertSrc), terrainVertSrc},
				{GL_FRAGMENT_SHADER, ARRAY_SIZE(terrainFragSrc), terrainFragSrc}
			};

			//terrainProgram = createProgram(ARRAY_SIZE(terrainProgramDef), terrainProgramDef);
			terrainProgram = resources::createProgramFromFiles("Terrain.CDLOD.vert", "FF.Color.frag");
			uniOffset = glGetUniformLocation(terrainProgram, "uOffset");
			uniScale = glGetUniformLocation(terrainProgram, "uScale");
			uniViewPos = glGetUniformLocation(terrainProgram, "uViewPos");
			uniMorphParams = glGetUniformLocation(terrainProgram, "uMorphParams");

			gradDisplayList = glGenLists(1);
			glNewList(gradDisplayList, GL_COMPILE);
				glUseProgram(gradProgram);
				
				glUniform2f(uniP1, 40, 40);
				glUniform2f(uniP2, 60, 60);
				glUniform4f(uniTimeStops, 0, 0.11f, 0.5f, 1);

				float colors[4*4] = 
				{
					0, 0, 0, 1,
					1, 0, 0, 1,
					0, 1, 0, 1,
					0, 0, 1, 1,
				};

				glUniform4fv(uniColorStops, 4, colors);
			glEndList();
			GLenum err = glGetError();
			assert(err==GL_NO_ERROR);

			setupTerrainParams();

			mProj = glm::perspectiveGTX(90.0f, mWidth/mHeight, 0.1f, visibilityDistance);
			camera.set(glm::vec3(0, 20, 0), glm::vec3(0, 0, -20));

			viewPoint = glm::vec3(0, 0, 0);
			glm::mat4 lookAt = glm::lookAtGTX<float>(viewPoint, glm::vec3(0, 0, 10), glm::vec3(0, 1, 0));
			glm::mat4 proj = glm::perspectiveGTX<float>(33.0f, 1.33333333f, 0.1f, 600.0f);

			VP = proj*lookAt;
		}
		glm::vec3 viewPoint;
		glm::mat4 VP;
		
		~Exest()
		{
			glDeleteLists(gradDisplayList, 1);
			glDeleteProgram(gradProgram);
			glDeleteProgram(rectProgram);
		}

#define RGBA_RED(color)   ((color)&0xFF)
#define RGBA_GREEN(color) ((color>>8)&0xFF)
#define RGBA_BLUE(color)  ((color>>16)&0xFF)
#define RGBA_ALPHA(color) ((color>>24)&0xFF)

#define RGBA_RED_FLOAT(color)   (RGBA_RED(color)/255.0f)
#define RGBA_GREEN_FLOAT(color) (RGBA_GREEN(color)/255.0f)
#define RGBA_BLUE_FLOAT(color)  (RGBA_BLUE(color)/255.0f)
#define RGBA_ALPHA_FLOAT(color) (RGBA_ALPHA(color)/255.0f)
#define RGBA(r, g, b, a) (((a&0xFF)<<24)|((b&0xFF)<<16)|((g&0xFF)<<8)|(r&0xFF))

		size_t gridDimX;
		size_t gridDimY;
		float size;
		float startX;
		float startY;
		float endX;
		float endY;
		float visibilityDistance;
		GLint LODCount;
		float detailBalance;
		float morphZoneRatio;
		size_t minPatchDimX, minPatchDimY;

		void setupTerrainParams()
		{
			gridDimX = 257;
			gridDimY = 257;
			size = 20;
			startX = 0.5f*(mWidth  - size*(gridDimX-1));
			startY = 0.5f*(mHeight - size*(gridDimY-1));
			endX = mWidth -startX;
			endY = mHeight-startY;
			minPatchDimX = 4;
			minPatchDimY = 4;

			visibilityDistance = size*50;
			LODCount = 4;
			detailBalance = 2.0f;
			morphZoneRatio = 0.30f;

			setupLODParams();
		}
		
		struct LODDesc
		{
			float scaleX, scaleY;
			float rangeStart;
			float rangeEnd;
			float minRange;
			size_t patchDimX, patchDimY;
			float morphStart;
			float morphInvRange, morphStartRatio;
		};

		LODDesc	LODs[16];

		void setupLODParams()
		{
			float	curScale=size,
					curDetailBalance=1.0f,
					totalDetail = 0.0f,
					curRange=0.0f;

			for (GLint i=0; i<LODCount; ++i)
			{
				LODs[i].scaleX = curScale;
				LODs[i].scaleY = curScale;
				LODs[i].minRange = sqrt(LODs[i].scaleX*LODs[i].scaleX*minPatchDimX*minPatchDimX+LODs[i].scaleY*LODs[i].scaleY*minPatchDimY*minPatchDimY);

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
	
		void addPatchToQueue(size_t level, size_t i, size_t j)
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

		bool intersectViewFrustum(size_t level, size_t i, size_t j)
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

		bool intersectSphere(size_t level, size_t i, size_t j)
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
		void selectQuadsForDrawing(size_t level, size_t i, size_t j)
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

		void drawPatch(float baseX, float baseY, float scaleX, float scaleY, int level)
		{
			glUseProgram(terrainProgram);

			glColor3f(1.0f, 1.0f, 0.0f);
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

		struct PatchData
		{
			float baseX, baseY;
			float scaleX, scaleY; //TODO: Can be accessed from level???
			GLint level;
		};

		std::vector<PatchData>	patchList;

		void drawTerrain()
		{
			patchList.reserve(2048);
			patchList.clear();

			size_t level = 1;
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

		void drawRect(float x0, float y0, float x1, float y1, uint32_t fillColor, uint32_t borderColor)
		{
			float w = 0.5f;
			if (1)
			{
				glUseProgram(rectProgram);

				glUniform4f(uniBorderColor,
					RGBA_RED_FLOAT(borderColor),
					RGBA_GREEN_FLOAT(borderColor),
					RGBA_BLUE_FLOAT(borderColor),
					RGBA_ALPHA_FLOAT(borderColor));
				glUniform4f(uniFillColor,
					RGBA_RED_FLOAT(fillColor),
					RGBA_GREEN_FLOAT(fillColor),
					RGBA_BLUE_FLOAT(fillColor),
					RGBA_ALPHA_FLOAT(fillColor));
				glUniform1f(uniWidth, 2*w);
			}
			else
			{
				glCallList(gradDisplayList);
			}

			float dx = x1-x0;
			float dy = y1-y0;

			assert(dx>=0);
			assert(dy>=0);
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_TRIANGLES);
				glTexCoord2f(-w, -w);
				glVertex2f(x0-w, y0-w);
				glTexCoord2f(-w, dx+w);
				glVertex2f(x1+w, y0-w);
				glTexCoord2f(dy+w, -w);
				glVertex2f(x0-w, y1+w);

				glTexCoord2f(dy+w, -w);
				glVertex2f(x1+w, y0-w);
				glTexCoord2f(-w, -w);
				glVertex2f(x1+w, y1+w);
				glTexCoord2f(-w, dx+w);
				glVertex2f(x0-w, y1+w);
			glEnd();
			glDisable(GL_BLEND);
			glPopAttrib();
		}

		void OnCreate()
		{
			//mesh = loadMesh1("sky.mesh");

			//mImage[0].create("HorizontTex", "horizon.png");
			//mImage[0]->setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			//mImage[1].create("CloudsTex", "clouds.png");

			//mProg.create("SkyProg", "sky.vert", "sky.frag");

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			//gluPerspective(90.0, (float)mWidth/mHeight,1,2048);
			glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
			glClearDepth(1.0f);									// Depth Buffer Setup
			glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
			glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
			//timeLoc = mProg->bindUniform("time");
			//tex0Loc = mProg->bindUniform("tex0");
			//tex1Loc = mProg->bindUniform("tex1");
			//renderer_.useProgram(mProg.get());
			//renderer_.setUniform1f(timeLoc, (float)glfwGetTime());
			//renderer_.setUniform1i(tex0Loc, 0);
			//renderer_.setUniform1i(tex1Loc, 1);
			//renderer_.useProgram(0);
			//camera.rotate(180, 0);
			//camera.move(glm::vec3(0, 1, -4)); 
		}

		void drawFrustum(const glm::mat4& frustum)
		{
			glm::mat4 inverted = glm::inverseGTX(frustum);

			static const glm::vec4 points[8] = {
				glm::vec4(-1, -1, -1, 1), 
				glm::vec4( 1, -1, -1, 1), 
				glm::vec4( 1,  1, -1, 1), 
				glm::vec4(-1,  1, -1, 1), 
				glm::vec4(-1, -1,  1, 1), 
				glm::vec4( 1, -1,  1, 1), 
				glm::vec4( 1,  1,  1, 1), 
				glm::vec4(-1,  1,  1, 1), 
			};

			glm::vec4 wpoints[8];

			for (size_t i=0; i<8; ++i)
				wpoints[i] = inverted*points[i];

			glUseProgram(0);
			glColor3f(1, 0, 0);
			glBegin(GL_LINES);
				glVertex4fv(wpoints[0]); glVertex4fv(wpoints[1]);
				glVertex4fv(wpoints[1]); glVertex4fv(wpoints[2]);
				glVertex4fv(wpoints[2]); glVertex4fv(wpoints[3]);
				glVertex4fv(wpoints[3]); glVertex4fv(wpoints[0]);

				glVertex4fv(wpoints[4]); glVertex4fv(wpoints[5]);
				glVertex4fv(wpoints[5]); glVertex4fv(wpoints[6]);
				glVertex4fv(wpoints[6]); glVertex4fv(wpoints[7]);
				glVertex4fv(wpoints[7]); glVertex4fv(wpoints[4]);

				glVertex4fv(wpoints[0]); glVertex4fv(wpoints[4]);
				glVertex4fv(wpoints[1]); glVertex4fv(wpoints[5]);
				glVertex4fv(wpoints[2]); glVertex4fv(wpoints[6]);
				glVertex4fv(wpoints[3]); glVertex4fv(wpoints[7]);
			glEnd();
		}

	protected:
		void onPaint()
		{
			glClearDepth(1.0);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			//glMatrixMode(GL_MODELVIEW);
			//glLoadIdentity();
			//glLoadMatrixf(camera.getViewMatrix());
			//glTranslatef(-camera.pos[0], -camera.pos[1], -camera.pos[2]);
			//glTranslatef(0.0f, -1.0f, -4.0f);
			//renderer_.begin();
			//	glDisable(GL_DEPTH_TEST);
			//	glColor3f(1.0f, 1.0f, 1.0f);
			//	renderer_.setTexture(GL_TEXTURE0, mImage[0].get());
			//	renderer_.setTexture(GL_TEXTURE1, mImage[1].get());
			//	renderer_.useProgram(mProg.get());
			//	renderer_.setUniform1f(timeLoc, (float)glfwGetTime());
			//	glPushMatrix();
			//		//glLoadIdentity();
			//		glTranslatef(camera.pos[0], camera.pos[1], camera.pos[2]);
			//		glMultMatrixf(glm::mat4(1,0,0,0,
			//								0,0,1,0,
			//								0,1,0,0,
			//								0,0,0,1));
			//		glTranslatef(0,0,-400);
			//		renderer_.addAttribBuffer((*mesh)[0]->vert_, 1, SubMesh::decl_);
			//		renderer_.setIndexBuffer((*mesh)[0]->ind_, GL_UNSIGNED_INT);
			//		renderer_.drawPrimitives(GL_TRIANGLES, (*mesh)[0]->numInd_);
			//	glPopMatrix();
			//	renderer_.useProgram(0);
			//glEnable(GL_DEPTH_TEST);
			//glDisable(GL_TEXTURE_2D);
			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadMatrixf(mProj);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glm::mat4 view = camera.getViewMatrix();

			glLoadMatrixf(view);
			glTranslatef(-camera.pos[0], -camera.pos[1], -camera.pos[2]);
			//glLoadIdentity();

			glDisable(GL_CULL_FACE);

			glUseProgram(0);
			glBegin(GL_TRIANGLES);
				glColor3f(1, 0, 0);
				glVertex3f(-3, -1, -10);
				glColor3f(0, 1, 0);
				glVertex3f( 3, -1, -10);
				glColor3f(0, 0, 1);
				glVertex3f( 0, 4, -10);
			glEnd();

			glUseProgram(0);
			glBegin(GL_LINES);
			glColor3ub(68, 193, 181);						
			for(float i = 0; i < gridDimX; ++i)
			{
					glVertex3f(startX, 0, startY+i*size); glVertex3f(endX, 0, startY+i*size);
					glVertex3f(startX+i*size, 0, startY); glVertex3f(startX+i*size, 0, endY);
			}
			glEnd();

			drawTerrain();

			drawFrustum(VP);

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();

			//sui::drawRect(100, 100, 700, 500, sui::cBase+1, sui::cOutline);
			//sui::drawBoolFrame(0, 0, 100, 100, 16, 16, false, false, false );

			glDisable(GL_BLEND);
			//drawRect(20, 20, 80, 80, RGBA(255, 0, 0, 255), RGBA(255, 255, 255, 255));

			GLenum err = glGetError();
			assert(err==GL_NO_ERROR);
		}

		virtual void onKeyDown(const KeyEvent& event)
		{
			u32 key = event.keysym.sym;
			switch (key)
			{
				case SDLK_ESCAPE:
					close();
					break;
				case SDLK_w:
					camera.moveFB(1.1f);
					break;
				case SDLK_s:
					camera.moveFB(-1.1f);
					break;
				case SDLK_a:
					camera.moveLR(1.1f);
					break;
				case SDLK_d:
					camera.moveLR(-1.1f);
					break;
				case SDLK_KP8:
					camera.rotateUD(1.5);
					vertAngle+=0.5f;
					break;
				case SDLK_KP5:
					camera.rotateUD(-1.5);
					vertAngle-=0.5f;
					break;
				case SDLK_KP4:
					horzAngle-=0.5f;
					camera.rotateLR(-1.5);
					break;
				case SDLK_KP6:
					horzAngle+=0.5f;
					camera.rotateLR(1.5);
					break;
			}
			//static int look = 0;
			//
			//if(!look && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) 
			//{
			//	glfwDisable(GLFW_MOUSE_CURSOR);
			//	glfwSetMousePos(mWidth / 2, mHeight/ 2);
			//	look = 1;
			//}
			//
			//if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) 
			//{
			//	look = 0;
			//	glfwEnable(GLFW_MOUSE_CURSOR);
			//}

			//if(look)
			//{
			//	int mouseX=0, mouseY=0;
			//	glfwGetMousePos(&mouseX, &mouseY);
			//	float psi = (mouseX - mWidth / 2) * 0.2f;
			//	float phi = (mouseY - mHeight / 2) * 0.2f;
			//	if(phi < -89) phi = -89;
			//	if(phi > 89) phi = 89;
			//	camera.rotate(psi, -phi);
			//	//camera.rotateLR(psi);
			//	//camera.rotateUD(-phi);
			//	glfwSetMousePos(mWidth / 2, mHeight / 2);
			//} 
		}
};

int main(int argc, char** argv)
{
	ui::init();
	{
		Exest app;
		app.run();
	}
	ui::cleanup();

	return 0;
}