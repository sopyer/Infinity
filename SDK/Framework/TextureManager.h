#include <glClasses.h>
//#include <ResourceManager.h>
//
//class TextureManager: public ResourceManager<TextureManager, gl::Texture2D>
//{
//	public:
//		gl::Texture2D* load(const std::string& name);
//};
//
//typedef TextureManager::Reference	TextureRef;

#include <ResourceHandle.h>

bool testLoad(GLuint texture);
void loadTexture(const char* name, GLuint texture);

template<class T>
class TextureIface
{
public:
	//operator gl::Texture2D() {return *((T*)this);}

	void create(const char* name)
	{
		loadTexture(name, (GLuint)*(T*)this);
	}
};

typedef ResourceHandle<gl::Texture2D, TextureIface> TextureRef;
