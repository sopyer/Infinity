#include <glClasses.h>
#include <ResourceManager.h>
#include <FTGLTextureFont.h>

class FontManager: public ResourceManager<FontManager, FTFont>
{
	public:
		FTFont* load(const std::string& name);
};

typedef FontManager::Reference	FontRef;