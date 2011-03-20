#include <glClasses.h>
#include "Context.h"
#include "impl/Path.h"
#include "impl/SharedResources.h"
#include <FTGLTextureFont.h>
#include <impl/Rasterizer.h>

namespace vg
{
	void Font::setFaceSize(VGuint size) {mHandle->FaceSize(size);}

	void drawQuad(const glm::vec2& min, const glm::vec2& max, float offset)
	{
		glBegin(GL_QUADS);
		glVertex2f(min.x-offset, min.y-offset);
		glVertex2f(min.x-offset, max.y+offset);
		glVertex2f(max.x+offset, max.y+offset);
		glVertex2f(max.x+offset, min.y-offset);
		glEnd();
	}

	void clearStencil(const glm::vec2& min, const glm::vec2& max, float offset)
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_ALWAYS, 0x80, 0xFF);
		glStencilMask(0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glUseProgram(0);

		drawQuad(min, max, offset);
	}

	void drawPath(Path path, VGubyte red, VGubyte green, VGubyte blue, VGubyte alpha)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		impl::RasterizeEvenOdd(path->mFillGeom);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glUseProgram(0);
		glColor4ub(red, green, blue, alpha);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		glPopAttrib();
	}

	void drawPath(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		impl::RasterizeEvenOdd(path->mFillGeom);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		glPopAttrib();
	}

	void drawPathA2C(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		impl::rasterizeEvenOddA2C(path->mFillGeom);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		glPopAttrib();
	}

	void drawPathNZA2C(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		rasterizeNonZeroA2C(path->mFillGeom);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		glPopAttrib();
	}

	void drawPathNZ(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		impl::RasterizeNonZero(path->mFillGeom);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		glPopAttrib();
	}

	Font createFont(const char* name)
	{
		Font	newFont;

		newFont.mHandle = new FTGLTextureFont(name);
		return newFont;
	}

	void destroyFont(Font font)
	{
		delete font.mHandle;
	}

	void drawString(Font font, float x, float y, const char* str)
	{
		if (!font.mHandle) return;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(1.0f, -1.0f, 1.0f);//It's hack to compansate text rendering algorithm
		glEnable(GL_TEXTURE_2D);

		font.mHandle->Render(str);
		
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	void drawString(Font font, float x, float y, const wchar_t* str)
	{
		if (!font.mHandle) return;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(1.0f, -1.0f, 1.0f);//It's hack to compansate text rendering algorithm
		glEnable(GL_TEXTURE_2D);

		font.mHandle->Render(str);
		
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	void clearAlpha(const glm::vec2& min, const glm::vec2& max, float offset)
	{
		glUseProgram(0);
	
		glPushAttrib(GL_ALL_ATTRIB_BITS);
	
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
		glColor4ub(0, 0, 0, 0);
		drawQuad(min, max, offset);

		glPopAttrib();
	}

	void drawPathAA(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glEnable(GL_STENCIL_TEST);
		clearAlpha(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);
		impl::RasterizeEvenOddAA(path->mFillGeom);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
		drawQuad(path->mFillGeom.mMin, path->mFillGeom.mMax, 0);

		glPopAttrib();
	}
}