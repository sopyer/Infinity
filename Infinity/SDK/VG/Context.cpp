#include <glClasses.h>
#include "Context.h"
#include "PathObject.h"
#include "PaintObject.h"

namespace
{
	const char* const sourceArcFragSh = 
		"void main(void)							"
		"{											"
		"	vec2 uv = gl_TexCoord[0].st;			"
		"											"
		"	if( (uv.s*uv.s + uv.t*uv.t)>1.0 )		"
		"		discard;							"
		"											"
		"	gl_FragColor = vec4(1);					"
		"}											";

	const char* const sourceQuadFragSh = 
		"void main(void)							"
		"{											"
		"	vec2 uv = gl_TexCoord[0].st;			"
		"											"
		"	if( (pow(uv.s, 2.0)-uv.t)>0.0 )			"
		"		discard;							"
		"											"
		"	gl_FragColor = vec4(1);					"
		"}											";

	const char* const sourceCubicFragSh = 
		"void main(void)							"
		"{											"
		"	vec3 uv = gl_TexCoord[0].stp;			"
		"											"
		"	if( (pow(uv.s, 3.0)-uv.t*uv.p)>0.0 )	"
		"		discard;							"
		"											"
		"	gl_FragColor = vec4(1);					"
		"}											";

	const char* const sourceColorFillFragSh =
		"uniform vec4 uFillColor;					"
		"											"
		"void main()								"
		"{											"
		"	gl_FragColor = uFillColor;				"
		"}											";

}

namespace vg
{
	Context::Context()
	{
		gl::FragmentShader	fsArc;
		gl::FragmentShader	fsCubic;
		gl::FragmentShader	fsQuad;

		fsArc.compile(sourceArcFragSh);
		fsCubic.compile(sourceCubicFragSh);
		fsQuad.compile(sourceQuadFragSh);

		mMaskArc.addShader(fsArc).link();
		mMaskCubic.addShader(fsCubic).link();
		mMaskQuad.addShader(fsQuad).link();

		gl::FragmentShader	fsColor;

		fsColor.compile(sourceColorFillFragSh);

		mFillColor.addShader(fsColor).link();
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

		paint.mObject = new PaintObject(mFillColor, 0, 0);

		return paint;
	}

	void Context::destroyPaint(Paint paint)
	{
		delete paint.mObject;
	}

	void Context::maskPathRegion(Path path)
	{
		if (path.mObject->vertices.empty())
			return;

		glEnableClientState(GL_VERTEX_ARRAY);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glEnable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);

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

		GLint base = 0;
		glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &path.mObject->vertices[0]);
		for(size_t i=0; i<path.mObject->subPathes.size(); ++i)
		{
			GLsizei numPoints = path.mObject->subPathes[i];
			glDrawArrays(GL_TRIANGLE_FAN, base, numPoints);
			base += numPoints;
		}

		if (!path.mObject->quads.empty())
		{
			glUseProgram(mMaskQuad);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &path.mObject->quads[0]);
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), &path.mObject->quadsTC[0]);
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)path.mObject->quads.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if (!path.mObject->cubics.empty())
		{
			glUseProgram(mMaskCubic);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &path.mObject->cubics[0]);
			glTexCoordPointer(3, GL_FLOAT, 3*sizeof(float), &path.mObject->cubicsTC[0]);
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)path.mObject->cubics.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if (!path.mObject->arcs.empty())
		{
			glUseProgram(mMaskArc);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(float), &path.mObject->arcs[0]);
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), &path.mObject->arcsTC[0]);
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)path.mObject->arcs.size());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glPopAttrib();
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}