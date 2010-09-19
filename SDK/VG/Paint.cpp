#include <vector>
#include <glm/glm.h>
#include <gl/glee.h>

#include "Paint.h"
#include "impl/SharedResources.h"

namespace vg
{
	Paint Paint::createSolid(unsigned int color)
	{
		//GLint uLocColor = glGetUniformLocation(impl::shared::prgFillColor, "uFillColor");
		//mObject->fillProgram.begin();
		//	glUseProgram(impl::shared::prgFillColor);
		//	glUniform4fv(uLocColor, 1, color);
		//	glBindTexture(GL_TEXTURE_2D, 0);
		//mObject->fillProgram.end();

		//mObject->color = color;
		//mObject->texture = 0;
	
		Paint	newPaint;

		newPaint.mHandle = glGenLists(1);

		glNewList(newPaint.mHandle, GL_COMPILE);
		glUseProgram(0);
		glColor4ubv((GLubyte*)&color);
		glEndList();

		return newPaint;
	}

	Paint Paint::createSolid(float* color4f)
	{
		//GLint uLocColor = glGetUniformLocation(impl::shared::prgFillColor, "uFillColor");
		//mObject->fillProgram.begin();
		//	glUseProgram(impl::shared::prgFillColor);
		//	glUniform4fv(uLocColor, 1, color);
		//	glBindTexture(GL_TEXTURE_2D, 0);
		//mObject->fillProgram.end();

		//mObject->color = color;
		//mObject->texture = 0;
	
		Paint	newPaint;

		newPaint.mHandle = glGenLists(1);

		glNewList(newPaint.mHandle, GL_COMPILE);
		glUseProgram(0);
		glColor4fv(color4f);
		glEndList();

		return newPaint;
	}

	void Paint::destroy(Paint paint)
	{
		glDeleteLists(paint.mHandle, 1);
		paint.mHandle = 0;
	}

	//void Paint::setColorPaint(const glm::vec4& color)
	//{
	//	//GLint uLocColor = glGetUniformLocation(impl::shared::prgFillColor, "uFillColor");
	//	//mObject->fillProgram.begin();
	//	//	glUseProgram(impl::shared::prgFillColor);
	//	//	glUniform4fv(uLocColor, 1, color);
	//	//	glBindTexture(GL_TEXTURE_2D, 0);
	//	//mObject->fillProgram.end();

	//	//mObject->color = color;
	//	//mObject->texture = 0;
	//}

	//void setPatternSamplerParams(const glm::vec4& fillColor, VGTilingMode mode)
	//{
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//	switch(mode)
	//	{
	//		case VG_TILE_FILL:
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, fillColor);
	//			break;
	//		case VG_TILE_PAD:
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//			break;
	//		case VG_TILE_REPEAT:
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//			break;
	//		case VG_TILE_REFLECT:
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//			break;
	//	}

	//	glColor4f(1,1,1,1);
	//}


	//void Paint::setPatternPaint(GLuint texture, const glm::vec4& fillColor, VGTilingMode mode)
	//{
	//	//GLint uLocImage    = glGetUniformLocation(impl::shared::prgFillPattern, "uPattern");
	//	//GLint uLocImageDim = glGetUniformLocation(impl::shared::prgFillPattern, "uImageDim");
	//	//glm::vec2	imgDim;

	//	//glBindTexture(GL_TEXTURE_2D, texture);
	//	//glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &imgDim.x);
	//	//glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &imgDim.y);

	//	//mObject->fillProgram.begin();
	//	//	glUseProgram(impl::shared::prgFillPattern);
	//	//	glUniform1i(uLocImage, 0);
	//	//	glUniform2fv(uLocImageDim, 1, imgDim);
	//	//	glActiveTexture(GL_TEXTURE0);
	//	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	//	setPatternSamplerParams(fillColor, mode);
	//	//mObject->fillProgram.end();

	//	//mObject->texture = texture;
	//	//mObject->tilingMode = mode;
	//}
}