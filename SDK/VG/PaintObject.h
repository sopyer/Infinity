#pragma once

#include <vector>
#include <glm/glm.h>
#include <Framework.h>
#include <vg/openvg.h>
#include ".\types.h"

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