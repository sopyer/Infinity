#pragma once

#include <VG/OpenVG.h>
#include ".\types.h"

//namespace VG
namespace vg
{
	struct PaintObject;
	
	struct Paint: public PtrToImpl<PaintObject>
	{
		void setColorPaint(const glm::vec4& color);
		void setPatternPaint(GLuint texture, const glm::vec4& fillColor, VGTilingMode mode);
	};
}