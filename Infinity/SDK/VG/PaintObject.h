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
						radialGradientProgram,
						patternProgram;
		glm::vec4		color;
		GLuint			texture;
		VGTilingMode	tilingMode;

		PaintObject(GLuint progColor, GLuint progLinearGrad, GLuint progRadialGrad, GLuint progPattern):
			colorProgram(progColor),
			linearGradientProgram(progLinearGrad),
			radialGradientProgram(progRadialGrad),
			patternProgram(progPattern)
		{
		}
	};
}