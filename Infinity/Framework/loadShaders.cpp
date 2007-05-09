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
#endif

glShader* loadShader(const char* path)
{
	glShader* shader=0;
	DataStreamPtr file = vfsMapFile(path);
	if(file)
	{
		const char* ext = strrchr(path, '.');
		if( strcmp(ext, ".vert")==0 )
			shader = new glVertexShader();
		else if( strcmp(ext, ".frag")==0 )
			shader = new glFragmentShader();
		if( shader )
		{
			START_PROFILE("compileShader");
			shader->compile((GLchar*)file->MapStream());
			END_PROFILE();
			START_PROFILE("infoShader");
			shader->getInfoLog(1023, &len, string);
			print(string);
			END_PROFILE();
			return shader;
		}
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
		glShader* shader= loadShader(shaderPath);
		program->addShader(shader);
		delete shader;
		//program->getInfoLog();
		shaderPath = strtok(0, ";");
	}
	program->link();
	program->getInfoLog(1023, &len, string);
	print(string);
	program->validate();
	program->getInfoLog(1023, &len, string);
	print(string);
	return program;
}
