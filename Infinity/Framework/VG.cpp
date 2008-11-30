#include "VG.h"
#include <gl\gl.h>
#include <gl\glu.h>

void VG::begin()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	setOrthoProj();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(- (mWidth / 2.0f), mHeight / 2.0f, 0.0f);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void VG::end()
{
	glPopAttrib();
	glPopClientAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void VG::drawText(FTFont& font, GLint x, GLint y, const char* text)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)-y, 0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
	font.Render(text);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPopMatrix();
}

void VG::setOrthoProj()
{
	int w = mWidth;
	int h = mHeight;
	GLdouble aspect = (GLdouble)w / (GLdouble)h;

	glViewport(0, 0, w, h);
	// We are going to do some 2-D orthographic drawing.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble size = (GLdouble)((w >= h) ? w : h) / 2.0;

	if (w <= h)
	{
		aspect = (GLdouble)h/(GLdouble)w;
		glOrtho(-size, size, -size*aspect, size*aspect, -100000.0, 100000.0);
	}
	else
	{
		aspect = (GLdouble)w/(GLdouble)h;
		glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
	}
	// Make the world and window coordinates coincide so that 1.0 in
	// model space equals one pixel in window space.
	glScaled(aspect, aspect, 1.0);
	// Now determine where to draw things.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
