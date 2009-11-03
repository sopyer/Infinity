#include <glClasses.h>
#include "Context.h"
#include "PathObject.h"
#include "PaintObject.h"
#include "SharedResources.h"

namespace vg
{
	Context::Context()
	{
		impl::shared::Acquire();
	}

	Context::~Context()
	{
		impl::shared::Release();
	}

	Path Context::createPath(float scale, float bias)
	{
		Path	path;

		path.mObject = new PathObject(scale, bias);

		return path;
	}

	void Context::drawPath(Path path, Paint fill)
	{
		maskPathRegion(path);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		//glEnable(GL_TEXTURE_2D);

		fill.mObject->fillProgram.execute();

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 1, 1);

		glBegin(GL_QUADS);
		glVertex2f(path.mObject->min.x, path.mObject->min.y);
		glVertex2f(path.mObject->max.x, path.mObject->min.y);
		glVertex2f(path.mObject->max.x, path.mObject->max.y);
		glVertex2f(path.mObject->min.x, path.mObject->max.y);
		glEnd();

		glPopAttrib();
	}

	void Context::drawPath(Path path, ProgramRef fill)
	{
		maskPathRegion(path);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glUseProgram(*fill);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 1, 1);

		glBegin(GL_QUADS);
		glVertex2f(path.mObject->min.x, path.mObject->min.y);
		glVertex2f(path.mObject->max.x, path.mObject->min.y);
		glVertex2f(path.mObject->max.x, path.mObject->max.y);
		glVertex2f(path.mObject->min.x, path.mObject->max.y);
		glEnd();

		glPopAttrib();
	}

	void Context::destroyPath(Path path)
	{
		delete path.mObject;
	}

	Paint Context::createPaint()
	{
		Paint	paint;

		paint.mObject = new PaintObject();

		return paint;
	}

	void Context::destroyPaint(Paint paint)
	{
		delete paint.mObject;
	}

	void Context::maskPathRegion(Path path)
	{
		//if (path.mObject->vertices.empty())
		//	return;

		glEnableClientState(GL_VERTEX_ARRAY);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);

		glCullFace(GL_FRONT_AND_BACK);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, 1);
		glStencilMask(0x01);

		// Clear stencil
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glBegin(GL_QUADS);
		glVertex2f(path.mObject->min.x, path.mObject->min.y);
		glVertex2f(path.mObject->max.x, path.mObject->min.y);
		glVertex2f(path.mObject->max.x, path.mObject->max.y);
		glVertex2f(path.mObject->min.x, path.mObject->max.y);
		glEnd();

		glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);

		glUseProgram(0);

		glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &path.mObject->vertRegion[0]);
		glDrawElements(GL_TRIANGLES, (GLsizei)path.mObject->idxRegion.size(), GL_UNSIGNED_INT, &path.mObject->idxRegion[0]);

		if (!path.mObject->vertQuad.empty())
		{
			glUseProgram(impl::shared::prgMaskQuad);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(VertexTex2), &path.mObject->vertQuad[0].p);
			glTexCoordPointer(2, GL_FLOAT, sizeof(VertexTex2), &path.mObject->vertQuad[0].tc);
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)path.mObject->vertQuad.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		//GLint base = 0;
		//glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &path.mObject->vertices[0]);
		//for(size_t i=0; i<path.mObject->subPathes.size(); ++i)
		//{
		//	GLsizei numPoints = path.mObject->subPathes[i];
		//	glDrawArrays(GL_TRIANGLE_FAN, base, numPoints);
		//	base += numPoints;
		//}

		//if (!path.mObject->quads.empty())
		//{
		//	glUseProgram(vg::shared::prgMaskQuad);
		//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glVertexPointer(2, GL_FLOAT, 0, &path.mObject->quads[0]);
		//	glTexCoordPointer(2, GL_FLOAT, 0, &path.mObject->quadsTC[0]);
		//	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)path.mObject->quads.size());
		//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//}

		//if (!path.mObject->cubics.empty())
		//{
		//	glUseProgram(vg::shared::prgMaskCubic);
		//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glVertexPointer(2, GL_FLOAT, 0, &path.mObject->cubics[0]);
		//	glTexCoordPointer(3, GL_FLOAT, 0, &path.mObject->cubicsTC[0]);
		//	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)path.mObject->cubics.size());
		//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//}

		//if (!path.mObject->arcs.empty())
		//{
		//	glUseProgram(vg::shared::prgMaskArc);
		//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		//	glVertexPointer(2, GL_FLOAT, 0, &path.mObject->arcs[0]);
		//	glTexCoordPointer(2, GL_FLOAT, 0, &path.mObject->arcsTC[0]);
		//	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)path.mObject->arcs.size());
		//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//}

		glPopAttrib();
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}