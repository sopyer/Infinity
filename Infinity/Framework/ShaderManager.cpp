#include <ShaderManager.h>
#include <..\vfs\vfs.h>

void logMessage(const char *s,...);
static	char	string[1024];
static	int		len;

template<typename Type>
glShaderBase* loadShader(const byte* text)
{
	Type*	shader = new Type();
	if (text)
	{
		shader->compile((GLchar*)text);
		shader->getLog(1023, &len, string);
		logMessage(string);
	}
	return shader;
}

gl::Shader* ShaderManager::load(const std::string& name)
{
	DataStreamPtr	file = vfsMapFile(name.c_str());
	byte*			data = file ? file->MapStream() : 0;
	const char*		ext = strrchr(name.c_str(), '.');

	if( strcmp(ext, ".vert")==0 )
		return loadShader<gl::VertexShader>(data);
	else if( strcmp(ext, ".frag")==0 )
		return loadShader<gl::FragmentShader>(data);
	
	return 0;
}