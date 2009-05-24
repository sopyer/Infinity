#include <vector>
#include <glm/glm.h>
#include <Framework.h>

#include "Paint.h"
#include "PaintObject.h"

namespace vg
{
	void Paint::setColorPaint(const glm::vec4& color)
	{
		GLint uLocColor = glGetUniformLocation(mObject->colorProgram, "uFillColor");
		mObject->fillProgram.begin();
			glUseProgram(mObject->colorProgram);
			glUniform4fv(uLocColor, 1, color);
		mObject->fillProgram.end();

		mObject->color = color;
	}
}