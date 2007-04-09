#include "pipeline.h"

/********************************************************************
(c) 2003-2006 by Martin Christen. All Rights reserved.
*********************************************************************/

using namespace std;

//-----------------------------------------------------------------------------
// Error, Warning and Info Strings
char* aGLSLStrings[] = {
        "[e00] GLSL is not available!",
        "[e01] Not a valid program object!",
        "[e02] Not a valid object!",
        "[e03] Out of memory!",
        "[e04] Unknown compiler error!",
        "[e05] Linker log is not available!",
        "[e06] Compiler log is not available!",
        "[Empty]"
        };

ShaderObject::ShaderObject()
{
	//InitOpenGLExtensions();
	IsCompiled = false;
	ProgramType = 0;
	ShaderObjectHandle = 0;
}

//----------------------------------------------------------------------------- 
ShaderObject::~ShaderObject()
{
	glDeleteShader(ShaderObjectHandle);
}

std::string ShaderObject::GetCompilerLog(void)
{    
    int infologLength = 0;
    char infoLog[1024];
 
	glGetShaderInfoLog(ShaderObjectHandle, 1024, &infologLength, infoLog);
	 
	if (infologLength > 0)
		return infoLog;
	return "";
}

// ----------------------------------------------------------------------------
bool ShaderObject::Compile(const char* Source)
{
	GLint compiled = 0;

	IsCompiled = false;
	GLint length = (GLint)strlen(Source);
	glShaderSource(ShaderObjectHandle, 1, (const GLchar **)&Source, &length);
	glCompileShader(ShaderObjectHandle); 
	glGetObjectParameterivARB(ShaderObjectHandle, GL_COMPILE_STATUS, &compiled);

	if (compiled) IsCompiled=true;

	return IsCompiled;
}

// ----------------------------------------------------------------------------
VertexShader::VertexShader()
{
	ProgramType = 1; 
	ShaderObjectHandle = glCreateShader(GL_VERTEX_SHADER);
}

// ----------------------------------------------------

FragmentShader::FragmentShader()
{
	ProgramType = 2;
	ShaderObjectHandle = glCreateShader(GL_FRAGMENT_SHADER); 
}

// ----------------------------------------------------

GPUProgram::GPUProgram()
{
	IsLinked=false;
	ProgramObjectHandle = glCreateProgram();
}

GPUProgram::~GPUProgram()
{
	glDeleteProgram(ProgramObjectHandle);
}

void GPUProgram::AddShader(ShaderObject*	Shader)
{
	if(!IsLinked)
	{
		glAttachShader(ProgramObjectHandle, Shader->ShaderObjectHandle);
	}
}

void GPUProgram::Link()
{
	IsLinked=true;
	glLinkProgram(ProgramObjectHandle);
}

std::string GPUProgram::GetLinkerLog()
{
    int infologLength = 0;
    char infoLog[1024];
 
	glGetProgramInfoLog(ProgramObjectHandle, 1024, &infologLength, infoLog);
	 
	if (infologLength > 0)
		return infoLog;
	return "";
}
GLuint GPUProgram::GetProgram()
{
	return ProgramObjectHandle;
}

GLint GPUProgram::BindVar(char* name)
{
	return glGetUniformLocation(ProgramObjectHandle, name);
}

void GPUProgram::SetVar1f(GLuint var, float val)
{
	glUniform1f(var, val);
}

void GPUProgram::SetVar1i(GLuint var, int val)
{
	glUniform1i(var, val);
}

void GPUProgram::SetVarMatrix4fv(GLuint var, GLsizei count, GLboolean transpose, GLfloat *value)
{
    glUniformMatrix4fv(var, count, transpose, value);
}

GLint GPUProgram::BindVertexAttrib(char* name)
{
	return glGetAttribLocation(ProgramObjectHandle, name);
}
