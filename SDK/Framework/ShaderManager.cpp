#include <ShaderManager.h>
#include <..\vfs++\vfs++.h>
#include <utils.h>

void logMessage(const char *s,...);
static	char	string[1024];
static	int		len;

template<typename Type>
glShaderBase* loadShader(const char* text)
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
	File	src = VFS::openRead(name.c_str());
	char*	data = new char[src.size()+1];
	const char*		ext = strrchr(name.c_str(), '.');
	gl::Shader*	shader=0;
	
	src.read(data, src.size(), 1);

	data[src.size()] = 0;

	if( strcmp(ext, ".vert")==0 )
		shader=loadShader<gl::VertexShader>(data);
	else if( strcmp(ext, ".frag")==0 )
		shader=loadShader<gl::FragmentShader>(data);
	
	delete [] data;

	return shader;
}