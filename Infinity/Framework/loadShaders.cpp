#include "Framework.h"
#include <cstring>

static char string[1024];
int len;

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
			shader->compile((GLchar*)file->MapStream());
			shader->getInfoLog(1023, &len, string);
			print(string);
			return shader;
		}
	}
	
	return 0;
}

glProgram* loadProgram(const char* shaders)
{
	char* shaderPath=strtok(const_cast<char*>(shaders), ";");
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
