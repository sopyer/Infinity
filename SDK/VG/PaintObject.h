#pragma once

#include <vector>
#include <glm/glm.h>
#include <vg/openvg.h>
#include ".\types.h"
#include <glClasses.h>

namespace vg
{
	struct PaintObject
	{
		gl::CommandList	fillProgram;
		glm::vec4		color;
		GLuint			texture;
		VGTilingMode	tilingMode;
	};
}