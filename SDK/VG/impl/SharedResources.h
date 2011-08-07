#pragma once

namespace impl
{
	enum
	{
		//PRG_SIMPLE_UI uniforms
		UNI_SIMPLE_UI_FILL_COLOR,
		UNI_SIMPLE_UI_BORDER_COLOR,
		UNI_SIMPLE_UI_ZONES,
		UNI_SIMPLE_UI_COUNT
	};

	extern GLuint	simpleUIProgram;
	extern GLint	simpleUIUniforms[UNI_SIMPLE_UI_COUNT];

	extern GLuint	stencilCubicAreaProgram;
	extern GLuint	stencilCubicAreaAAProgram;
	extern GLuint	stencilQuadAreaProgram;
	extern GLuint	stencilArcAreaProgram;

	enum
	{
		UNI_LIN_GRAD_STOPS,
		UNI_LIN_GRAD_SCALES,
		UNI_LIN_GRAD_INV_STOP_COUNT,
		UNI_LIN_GRAD_SAM_COLOR_RAMP,
		UNI_LIN_GRAD_START_POINT,
		UNI_LIN_GRAD_DIRECTION,
		UNI_LIN_GRAD_COUNT
	};

	extern GLuint	linGradProgram;
	extern GLint	linGradUniforms[UNI_LIN_GRAD_COUNT];

	void allocResources();
	void freeResources();
}
