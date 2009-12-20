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

class TextureRef: public ResourceHandle<gl::Texture2D>
{
	typedef ResourceHandle<gl::Texture2D> base;
public:
	void create(const char* name);
};
