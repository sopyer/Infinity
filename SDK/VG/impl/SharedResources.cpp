#include <cassert>
#include <stdarg.h>

#include <opengl.h>

#include "SharedResources.h"

namespace impl
{
	int initialized;
	
	enum
	{
		fsStencilArc,
		fsStencilQuad,
		fsStencilCubic,
		fsStencilCubicAA,
		vsStencilStroke,
		
		vsSimpleUI,
		fsSimpleUI,

		vsCover,
		fsGradientCommon,
		fsLinearGradient,

		shCount
	};

	struct ShaderDef
	{
		GLenum		type;
		const char*	source;
	} shaderDefs[shCount] =
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

		{
			GL_VERTEX_SHADER,
			"varying vec2 vFragPos;															\n"
			"																				\n"
			"void main()																	\n"
			"{																				\n"
			"	gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xy, 0, 1);		\n"
			"	vFragPos    = gl_Vertex.xy;													\n"
			"}																				\n"
		},

		{
			GL_FRAGMENT_SHADER,																			
			"#version 130																					\n"
			"																								\n"
			"uniform vec4		uStops[8];																	\n"
			"uniform vec4		uScales[8];																	\n"
			"uniform float		uInvStopCount;																\n"
			"uniform sampler1D	samColorRamp;																\n"
			"																								\n"
			"vec4 evalGrad(float t)																			\n"
			"{																								\n"
			"	vec4 accum = vec4(0);																		\n"
			"																								\n"
			"	for (int i=0; i<8; ++i)																		\n"
			"	{																							\n"
			"		vec4 deltas;																			\n"
			"																								\n"
			"		// can not use multiply-add, because {0, 0.5, 0.5, 1} case would not work 				\n"
			"		// also this case requires support of IEEE 754 floats as well							\n"
			"		// because uScales[1] is infinity in this case											\n"
			"		deltas = (vec4(t)-uStops[i])*uScales[i];												\n"
			"		deltas = clamp(deltas, vec4(0), vec4(1));												\n"
			"		accum += deltas;																		\n"
			"	}																							\n"
			"																								\n"
			"	float offset =(dot(accum, vec4(1))+0.5)*uInvStopCount;										\n"
			"																								\n"
			"	return texture(samColorRamp, offset);														\n"
			"}																								\n"
		},

		{
			GL_FRAGMENT_SHADER,																			
			"#version 130																					\n"
			"																								\n"
			"vec4 evalGrad(float t);																		\n"
			"																								\n"
			"//Mapping distance between uStartPt and uEndPt in range [0..1]									\n"
			"//uDirection  = (uEndPt-uStartPt);																\n"
			"//uScale      = 1.0/dot(uDirection, uDirection);												\n"
			"//uDirection *= uScale;																		\n"
			"uniform vec2 uStartPt;																			\n"
			"uniform vec2 uDirection;																		\n"
			"																								\n"
			"varying vec2 vFragPos;																			\n"
			"																								\n"
			"void main()																					\n"
			"{																								\n"
			"	float t = dot(uDirection, vFragPos-uStartPt);												\n"
			"	gl_FragColor = evalGrad(t);																	\n"
			"}																								\n"
		},
	};

	const char* UNI_NAME_HALF_WIDTH       = "uHalfWidth";
	const char* UNI_NAME_FILL_COLOR       = "uFillColor";
	const char* UNI_NAME_BORDER_COLOR     = "uBorderColor";
	const char* UNI_NAME_ZONES            = "uZones";
	const char* UNI_NAME_STOPS            = "uStops";
	const char* UNI_NAME_SCALES           = "uScales";
	const char* UNI_NAME_INV_STOP_COUNT   = "uInvStopCount";
	const char* UNI_NAME_SAM_COLOR_RAMP   = "samColorRamp";
	const char* UNI_NAME_START_POINT      = "uStartPt";
	const char* UNI_NAME_DIRECTION        = "uDirection";

	const char* simpleUIUniformNames[UNI_SIMPLE_UI_COUNT] = 
	{
		UNI_NAME_FILL_COLOR,
		UNI_NAME_BORDER_COLOR,
		UNI_NAME_ZONES,
	};

	GLuint	simpleUIProgram;
	GLint	simpleUIUniforms[UNI_SIMPLE_UI_COUNT];

	GLuint	stencilCubicAreaProgram;
	GLuint	stencilCubicAreaAAProgram;
	GLuint	stencilQuadAreaProgram;
	GLuint	stencilArcAreaProgram;

	const char* linGradUniformNames[UNI_LIN_GRAD_COUNT] = 
	{
		UNI_NAME_STOPS,
		UNI_NAME_SCALES,
		UNI_NAME_INV_STOP_COUNT,
		UNI_NAME_SAM_COLOR_RAMP,
		UNI_NAME_START_POINT,
		UNI_NAME_DIRECTION,
	};

	GLuint	linGradProgram;
	GLint	linGradUniforms[UNI_LIN_GRAD_COUNT];

	void	compileShader(GLuint shader, const char* source);
	GLuint	linkProgram(GLsizei shaderCount, ...);
	void	getUniforms(GLuint program, GLsizei uniformCount, const char** uniformNames, GLint* uniforms);

	void allocResources()
	{
		if (!initialized)
		{
			GLuint	shaders[shCount];

			for (size_t i=0; i<shCount; ++i)
			{
				shaders[i] = glCreateShader(shaderDefs[i].type);
				GLint len = (GLint)strlen(shaderDefs[i].source);
				glShaderSource(shaders[i], 1, (const GLchar **)&shaderDefs[i].source, &len);
				glCompileShader(shaders[i]);
			}

			simpleUIProgram = linkProgram(2, shaders[vsSimpleUI], shaders[fsSimpleUI]);
			getUniforms(simpleUIProgram, UNI_SIMPLE_UI_COUNT, simpleUIUniformNames, simpleUIUniforms);

			stencilCubicAreaProgram   = linkProgram(1, shaders[fsStencilCubic]);
			stencilCubicAreaAAProgram = linkProgram(1, shaders[fsStencilCubicAA]);
			stencilQuadAreaProgram    = linkProgram(1, shaders[fsStencilQuad]);
			stencilArcAreaProgram     = linkProgram(1, shaders[fsStencilArc]);
			
			linGradProgram = linkProgram(3, shaders[vsCover], shaders[fsGradientCommon], shaders[fsLinearGradient]);
			getUniforms(linGradProgram, UNI_LIN_GRAD_COUNT, linGradUniformNames, linGradUniforms);

			for (size_t i=0; i<shCount; ++i)
			{
				glDeleteShader(shaders[i]);
			}
		}
		initialized = true;
	}

	void freeResources()
	{
		if (initialized)
		{
			glDeleteProgram(simpleUIProgram);
			glDeleteProgram(stencilCubicAreaProgram);
			glDeleteProgram(stencilCubicAreaAAProgram);
			glDeleteProgram(stencilQuadAreaProgram);
			glDeleteProgram(stencilArcAreaProgram);
			glDeleteProgram(linGradProgram);
		}

		initialized = false;
	}

	void compileShader(GLuint shader, const char* source)
	{
		GLint len = (GLint)strlen(source);
		glShaderSource(shader, 1, (const GLchar **)&source, &len);
		glCompileShader(shader); 

#ifdef _DEBUG
		GLint	status;
		char output[8096];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(shader, 8096, &status, output);
		assert(status);
#endif
	}

	GLuint linkProgram(GLsizei shaderCount, ...)
	{
		GLuint program;
		va_list arg;

		va_start(arg, shaderCount);
		program = glCreateProgram();

		while (shaderCount--)
		{
			GLuint shader = va_arg(arg, GLuint);
			glAttachShader(program, shader);
		}

		glLinkProgram(program);

#ifdef _DEBUG
		GLint	status;
		char output[8096];
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		glGetProgramInfoLog(program, 8096, &status, output);
		assert(status);
#endif
		va_end(arg);

		return program;
	}

	void getUniforms(GLuint program, GLsizei uniformCount, const char** uniformNames, GLint* uniforms)
	{
		for (GLsizei u=0; u<uniformCount; ++u)
		{
			uniforms[u] = glGetUniformLocation(program, uniformNames[u]);
			assert(uniforms[u]!=-1);
		}
	}

}
