#ifndef _GLCLASSES_H_INCLUDED_
#	define _GLCLASSES_H_INCLUDED_

#	ifndef GLEW_STATIC
#		define GLEW_STATIC
#	endif
#	include <gl\glee.h>
#	include <cstring>

namespace gl
{
#	include <Common.h>
#	include <glCommandList.h>
#	include <glTexture.h>
#	include <glBuffer.h>
#	include <glShader.h>
#	include <glContext.h>
#	include	<UniformTable.h>
}
typedef gl::Shader			glShaderBase;
typedef gl::Context			glRenderer;
typedef	gl::Texture			glTexture;
typedef	gl::Texture2D		glTexture2D;
typedef	gl::Program			glProgram;
typedef	gl::CommandList		glDisplayList;
typedef gl::VertexShader	glVertexShader;
typedef gl::FragmentShader	glFragmentShader;
typedef	gl::AttribBuffer	glAttribBuffer;
typedef	gl::IndexBuffer		glIndexBuffer;

typedef	gl::VertexDeclaration	VertexDecl;
using	gl::USAGE_POSITION;
using	gl::USAGE_COLOR;
using	gl::USAGE_SECONDARYCOLOR;
using	gl::USAGE_NORMAL;
using	gl::USAGE_TEXCOORD0;
using	gl::USAGE_TEXCOORD1;
using	gl::USAGE_TEXCOORD2;
using	gl::USAGE_TEXCOORD3;
using	gl::USAGE_TEXCOORD4;
using	gl::USAGE_TEXCOORD5;
using	gl::USAGE_TEXCOORD6;
using	gl::USAGE_TEXCOORD7;
using	gl::USAGE_ATTRIB_BASE;

#endif