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
			glBindTexture(GL_TEXTURE_2D, 0);
		mObject->fillProgram.end();

		mObject->color = color;
		mObject->texture = 0;
	}

	void setPatternSamplerParams(const glm::vec4& fillColor, VGTilingMode mode)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		switch(mode)
		{
			case VG_TILE_FILL:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, fillColor);
				break;
			case VG_TILE_PAD:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			case VG_TILE_REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case VG_TILE_REFLECT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
		}

		glColor4f(1,1,1,1);
	}


	void Paint::setPatternPaint(GLuint texture, const glm::vec4& fillColor, VGTilingMode mode)
	{
		GLint uLocImage    = glGetUniformLocation(mObject->patternProgram, "uPattern");
		GLint uLocImageDim = glGetUniformLocation(mObject->patternProgram, "uImageDim");
		glm::vec2	imgDim;

		mObject->fillProgram.begin();
			glUseProgram(mObject->patternProgram);
			glUniform1i(uLocImage, 0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &imgDim.x);
			glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &imgDim.y);
			glUniform2fv(uLocImageDim, 1, imgDim);
			setPatternSamplerParams(fillColor, mode);
		mObject->fillProgram.end();

		mObject->texture = texture;
		mObject->tilingMode = mode;
	}
}