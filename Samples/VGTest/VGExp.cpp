#include "VGExp.h"

#include <cassert>
#include <gl/glee.h>

GLuint	cubicProgram, cubicProgramAA;

void clearStencil()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glBegin(GL_QUADS);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0,	1.0);
	glVertex2f(1.0, 1.0);
	glVertex2f(1.0, 0.0);
	glEnd();
	glPopAttrib();
}

void drawQuad()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor3f(1, 1, 1);//glCallList(paint.mHandle);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glUseProgram(0);
	glBegin(GL_QUADS);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0,	1.0);
	glVertex2f(1.0, 1.0);
	glVertex2f(1.0, 0.0);
	glEnd();
	glPopAttrib();
}

void rasterizeEvenOdd(Geometry<glm::vec2>& geom)
{
	if (geom.vertices.empty())
		return;

	glm::vec2& vtx = geom.vertices[0];
		
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
		
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 1);
	glStencilMask(0x01);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), &vtx);
		
	glUseProgram(0);
	glDrawElements(GL_TRIANGLES, (GLsizei)geom.indices.size(), GL_UNSIGNED_SHORT, &geom.indices[0]);

	glPopClientAttrib();
	glPopAttrib();
}

void rasterizeEvenOdd(Geometry<CubicVertex>& geom)
{
	if (geom.vertices.empty())
		return;

	CubicVertex& vtx = geom.vertices[0];
		
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
		
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 1);
	glStencilMask(0x01);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(CubicVertex), &vtx.pos);
		
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(4, GL_FLOAT, sizeof(CubicVertex), &vtx.klmn);

	glUseProgram(cubicProgram);
	glDrawElements(GL_TRIANGLES, (GLsizei)geom.indices.size(), GL_UNSIGNED_SHORT, &geom.indices[0]+6);

	glPopClientAttrib();
	glPopAttrib();
}

void drawCubicAA(Geometry<CubicVertex>& geom)
{
	if (geom.vertices.empty())
		return;

	CubicVertex& vtx = geom.vertices[0];
		
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(CubicVertex), &vtx.pos);
		
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(4, GL_FLOAT, sizeof(CubicVertex), &vtx.klmn);

	glUseProgram(cubicProgramAA);
	glDrawElements(GL_TRIANGLES, (GLsizei)geom.indices.size(), GL_UNSIGNED_SHORT, &geom.indices[0]);

	glPopClientAttrib();
	glPopAttrib();
}

void addCubic(Geometry<CubicVertex>& cubics, Geometry<glm::vec2>& tri, const Array<glm::vec2>& v)
{
	assert(v.size()==4);

	Array<glm::vec2>	pos;
	Array<glm::vec3>	tc;

	cubicTriVertices(v, pos, tc);

	assert(pos.size()==tc.size());
	assert(tc.size()%4==0);
				
	VGuint	pointCount = pos.size();
	VGuint	begin = cubics.vertices.size(), end=begin+pointCount;

	cubics.vertices.resize(end);

	for (size_t i=0; i<pos.size(); ++i)
	{
		cubics.vertices[begin+i].pos = pos[i];
		cubics.vertices[begin+i].klmn = glm::vec4(tc[i], 0.0f);
	}

	for (VGuint i0=begin; i0<end; i0+=4)
	{
		cubics.indices.pushBack(i0+0); cubics.indices.pushBack(i0+1); cubics.indices.pushBack(i0+2);
		cubics.indices.pushBack(i0+2); cubics.indices.pushBack(i0+3); cubics.indices.pushBack(i0+0);
	}

	if (pointCount>4)
	{
		begin = tri.vertices.size(); end = begin+pointCount/4+1;

		tri.vertices.resize(end);
		for (size_t i=0; i<pos.size(); i+=4)
		{
			tri.vertices[begin+i/4] = pos[i];
		}
		tri.vertices[end-1] = pos[pointCount-1];

		for (VGuint i0=begin; i0<end-2; ++i0)
		{
			tri.indices.pushBack(i0); tri.indices.pushBack(i0+1); tri.indices.pushBack(i0+2);
		}
	}
}

const char cubicFSSource[] = 
				"#version 120												\n"
				"															\n"
				"void main(void)											\n"
				"{															\n"
				"	vec3 uv = gl_TexCoord[0].stp;							\n"
				"															\n"
				"	if( (uv.s*uv.s*uv.s - uv.t*uv.p)>=0.0 )					\n"
				"		discard;											\n"
				"															\n"
				"	gl_FragColor = vec4(1.0);								\n"
				"}															\n";

const char cubicAAFSSource[] = 
				"void main(void)												\n"
				"{																\n"
				"	vec3   uv = gl_TexCoord[0].stp;								\n"
				"	vec3   dF = vec3(3*uv.x*uv.x, -uv.y, -uv.z);				\n"
				"	vec2   grad = vec2(dot(dFdx(uv), dF), dot(dFdy(uv), dF));	\n"
				"	float  F = uv.x*uv.x*uv.x - uv.y*uv.z;						\n"
				"	float  sdist = 0.5-F/length(grad);							\n"
				"	float  a = clamp(sdist, 0.0, 1.0);							\n"
				"																\n"
				"	if( a==0.0 )												\n"
				"		discard;												\n"
				"																\n"
				"	gl_FragColor = vec4(1.0, 1.0, 1.0, a);						\n"
				"}																\n";

#define MAX_INFOLOG_LENGTH 256
bool compileAndAttachShader(GLuint program, GLenum type, GLsizei len, const char* source)
{
	GLint	status, length;
	char	logStr[MAX_INFOLOG_LENGTH];
	
	GLuint	shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, &len);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		glGetShaderInfoLog(shader, 256, &length, logStr);
		printf("%s", logStr);
		return false;
	}

	glAttachShader(program, shader);
	glDeleteShader(shader);

	return true;
}

GLuint createProgram(GLenum type, const char* source)
{
	GLchar	logStr[MAX_INFOLOG_LENGTH];
	GLint	status, length;

	GLuint program = glCreateProgram();

	compileAndAttachShader(program, type, strlen(source), source);

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

void initVGExp()
{
	cubicProgram = createProgram(GL_FRAGMENT_SHADER, cubicFSSource);
	cubicProgramAA = createProgram(GL_FRAGMENT_SHADER, cubicAAFSSource);
}

void terminateVGExp()
{
	glDeleteProgram(cubicProgramAA);
	glDeleteProgram(cubicProgram);
}