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
		GLuint			colorProgram,
						linearGradientProgram,
						radialGradientProgram;
		glm::vec4		color;
		//gl::Texture2D	texture;

		PaintObject(GLuint progColor, GLuint progLinearGrad, GLuint progRadialGrad):
			colorProgram(progColor),
			linearGradientProgram(progLinearGrad),
			radialGradientProgram(progRadialGrad)
		{
		}
	};
}