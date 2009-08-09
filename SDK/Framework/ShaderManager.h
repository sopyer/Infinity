#pragma once

#include <glClasses.h>
#include <ResourceManager.h>

class ShaderManager: public ResourceManager<ShaderManager, gl::Shader>
{
	public:
		gl::Shader* load(const std::string& name);
};

typedef ShaderManager::Reference	ShaderRef;