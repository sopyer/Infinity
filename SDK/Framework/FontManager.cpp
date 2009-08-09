#include <FontManager.h>
#include <..\vfs\vfs.h>

FTFont* FontManager::load(const std::string& name)
{	
	char			fntName[256];
	unsigned int	size;
	sscanf(name.c_str(), "%s %dpx", fntName, &size);
	//! Use VFS
	FTFont* font = new FTGLTextureFont(fntName);
	font->FaceSize(size);
	return font;
}