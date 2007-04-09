#pragma once

#include <gl\glew.h>
#include <string>
#include <cstring>

class ShaderObject
{
	friend class GPUProgram;
public:
					ShaderObject();
	virtual			~ShaderObject();

	bool			Compile(const char* Source);
	std::string		GetCompilerLog(void);

protected:
	int			ProgramType;  //!< The program type. 1=Vertex Program, 2=Fragment Program, 3=Geometry Progam, 0=none
	GLuint		ShaderObjectHandle;  //!< Shader Object
	bool		IsCompiled;   //!< true if compiled
};

//-----------------------------------------------------------------------------

//! \ingroup GLSL
class VertexShader : public ShaderObject
{
public:
	VertexShader();  //!< Constructor for Vertex Shader
};

//-----------------------------------------------------------------------------

//! \ingroup GLSL
class FragmentShader : public ShaderObject
{
public:
	FragmentShader(); //!< Constructor for Fragment Shader
};

class GPUProgram
{
public:
	GPUProgram();
	~GPUProgram();
	void AddShader(ShaderObject*	Shader);
	void Link();
	GLuint GetProgram();
	std::string GetLinkerLog();
	GLint BindVar(char* name);
	GLint BindVertexAttrib(char* name);
	void SetVar1f(GLuint var, float val);
	void SetVar1i(GLuint var, int val);
	void SetVarMatrix4fv(GLuint var, GLsizei count, GLboolean transpose, GLfloat *value);
public:
	bool IsLinked;
	GLuint	ProgramObjectHandle;
};