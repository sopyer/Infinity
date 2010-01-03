#include <glClasses.h>
#include "Context.h"
#include "PathObject.h"
#include "PaintObject.h"
#include "SharedResources.h"

namespace vg
{
	Context::Context()
	{
		impl::acquire();
	}

	Context::~Context()
	{
		impl::release();
	}

	Font	createFont(const char* name)
	{
		Font	res = {new FTGLTextureFont(name)};
		return res;
	}

	void	destroyFont(Font font)
	{
		delete font.object;
	}

	void	drawString(Font font, float x, float y, const char* str)
	{
		if (!font.object) return;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(1.0f, -1.0f, 1.0f);//It's hack to compansate text rendering algorithm
		glEnable(GL_TEXTURE_2D);

		font.object->Render(str);
		
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	void	drawString(Font font, float x, float y, const wchar_t* str)
	{
		if (!font.object) return;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(1.0f, -1.0f, 1.0f);//It's hack to compansate text rendering algorithm
		glEnable(GL_TEXTURE_2D);

		font.object->Render(str);
		
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

}