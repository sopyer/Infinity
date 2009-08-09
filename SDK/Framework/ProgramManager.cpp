#include <ProgramManager.h>
#include <ShaderManager.h>
#include <..\vfs\vfs.h>

void logMessage(const char *s,...);
static	char	string[1024];
static	int		len;

template<typename Type>
glShaderBase* loadShader(const byte* text)
{
	Type*	shader = new Type();
	if (shader)
	{
		shader->compile((GLchar*)text);
		shader->getLog(1023, &len, string);
		logMessage(string);
	}
	return shader;
}

static glShaderBase* loadShader(const char* path)
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

gl::Program* ProgramManager::load(const std::string& name)
{
	gl::Program *program = new gl::Program();

	DataStreamPtr fin;

	char	str[1024];
	strncpy(str, name.c_str(), 1023);
	char*	shaderPath=strtok(str, ";");

	while( shaderPath )
	{
		ShaderRef shader;
		shader.create(shaderPath, shaderPath);
		program->addShader(*shader);
		shaderPath = strtok(0, ";");
	}
	

	program->link();
	program->getLog(1023, &len, string); logMessage(string);
	program->validate();
	program->getLog(1023, &len, string); logMessage(string);

	return program;
}

gl::Program* ProgramManager::load(const std::string& vsName, const std::string& fsName)
{
	gl::Program *program = new gl::Program();

	ShaderRef VShader;
	VShader.create(vsName, vsName);
	ShaderRef FShader;
	FShader.create(fsName, fsName);

	program->addShader(*VShader);
	program->addShader(*FShader);

	program->link();
	program->getLog(1023, &len, string); logMessage(string);
	program->validate();
	program->getLog(1023, &len, string); logMessage(string);

	return program;
}

