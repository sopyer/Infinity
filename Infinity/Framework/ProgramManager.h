#include <glClasses.h>
#include <ResourceManager.h>

class ProgramManager: public ResourceManager<ProgramManager, gl::Program>
{
	public:
		gl::Program* load(const std::string& name);
};

typedef ProgramManager::Reference	ProgramRef;