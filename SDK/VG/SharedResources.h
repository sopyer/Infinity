#pragma once

//Temporary - do we need to export internal stuff????
#define VG_EXPORT extern

namespace vg
{
	namespace shared
	{
		void Acquire();
		void Release();

		VG_EXPORT GLuint	prgMaskQuad;
		VG_EXPORT GLuint	prgMaskCubic;
		VG_EXPORT GLuint	prgMaskArc;	
		VG_EXPORT GLuint	prgFillColor;
		VG_EXPORT GLuint	prgFillPattern;
	}
}