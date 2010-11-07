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
		}

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

		float resX;
		float resY;
		float size;
		float startX;
		float startY;
		float endX;
		float endY;
		float visibilityDistance;
		GLint LODCount;
		float detailBalance;
		float morphZoneRatio;
		float gridSizeX, gridSizeY;

		void setupTerrainParams()
		{
			resX = 257;
			resY = 257;
			size = 20;
			startX = 0.5f*(mWidth  - size*(resX-1));
			startY = 0.5f*(mHeight - size*(resY-1));
			endX = mWidth -startX;
			endY = mHeight-startY;
			gridSizeX = 4.0f;
			gridSizeY = 4.0f;

			visibilityDistance = size*50;
			LODCount = 4;
			detailBalance = 2.0f;
			morphZoneRatio = 0.30f;

			setupLODParams();
		}
		
		struct LODDesc
		{
			float scaleX, scaleY;
			float rangeEnd;
			float range;
			float minRange;
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
				LODs[i].minRange = sqrt(LODs[i].scaleX*LODs[i].scaleX*gridSizeX*gridSizeX+LODs[i].scaleY*LODs[i].scaleY*gridSizeY*gridSizeY);

				curScale *= 2;
				totalDetail += curDetailBalance;
				curDetailBalance *= detailBalance;
			}

			float minDetailDist = visibilityDistance/totalDetail;
			curDetailBalance = 1.0f;

			for (GLint i=0; i<LODCount; ++i)
			{
				LODs[i].range = std::max(minDetailDist*curDetailBalance, LODs[i].minRange);
				curRange += LODs[i].range;
				LODs[i].rangeEnd = curRange;
				float morphRange = LODs[i].range*morphZoneRatio;
				LODs[i].morphStart = LODs[i].rangeEnd-morphRange;
				LODs[i].morphInvRange = 1.0f/morphRange;
				LODs[i].morphStartRatio = -LODs[i].morphStart/morphRange;

				curDetailBalance *= 2.0f;
			}
		}

		void drawPatch(float baseX, float baseY, float scaleX, float scaleY, int level)
		{
			glUseProgram(terrainProgram);

			glColor3f(1.0f, 1.0f, 0.0f);
			glUniform2f(uniOffset, baseX, baseY);
			glUniform2f(uniViewPos, startX+128*size, startY+128*size);
			glUniform2f(uniScale, LODs[level].scaleX, LODs[level].scaleY);
			
			//Implement in proper way - acces to this params on per level basis
			float startMorph = 2.0f*size;
			float endMorph = 4.0f*size;
			glUniform2f(uniMorphParams, LODs[level].morphInvRange, LODs[level].morphStartRatio);

			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_TRIANGLES);
				for (float y=0; y<gridSizeY; y+=1.0f)
				{
					for (float x=0; x<gridSizeX; x+=1.0f)
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
			patchList.resize(16);

			auto	it  = patchList.begin(),
					end = patchList.end();

			for (int i=0; i<4; ++i)
				for (int j=0; j<4; ++j)
				{
					float scale = size;
					float baseX = 128.0f+(i-2)*4.0f;
					float baseY = 128.0f+(j-2)*4.0f;
					it->baseX = startX+baseX*scale;
					it->baseY = startY+baseY*scale;
					it->scaleX = scale;
					it->scaleY = scale;
					it->level = 0;
					++it;
				}

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
			glUseProgram(0);
			//glEnable(GL_DEPTH_TEST);
			//glDisable(GL_TEXTURE_2D);
			for(float i = 0; i <= 257; ++i)
			{
				glBegin(GL_LINES);
					glColor3ub(68, 193, 181);						
					glVertex3f(startX, startY+i*size, 0); glVertex3f(endX, startY+i*size, 0);
					glVertex3f(startX+i*size, startY, 0); glVertex3f(startX+i*size, endY, 0);
				glEnd();
			}

			drawTerrain();
			//sui::drawRect(100, 100, 700, 500, sui::cBase+1, sui::cOutline);
			//sui::drawBoolFrame(0, 0, 100, 100, 16, 16, false, false, false );

			glDisable(GL_BLEND);
			//drawRect(20, 20, 80, 80, RGBA(255, 0, 0, 255), RGBA(255, 255, 255, 255));

			GLenum err = glGetError();
			assert(err==GL_NO_ERROR);
				//const float ww=1;
				//glBegin(GL_LINES);
				//	glColor3f(1,0,0);
				//	glVertex3f(0,0,0);
				//	glVertex3f(ww,0,0);

				//	glColor3f(0,1,0);
				//	glVertex3f(0,0,0);
				//	glVertex3f(0,ww,0);

				//	glColor3f(0,0,1);
				//	glVertex3f(0,0,0);
				//	glVertex3f(0,0,ww);
				//glEnd();
			//renderer_.end();
			glFlush();
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
					camera.moveFB(0.1f);
					break;
				case SDLK_s:
					camera.moveFB(-0.1f);
					break;
				case SDLK_a:
					camera.moveLR(0.1f);
					break;
				case SDLK_d:
					camera.moveLR(-0.1f);
					break;
				case SDLK_KP8:
					camera.rotateUD(0.5);
					break;
				case SDLK_KP2:
					camera.rotateUD(-0.5);
					break;
				case SDLK_KP4:
					camera.rotateLR(-0.5);
					break;
				case SDLK_KP6:
					camera.rotateLR(0.5);
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