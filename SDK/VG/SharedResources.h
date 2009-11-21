#pragma once

//Temporary - do we need to export internal stuff????
#define VG_EXPORT extern

namespace impl
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
		VG_EXPORT GLuint	prgMaskStrokeSeg;
		VG_EXPORT GLuint	prgStrokeMaskQuad;
		VG_EXPORT GLuint	prgStrokeMaskCubic;
		VG_EXPORT GLuint	prgStrokeMaskArc;
		
		VG_EXPORT GLint		locOffsetAttribQuad;
		VG_EXPORT GLint		attrOffsetArc;
		VG_EXPORT GLint		attrOffsetCubic;
		VG_EXPORT GLint		locOffsetAttrib;
	}
}