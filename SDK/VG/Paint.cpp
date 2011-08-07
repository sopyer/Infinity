#include "VG.h"
#include "Paint.h"
#include "impl/SharedResources.h"

#include <float.h>
#include <assert.h>

namespace vg
{
	Paint createSolidPaint(float* color4f)
	{
		Paint	newPaint = new PaintOpaque;

		newPaint->displayList	= glGenLists(1);
		newPaint->texture		= 0;

		glNewList(newPaint->displayList, GL_COMPILE);
		glUseProgram(0);
		glColor4fv(color4f);
		glEndList();

		return newPaint;
	}

	Paint createSolidPaint(unsigned int color)
	{
		Paint	newPaint = new PaintOpaque;

		newPaint->displayList	= glGenLists(1);
		newPaint->texture		= 0;

		glNewList(newPaint->displayList, GL_COMPILE);
		glUseProgram(0);
		glColor4ubv((GLubyte*)&color);
		glEndList();

		return newPaint;
	}

	Paint createLinearGradientPaint(float x0, float y0, float x1, float y1, size_t stopCount, float stops[], unsigned int colorRamp[])
	{
		Paint	newPaint = new PaintOpaque;

		newPaint->displayList	= glGenLists(1);
		glGenTextures(1, &newPaint->texture);
		glBindTexture(GL_TEXTURE_1D, newPaint->texture);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, stopCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorRamp);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glNewList(newPaint->displayList, GL_COMPILE);
		glUseProgram(impl::linGradProgram);

		float stopsData[8*4], scalesData[8*4];

		for (int i=0; i<8*4; ++i)
		{
			stopsData[i]  = 1.0f;
			scalesData[i] = 0.0f;
		}

		memcpy(stopsData, stops, stopCount*4);

		for (size_t i=0; i<stopCount-1; ++i)
		{
			float delta = stops[i+1]-stops[i];
			assert(delta>=0);
			scalesData[i] = (delta>0)?(1.0f/delta):FLT_MAX;
		}

		glUniform4fv(impl::linGradUniforms[impl::UNI_LIN_GRAD_STOPS],  1, stopsData);
		glUniform4fv(impl::linGradUniforms[impl::UNI_LIN_GRAD_SCALES], 1, scalesData);
		glUniform1f (impl::linGradUniforms[impl::UNI_LIN_GRAD_INV_STOP_COUNT], 1.0f/stopCount);
		glUniform1i (impl::linGradUniforms[impl::UNI_LIN_GRAD_SAM_COLOR_RAMP], 0);

		float dx=x1-x0, dy=y1-y0;
		float scale = dx*dx+dy*dy;

		glUniform2f(impl::linGradUniforms[impl::UNI_LIN_GRAD_START_POINT], x0, y0);
		glUniform2f(impl::linGradUniforms[impl::UNI_LIN_GRAD_DIRECTION], scale?dx/scale:FLT_MAX, scale?dy/scale:FLT_MAX);
		glEndList();

		return newPaint;
	}

	void applyPaintAsGLProgram(Paint paint)
	{
		glCallList(paint->displayList);
		if (paint->texture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_1D, paint->texture);
		}
	}

	void destroyPaint(Paint paint)
	{
		glDeleteLists(paint->displayList, 1);
		if (paint->texture) glDeleteTextures(1, &paint->texture);

		delete paint;
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