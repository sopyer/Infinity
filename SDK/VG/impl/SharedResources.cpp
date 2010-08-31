#include <gl\glee.h>
#include <cassert>
#include "SharedResources.h"
#include <utils.h>

namespace impl
{
	GLuint	programs[PRG_COUNT];
	GLint	uniforms[UNI_COUNT];

	namespace
	{
		//MSVC8 is unhappy without extern:)
		extern const char UNI_NAME_HALF_WIDTH[] = "uHalfWidth";
		extern const char UNI_NAME_FILL_COLOR[] = "uFillColor";
		extern const char UNI_NAME_BORDER_COLOR[] = "uBorderColor";
		extern const char UNI_NAME_ZONES[] = "uZones";

		size_t refCount = 0;
		
		enum
		{
			fsRastArc,
			fsRastQuad,
			fsRastCubic,
			fsRastCubicAA,
			vsRastStroke,
			
			vsSimpleUI,
			fsSimpleUI,

			shCount
		};

		struct ShaderDef
		{
			GLenum		mType;
			const char*	mSource;
		};

		struct ProgramDef
		{
			size_t			mShaderCount;
			const size_t*	mShaderList;
			size_t			mUniformCount;
			const char**	mUniformList;
			size_t*			mUniformID;
		};
		
		ProgramDef	programDefs[PRG_COUNT] = 
		{
			//PRG_SIMPLE_UI,
			{
				2, CArray2<size_t, vsSimpleUI, fsSimpleUI>::ptr,
				3,
				CArray3<const char*, UNI_NAME_FILL_COLOR, UNI_NAME_BORDER_COLOR, UNI_NAME_ZONES>::ptr,
				CArray3<size_t, UNI_SIMPLE_UI_FILL_COLOR, UNI_SIMPLE_UI_BORDER_COLOR, UNI_SIMPLE_UI_ZONES>::ptr
			},
			
			//PRG_RAST_FILL_CUBIC_AA,
			{
				1, CArray1<size_t, fsRastCubicAA>::ptr,
				0, 0, 0
			},

			//PRG_RAST_FILL_CUBIC,
			{
				1, CArray1<size_t, fsRastCubic>::ptr,
				0, 0, 0
			},
			//PRG_RAST_FILL_QUAD,
			{
				1, CArray1<size_t, fsRastQuad>::ptr,
				0, 0, 0
			},
			//PRG_RAST_FILL_ARC,
			{
				1, CArray1<size_t, fsRastArc>::ptr,
				0, 0, 0
			},
			//PRG_RAST_STROKE_CUBIC,
			{
				2, CArray2<size_t, vsRastStroke, fsRastCubic>::ptr,
				1,
				CArray1<const char*, UNI_NAME_HALF_WIDTH>::ptr,
				CArray1<size_t, UNI_RAST_STROKE_CUBIC_HALFWIDTH>::ptr
			},
			//PRG_RAST_STROKE_QUAD,
			{
				2, CArray2<size_t, vsRastStroke, fsRastQuad>::ptr,
				1,
				CArray1<const char*, UNI_NAME_HALF_WIDTH>::ptr,
				CArray1<size_t, UNI_RAST_STROKE_QUAD_HALFWIDTH>::ptr
			},
			//PRG_RAST_STROKE_ARC,
			{
				2, CArray2<size_t, vsRastStroke, fsRastArc>::ptr,
				1,
				CArray1<const char*, UNI_NAME_HALF_WIDTH>::ptr,
				CArray1<size_t, UNI_RAST_STROKE_ARC_HALFWIDTH>::ptr
			},
			//PRG_RAST_STROKE_TRI,
			{
				1, CArray1<size_t, vsRastStroke>::ptr,
				1,
				CArray1<const char*, UNI_NAME_HALF_WIDTH>::ptr,
				CArray1<size_t, UNI_RAST_STROKE_TRI_HALFWIDTH>::ptr
			},
		};
		
		ShaderDef shaderDefs[shCount] =
		{
			{
				GL_FRAGMENT_SHADER,
				"void main(void)											\n"
				"{															\n"
				"	vec2 uv = gl_TexCoord[0].st;							\n"
				"															\n"
				"	if( (uv.s*uv.s + uv.t*uv.t)>=1.0 )						\n"
				"		discard;											\n"
				"															\n"
				"	gl_FragColor = vec4(1.0);								\n"
				"}															\n"
			},

			{
				GL_FRAGMENT_SHADER,
				"void main(void)											\n"
				"{															\n"
				"	vec2 uv = gl_TexCoord[0].st;							\n"
				"															\n"
				"	if( (pow(uv.s, 2.0)-uv.t)>=0.0 )						\n"
				"		discard;											\n"
				"															\n"
				"	gl_FragColor = vec4(1.0);								\n"
				"}															\n"
			},

			{
				GL_FRAGMENT_SHADER,
				"void main(void)											\n"
				"{															\n"
				"	vec3 uv = gl_TexCoord[0].stp;							\n"
				"															\n"
				"	if( (uv.s*uv.s*uv.s - uv.t*uv.p)>=0.0 )					\n"
				"		discard;											\n"
				"															\n"
				"	gl_FragColor = vec4(1.0);								\n"
				"}															\n"
			},

			{
				GL_FRAGMENT_SHADER,
				"void main(void)												\n"
				"{																\n"
				"	vec3   uv = gl_TexCoord[0].xyz;								\n"
				"	vec3   dF = vec3(3*uv.x*uv.x, -uv.z, -uv.y);				\n"
				"	vec2   grad = vec2(dot(dFdx(uv), dF), dot(dFdy(uv), dF));	\n"
				"	float  F = uv.x*uv.x*uv.x - uv.y*uv.z;						\n"
				"	float  sdist = 0.5-F/length(grad);							\n"
				"	float  a = clamp(sdist, 0.0, 1.0);							\n"
				"																\n"
				"	if( a==0.0 )												\n"
				"		discard;												\n"
				"																\n"
				"	gl_FragColor = vec4(1.0, 1.0, 1.0, a);						\n"
				"}																\n"
			},

			{
				GL_VERTEX_SHADER,
				"uniform float uHalfWidth;									\n"
				"															\n"
				"void main()												\n"
				"{															\n"
				"	vec4 pos = gl_Vertex;									\n"
				"	pos.xy += uHalfWidth*gl_MultiTexCoord1.xy;				\n"
				"	gl_Position = gl_ModelViewProjectionMatrix*pos;			\n"
				"	gl_TexCoord[0] = gl_MultiTexCoord0;						\n"
				"}															\n"
			},

			{
				GL_VERTEX_SHADER,
				"void main()																	\n"
				"{																				\n"
				"	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;						\n"
				"	gl_TexCoord[0] = gl_MultiTexCoord0;											\n"
				"}																				\n"
			},

			{
				GL_FRAGMENT_SHADER,
				"uniform vec4 uFillColor;														\n"
				"uniform vec4 uBorderColor;														\n"
				"uniform vec2 uZones;															\n"
				"																				\n"
				"void main()																	\n"
				"{																				\n"
				"	float doTurn       = float(gl_TexCoord[0].y>0);								\n"
				"	float radiusOffset = doTurn*abs(gl_TexCoord[0].z);							\n"
				"	float turnDir      = sign(gl_TexCoord[0].z);								\n"
				"	vec2  uv = vec2(gl_TexCoord[0].x+turnDir*radiusOffset, gl_TexCoord[0].y);	\n"
				"	float l  = abs(length(uv) - radiusOffset);									\n"
				"	float a  = clamp(l - uZones.x, 0.0, 2.0);									\n"
				"	float b  = clamp(l - uZones.y, 0.0, 2.0);									\n"
				"	b = exp2(-2.0*b*b);															\n"
				"	gl_FragColor = (uFillColor*b + (1.0-b)*uBorderColor);						\n"
				"	gl_FragColor.a *= exp2(-2.0*a*a);											\n"
				"}																				\n"
			},
		};

		//const char* const sourceColorFillFragSh =
		//	"uniform vec4 uFillColor;									\n"
		//	"															\n"
		//	"void main()												\n"
		//	"{															\n"
		//	"	gl_FragColor = uFillColor;								\n"
		//	"}															\n";

		//const char* const sourceFillVertSh =
		//	"uniform vec2 uImageDim;									\n"
		//	"															\n"
		//	"void main()												\n"
		//	"{															\n"
		//	"	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;	\n"
		//	"	gl_TexCoord[0] = vec4(gl_Vertex.xy/uImageDim, 0, 0);	\n"
		//	"}															\n";

		//const char* const sourcePatternFillFragSh =
		//	"uniform sampler2D uPattern;								\n"
		//	"															\n"
		//	"void main()												\n"
		//	"{															\n"
		//	"	gl_FragColor = tex2D(uPattern, gl_TexCoord[0].st);		\n"
		//	"}															\n";

		void CompileShader(GLuint shader, const char* source)
		{
			GLint len = (GLint)strlen(source);
			glShaderSource(shader, 1, (const GLchar **)&source, &len);
			glCompileShader(shader); 
		}
	}


	void acquire()
	{
		assert(refCount>=0);
		if (refCount==0)
		{
			GLuint	shaders[shCount];

			for (size_t i=0; i<shCount; ++i)
			{
				shaders[i] = glCreateShader(shaderDefs[i].mType);
				GLint len = (GLint)strlen(shaderDefs[i].mSource);
				glShaderSource(shaders[i], 1, (const GLchar **)&shaderDefs[i].mSource, &len);
				glCompileShader(shaders[i]); 
			}

			for (size_t i=0; i<PRG_COUNT; ++i)
			{
				programs[i] = glCreateProgram();
				
				for (size_t sh=0; sh<programDefs[i].mShaderCount; ++sh)
				{
					glAttachShader(programs[i], shaders[programDefs[i].mShaderList[sh]]);
				}
				
				glLinkProgram(programs[i]);

				for (size_t u=0; u<programDefs[i].mUniformCount; ++u)
				{
					uniforms[programDefs[i].mUniformID[u]] = glGetUniformLocation(programs[i], programDefs[i].mUniformList[u]);
				}
			}

			//prgFillColor = glCreateProgram();
			//fsColor = glCreateShader(GL_FRAGMENT_SHADER);
			//CompileShader(fsColor, sourceColorFillFragSh);
			//glAttachShader(prgFillColor, fsColor);
			//glLinkProgram(prgFillColor);
			//glDeleteShader(fsColor);

			//prgFillPattern = glCreateProgram();
			//vsFill = glCreateShader(GL_VERTEX_SHADER);
			//fsPattern = glCreateShader(GL_FRAGMENT_SHADER);
			//CompileShader(vsFill, sourceFillVertSh);
			//CompileShader(fsPattern, sourcePatternFillFragSh);
			//glAttachShader(prgFillPattern, fsPattern);
			//glAttachShader(prgFillPattern, vsFill);
			//glLinkProgram(prgFillPattern);
			//glDeleteShader(fsPattern);
			//glDeleteShader(vsFill);

			for (size_t i=0; i<shCount; ++i)
			{
				glDeleteShader(shaders[i]);
			}
		}
		refCount++;
	}

	void release()
	{
		assert(refCount>0);
		refCount--;
		if (refCount==0)
		{
			//glDeleteProgram(prgFillColor);
			//glDeleteProgram(prgFillPattern);

			for (size_t i=0; i<PRG_COUNT; ++i)
			{
				glDeleteProgram(programs[i]);
			}
		}
	}
}
