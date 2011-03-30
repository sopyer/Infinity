#include <gl/glee.h>
#include "VG.h"
#include "impl/SharedResources.h"
#include "impl/Rasterizer.h"

namespace vg
{
	void initFontSubsystem();
	void shutdownFontSubsystem();

	void init()
	{
		impl::acquire();
		initFontSubsystem();
	}

	void cleanup()
	{
		shutdownFontSubsystem();
		impl::release();
	}

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
		clearStencil(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		impl::rasterizeEvenOdd(*path);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glUseProgram(0);
		glColor4ub(red, green, blue, alpha);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		glPopAttrib();
	}

	void drawPath(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		impl::rasterizeEvenOdd(*path);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		glPopAttrib();
	}

	void drawPathA2C(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		impl::rasterizeEvenOddA2C(*path);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		glPopAttrib();
	}

	void drawPathNZA2C(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		rasterizeNonZeroA2C(*path);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		glPopAttrib();
	}

	void drawPathNZ(Path path, Paint paint)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_STENCIL_TEST);
		clearStencil(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		impl::rasterizeNonZero(*path);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_NOTEQUAL, 0x80, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		drawQuad(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		glPopAttrib();
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
		clearAlpha(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);
		impl::rasterizeEvenOddAA(*path);

		glCallList(paint);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
		drawQuad(glm::vec2(path->xmin, path->ymin), glm::vec2(path->xmax, path->ymax), 0);

		glPopAttrib();
	}
}