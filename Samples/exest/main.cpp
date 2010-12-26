#define GLEW_STATIC
#include <framework.h>
#include <glm\glmext.h>
#include <cassert>
#include <ResourceHelpers.h>
//#include <scheduler.h>

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

#include "CDLODTerrain.h"

class Exest: public UI::SDLStage
{
	private:
		FirstPersonCamera1	camera;
		GLuint	rectProgram;
		GLint	uniWidth, uniFillColor, uniBorderColor;

		GLuint	gradProgram;
		GLint	uniP1, uniP2, uniTimeStops, uniColorStops;
		GLuint	gradDisplayList;

		VFS				mVFS;

		glm::mat4	mProj, VP;

		float vertAngle, horzAngle;
		CDLODTerrain terrain;

	public:
		Exest(): fixedMode(false)
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

			terrain.initialize();

			mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 1000.0f);
			camera.rotateLR(180);

			terrain.viewPoint = glm::vec3(0, 0, 0);
			glm::mat4 lookAt = glm::lookAtGTX<float>(terrain.viewPoint, glm::vec3(0, 0, 10), glm::vec3(0, 1, 0));
			glm::mat4 proj = glm::perspectiveGTX<float>(33.0f, 1.33333333f, 0.1f, 1200.0f);

			VP = proj*lookAt;

			terrain.setViewProj(VP);

			mt::addTimedTask<Exest, &Exest::handleInput>(this, 20);

			File	src = VFS::openRead("heightmap_small.tga");
			//explicit conversion to avoid warning on 32-bit system
			assert(src.size()<SIZE_MAX);
			size_t fileSize = (size_t)src.size();

			unsigned char* data = new unsigned char[fileSize+1];
			src.read(data, fileSize, 1);

			int	imgWidth, imgHeight, imgChannels;
			unsigned char*	pixelsPtr = SOIL_load_image_from_memory(data, fileSize,
				&imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_L);
			assert(imgChannels==1);
			terrain.setHeightmap(pixelsPtr, imgWidth, imgHeight);

			SOIL_free_image_data(pixelsPtr);
			delete [] data;

			memset(prevKeystate, 0, SDLK_LAST);
		}
		
		~Exest()
		{
			terrain.cleanup();
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
			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadMatrixf(mProj);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glm::mat4 view = camera.getViewMatrix();

			glLoadMatrixf(view);
			glm::mat4 xm;
			glGetFloatv(GL_MODELVIEW_MATRIX, xm);
			glTranslatef(-camera.pos[0], -camera.pos[1], -camera.pos[2]);

			glGetFloatv(GL_MODELVIEW_MATRIX, xm);

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
			//for(float i = 0; i < terrain.gridDimX; ++i)
			//{
			//		glVertex3f(terrain.startX, 0, terrain.startY+i*terrain.size); glVertex3f(terrain.endX, 0, terrain.startY+i*terrain.size);
			//		glVertex3f(terrain.startX+i*terrain.size, 0, terrain.startY); glVertex3f(terrain.startX+i*terrain.size, 0, terrain.endY);
			//}
			glEnd();

			if (!fixedMode)
			{
				terrain.viewPoint = camera.pos;
				terrain.setViewProj(glm::translate3DGTX(mProj*camera.getViewMatrix(), -camera.pos));
			}
			else
			{
				terrain.viewPoint = VPpp;
				terrain.setViewProj(VP);
				drawFrustum(VP);
			}

			terrain.drawTerrain();

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

		bool fixedMode;
		glm::vec3 VPpp;
		uint8_t prevKeystate[SDLK_LAST];

		void handleInput()
		{
			uint8_t *keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_ESCAPE])
				close();
			if (keystate[SDLK_w])
				camera.moveFB(keystate[SDLK_LSHIFT]?5.1f:0.5f);
			if (keystate[SDLK_s])
				camera.moveFB(keystate[SDLK_LSHIFT]?-5.1f:-0.5f);
			if (keystate[SDLK_a])
				camera.moveLR(keystate[SDLK_LSHIFT]?5.1f:0.5f);
			if (keystate[SDLK_d])
				camera.moveLR(keystate[SDLK_LSHIFT]?-5.1f:-0.5f);
			if (keystate[SDLK_KP8])
				camera.rotateUD(1.5);
			if (keystate[SDLK_KP5])
				camera.rotateUD(-1.5);
			if (keystate[SDLK_KP4])
				camera.rotateLR(-1.5);
			if (keystate[SDLK_KP6])
				camera.rotateLR(1.5);

			bool lockView = false;
			if (keystate[SDLK_l]&&!prevKeystate[SDLK_l])
			{
				fixedMode = !fixedMode;
				lockView = fixedMode==true;
			}

			if (lockView)
			{
				printf("%d\n", fixedMode);
				VP = glm::translate3DGTX(mProj*camera.getViewMatrix(), -camera.pos);
				VPpp = camera.pos;
			}

			memcpy(prevKeystate, keystate, SDLK_LAST);
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