#include <cassert>

#include "FTGLTextureFont.h"
#include "FTLibrary.h"

const FT_Long DEFAULT_FACE_INDEX = 0;

#undef min
#undef max

GLFont::GLFont(const char* fontFilePath, unsigned int size)
	:	glyphHeight(0),	glyphWidth(0),
	xOffset(0),	yOffset(0),
	padding(3)
{
	FT_New_Face(*FTLibrary::Instance().GetLibrary(), fontFilePath, DEFAULT_FACE_INDEX, &ftFace);
	FaceSize(size);
}

GLFont::GLFont(const unsigned char *pBufferBytes, size_t bufferSizeInBytes, unsigned int size)
	:	glyphHeight(0), glyphWidth(0),
	xOffset(0),	yOffset(0),
	padding(3)
{
	FT_New_Memory_Face(*FTLibrary::Instance().GetLibrary(), (FT_Byte *)pBufferBytes, bufferSizeInBytes, DEFAULT_FACE_INDEX, &ftFace);
	FaceSize(size);
}


GLFont::~GLFont()
{
	FT_Done_Face(ftFace);
	if (!textureIDList.empty())
		glDeleteTextures(textureIDList.size(), (const GLuint*)&textureIDList[0]);
}

GLuint GLFont::CreateTexture()
{   
	GLuint textID;
	glGenTextures(1, (GLuint*)&textID);

	glBindTexture(GL_TEXTURE_2D, textID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	return textID;
}


bool GLFont::FaceSize( const unsigned int size, const unsigned int res)
{
	if (FT_Select_Charmap(ftFace, FT_ENCODING_UNICODE) || FT_Set_Char_Size(ftFace, 0L, size * 64, res, res))
		return false;

	charSize = size;
	ftSize = ftFace->size;

	if(FT_IS_SCALABLE((ftFace)))
	{
		glyphWidth = (ftFace->bbox.xMax-ftFace->bbox.xMin) * ((float)(ftSize->metrics.x_ppem)/(float)(ftFace->units_per_EM));
		glyphHeight = (ftFace->bbox.yMax-ftFace->bbox.yMin) * ((float)ftSize->metrics.y_ppem/(float)ftFace->units_per_EM);
	}
	else
	{
		glyphWidth = static_cast<float>(ftSize->metrics.max_advance)/64.0f;
		glyphHeight = static_cast<float>(ftSize->metrics.height)/64.0f;
	}

	return true;
}


unsigned int GLFont::FaceSize() const
{
	return charSize;
}


float GLFont::Ascender() const
{
	assert(ftSize!=0);
	return  (float)ftSize->metrics.ascender/64.0f;
}


float GLFont::Descender() const
{
	assert(ftSize!=0);
	return (float)ftSize->metrics.descender/64.0f;
}


float GLFont::LineHeight() const
{
	return glyphHeight;
}

GLFont::GlyphData* GLFont::getGlyphData(const unsigned int glyphIndex)
{
	GlyphMap::iterator	it = mGlyphs.find(glyphIndex);
	GlyphData*			tempGlyph = (it!= mGlyphs.end())?&it->second:0;

	if (NULL==tempGlyph)
	{
		FT_Error		err = FT_Load_Glyph(ftFace, glyphIndex, FT_LOAD_NO_HINTING);
		FT_GlyphSlot	ftGlyph = ftFace->glyph;

		if (!err && ftGlyph)
		{
			err = FT_Render_Glyph(ftGlyph, FT_RENDER_MODE_NORMAL);
			if (!err && ftGlyph->format == FT_GLYPH_FORMAT_BITMAP)
			{
				if (textureIDList.empty())
				{
					textureIDList.push_back(CreateTexture());
					xOffset = yOffset = padding;
				}

				if (xOffset > (textureWidth-glyphWidth))
				{
					xOffset = padding;
					yOffset += glyphHeight;

					if (yOffset > ( textureHeight - glyphHeight))
					{
						textureIDList.push_back(CreateTexture());
						yOffset = padding;
					}
				}

				tempGlyph = &mGlyphs[glyphIndex];

				tempGlyph->glTextureID = textureIDList.back();

				FT_BBox bbox;
				FT_Outline_Get_CBox(&(ftGlyph->outline), &bbox);

				tempGlyph->xmin = static_cast<float>(bbox.xMin) / 64.0f;
				tempGlyph->ymin = static_cast<float>(bbox.yMin) / 64.0f;
				tempGlyph->xmax = static_cast<float>(bbox.xMax) / 64.0f;
				tempGlyph->ymax = static_cast<float>(bbox.yMax) / 64.0f;

				tempGlyph->xadvance = ftGlyph->advance.x / 64.0f;
				tempGlyph->yadvance = ftGlyph->advance.y / 64.0f;

				FT_Bitmap	bitmap = ftGlyph->bitmap;

				tempGlyph->width  = bitmap.width;
				tempGlyph->height = bitmap.rows;

				if( tempGlyph->width && tempGlyph->height)
				{
					glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);
					glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE);
					glPixelStorei( GL_UNPACK_ROW_LENGTH, 0);
					glPixelStorei( GL_UNPACK_ALIGNMENT, 1);

					glBindTexture( GL_TEXTURE_2D, tempGlyph->glTextureID);
					glTexSubImage2D( GL_TEXTURE_2D, 0, xOffset, yOffset, tempGlyph->width, tempGlyph->height, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.buffer);

					glPopClientAttrib();
				}

				tempGlyph->u0 = static_cast<float>(xOffset) / static_cast<float>(textureWidth);
				tempGlyph->v0 = static_cast<float>(yOffset) / static_cast<float>(textureHeight);
				tempGlyph->u1 = static_cast<float>(xOffset + tempGlyph->width) / static_cast<float>(textureWidth);
				tempGlyph->v1 = static_cast<float>(yOffset + tempGlyph->height) / static_cast<float>(textureHeight);

				tempGlyph->xoffset = (float)ftGlyph->bitmap_left;
				tempGlyph->yoffset = (float)ftGlyph->bitmap_top;

				xOffset += tempGlyph->xmax - tempGlyph->xmin + padding;
			}
		}
	}

	return tempGlyph;
}

//Optimize: advance > 0, x always increases
template<typename T>
void GLFont::getBounds(const T* string, float& xmin, float& ymin, float& xmax, float& ymax)
{
	xmin=ymin=xmax=ymax=0;

	if ((NULL!=string) && ('\0'!=*string))
	{
		const T*	c = string;
		bool		applyAdvance = false;
		float		advance = 0;
		GlyphData*	glyph;

		unsigned int left =FT_Get_Char_Index(ftFace, *c);
		unsigned int right = FT_Get_Char_Index(ftFace, *++c));

		if (glyph = getGlyphData(left))
		{
			xmin = glyph->xmin;
			ymin = glyph->ymin;
			xmax = glyph->xmax;
			ymax = glyph->ymax;

			float xadvance, yadvance;
			KernAdvance(left, right, xadvance, yadvance);
			advance = xadvance+glyph->xadvance;
		}

		left = right;

		while (*c++)
		{
			right = FT_Get_Char_Index(ftFace, *c);

			if (glyph = getGlyphData(left))
			{
				xmin = std::min(xmin, glyph->xmin+advance);
				ymin = std::min(ymin, glyph->ymin);
				xmax = std::max(xmax, glyph->xmax+advance);
				ymax = std::max(ymax, glyph->ymax);
				
				float xadvance, yadvance;
				
				KernAdvance(left, right, xadvance, yadvance);
				advance += xadvance+glyph->xadvance;

				applyAdvance = glyph->xmin==glyph->xmax;//tempBBox.IsEmpty();
			}

			left = right;
		}

		//Deal with spaces at the end of string, for spaces BBox.IsEmpty()==true and advance is not apply
		//That's why we manually do this:
		if (applyAdvance)
		{
			xmin = std::min(xmin, advance);
			xmax = std::max(xmax, advance);
		}
	}
}

template void GLFont::getBounds<char>(const char* string, float& xmin, float& ymin, float& xmax, float& ymax);
template void GLFont::getBounds<wchar_t>(const wchar_t* string, float& xmin, float& ymin, float& xmax, float& ymax);

void GLFont::KernAdvance(unsigned int index1, unsigned int index2, float& x, float& y)
{
	x=0.0f;
	y=0.0f;

	if (FT_HAS_KERNING(ftFace) && index1 && index2)
	{
		FT_Vector kernAdvance;

		kernAdvance.x = kernAdvance.y = 0;

		FT_Error err = FT_Get_Kerning(ftFace, index1, index2, ft_kerning_unfitted, &kernAdvance);
		if (!err)
		{   
			x = static_cast<float>(kernAdvance.x) / 64.0f;
			y = static_cast<float>(kernAdvance.y) / 64.0f;
		}
	}
}

template<typename T>
void GLFont::Render(const T* string)
{   
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE

	activeTextureID = 0;

	GlyphData*	glyph;
	const T*	c = string;
	float		xpos=0, ypos=0;

	unsigned int left = FT_Get_Char_Index(ftFace, *c);
	unsigned int right;

	while (*c++)
	{
		right = FT_Get_Char_Index(ftFace, *c);
		
		if (glyph = getGlyphData(left))
		{
			float xkern, ykern;
			KernAdvance(left, right, xkern, ykern);

			if (activeTextureID != glyph->glTextureID)
			{
				glBindTexture( GL_TEXTURE_2D, (GLuint)glyph->glTextureID);
				activeTextureID = glyph->glTextureID;
			}

			float	u0 = glyph->u0,
					v0 = glyph->v0,
					u1 = glyph->u1,
					v1 = glyph->v1;

			float	px = xpos+glyph->xoffset,
					py = ypos+glyph->yoffset,
					w  = (float)glyph->width,
					h  = (float)glyph->height;

			glBegin(GL_QUADS);
			glTexCoord2f(u0, v0);
			glVertex2f(px, py);

			glTexCoord2f(u0, v1);
			glVertex2f(px, py - h);

			glTexCoord2f(u1, v1);
			glVertex2f(w + px, py - h);

			glTexCoord2f(u1, v0);
			glVertex2f(w + px, py);
			glEnd();

			xpos += xkern+glyph->xadvance;
			ypos += ykern+glyph->yadvance;
		}

		left = right;
	}

	glPopAttrib();
}

template void GLFont::Render<char>(const char* string);
template void GLFont::Render<wchar_t>(const wchar_t* string);
