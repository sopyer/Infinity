#include "Framework.h"
#include <cstring>

static char string[1024];
int len;

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

template<typename Type>
glShaderBase* loadShader(const byte* text)
{
	Type*	shader = new Type();
	if (shader)
	{
		START_PROFILE("compileShader");
		shader->compile((GLchar*)text);
		END_PROFILE();
		START_PROFILE("infoShader");
		shader->getLog(1023, &len, string);	print(string);
		END_PROFILE();
	}
	return shader;
}

glShaderBase* loadShader(const char* path)
{
	glShaderBase* shader=0;
	DataStreamPtr file = vfsMapFile(path);
	if(file)
	{
		const char* ext = strrchr(path, '.');
		byte* data = file->MapStream();
		if( strcmp(ext, ".vert")==0 )
			return loadShader<gl::VertexShader>(data);
		else if( strcmp(ext, ".frag")==0 )
			return loadShader<gl::FragmentShader>(data);
	}
	
	return 0;
}

glProgram* loadProgram(const char* shaders)
{
	char str[1024];
	strncpy(str, shaders, 1023);
	char* shaderPath=strtok(str, ";");
	glProgram *program = new glProgram();
	while( shaderPath )
	{
		glShaderBase* shader= loadShader(shaderPath);
		program->addShader(shader);
		delete shader;
		//program->getInfoLog();
		shaderPath = strtok(0, ";");
	}
	program->link();
	program->getLog(1023, &len, string); print(string);
	program->validate();
	program->getLog(1023, &len, string); print(string);
	return program;
}
