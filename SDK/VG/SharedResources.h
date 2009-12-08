#pragma once

//Temporary - do we need to export internal stuff????
#define VG_EXPORT extern

namespace impl
{
	namespace shared
	{
		void Acquire();
		void Release();

		enum
		{
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
			//PRG_RAST_FILL_CUBIC,
			//PRG_RAST_FILL_QUAD,
			//PRG_RAST_FILL_ARC,
			//PRG_RAST_STROKE_CUBIC,
			UNI_RAST_STROKE_CUBIC_HALFWIDTH,
			//PRG_RAST_STROKE_QUAD,
			UNI_RAST_STROKE_QUAD_HALFWIDTH,
			//PRG_RAST_STROKE_ARC,
			UNI_RAST_STROKE_ARC_HALFWIDTH,
			//PRG_RAST_STROKE_TRI,
			UNI_RAST_STROKE_TRI_HALFWIDTH,
			UNI_COUNT
		};

		extern GLuint	programs[];
		extern GLint	uniforms[];

		//VG_EXPORT GLuint	prgFillColor;
		//VG_EXPORT GLuint	prgFillPattern;
	}
}