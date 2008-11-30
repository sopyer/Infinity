#pragma once

#include <FTGLTextureFont.h>

class VG
{
public:
	VG(GLuint width, GLuint height): mWidth(width), mHeight(height) {}

	void begin();
	void end();

	void drawText(FTFont* font, GLint x, GLint y, const char* text) {if (font) drawText(*font, x, y, text);}
	void drawText(FTFont& font, GLint x, GLint y, const char* text);

private:
	void setOrthoProj();

private:
	GLuint	mWidth;
	GLuint	mHeight;
};