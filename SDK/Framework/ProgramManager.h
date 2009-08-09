#pragma once

#include <glClasses.h>
#include <ResourceManager.h>

class ProgramManager: public ResourceManager<ProgramManager, gl::Program>
{
	public:
		gl::Program* load(const std::string& name);
		gl::Program* load(const std::string& vsName, const std::string& fsName);
};

typedef ProgramManager::Reference	ProgramRef;