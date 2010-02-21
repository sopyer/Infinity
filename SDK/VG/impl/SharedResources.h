#pragma once

//Temporary - do we need to export internal stuff????
#define VG_EXPORT extern

namespace impl
{
	enum
	{
		PRG_SIMPLE_UI,
		PRG_RAST_FILL_CUBIC,
		PRG_RAST_FILL_QUAD,
		PRG_RAST_FILL_ARC,
		PRG_RAST_STROKE_CUBIC,
		PRG_RAST_STROKE_QUAD,
		PRG_RAST_STROKE_ARC,
		PRG_RAST_STROKE_TRI,
		PRG_COUNT
	};

	enum
	{
		//PRG_SIMPLE_UI uniforms
		UNI_SIMPLE_UI_FILL_COLOR,
		UNI_SIMPLE_UI_BORDER_COLOR,
		UNI_SIMPLE_UI_ZONES,
		//PRG_RAST_FILL_CUBIC uniforms
		//PRG_RAST_FILL_QUAD uniforms
		//PRG_RAST_FILL_ARC uniforms
		//PRG_RAST_STROKE_CUBIC uniforms
		UNI_RAST_STROKE_CUBIC_HALFWIDTH,
		//PRG_RAST_STROKE_QUAD uniforms
		UNI_RAST_STROKE_QUAD_HALFWIDTH,
		//PRG_RAST_STROKE_ARC uniforms
		UNI_RAST_STROKE_ARC_HALFWIDTH,
		//PRG_RAST_STROKE_TRI uniforms
		UNI_RAST_STROKE_TRI_HALFWIDTH,
		UNI_COUNT
	};

	extern GLuint	programs[];
	extern GLint	uniforms[];

	void acquire();
	void release();
}
