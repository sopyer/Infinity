#include <cassert>
#include <gl/glee.h>
#include <vector>
#include <hash_map>

#include "FTGLTextureFont.h"
#include "FTLibrary.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

const FT_Long DEFAULT_FACE_INDEX = 0;

struct GlyphData
{
	float			xadvance, yadvance; // The advance distance for this glyph
	float			xmin, ymin, xmax, ymax; // The bounding box of this glyph.
	unsigned int	width, height;
	float			xoffset, yoffset;
	float			u0, v0, u1, v1;
	int				glTextureID;
};

typedef std::hash_map<unsigned int, GlyphData> GlyphMap;

struct  GLFont
{
	//float Ascender() const;
	//float Descender() const;
	//float LineHeight() const;

	GLint activeTextureID;

	FT_Face ftFace;
	FT_Size ftSize;

	static const GLsizei textureWidth = 512;
	static const GLsizei textureHeight = 512;

	std::vector<GLuint> textureIDList;
	GlyphMap			mGlyphs;

	int glyphHeight, glyphWidth;
	int xOffset, yOffset;

	unsigned int padding; //A value to be added to the height and width to ensure that glyphs don't overlap in the texture
	unsigned int charSize;
};

bool initFaceSize(Font font, const unsigned int size, const unsigned int res = 72)
{
	FT_Face fontFace = font->ftFace;
	FT_Size fontSize;

	if (FT_Select_Charmap(fontFace, FT_ENCODING_UNICODE) || FT_Set_Char_Size(fontFace, 0L, size * 64, res, res))
		return false;

	font->charSize = size;
	fontSize = font->ftSize = fontFace->size;
	
	if(FT_IS_SCALABLE((fontFace)))
	{
		font->glyphWidth = (fontFace->bbox.xMax-fontFace->bbox.xMin) * ((float)(fontSize->metrics.x_ppem)/(float)(fontFace->units_per_EM));
		font->glyphHeight = (fontFace->bbox.yMax-fontFace->bbox.yMin) * ((float)fontSize->metrics.y_ppem/(float)fontFace->units_per_EM);
	}
	else
	{
		font->glyphWidth = static_cast<float>(fontSize->metrics.max_advance)/64.0f;
		font->glyphHeight = static_cast<float>(fontSize->metrics.height)/64.0f;
	}

	return true;
}

GLuint createTexture(Font font)
{   
	GLuint textID;
	glGenTextures(1, (GLuint*)&textID);

	glBindTexture(GL_TEXTURE_2D, textID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, font->textureWidth, font->textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	return textID;
}

GlyphData* getGlyphData(Font font, const unsigned int glyphIndex)
{
	GlyphMap::iterator	it = font->mGlyphs.find(glyphIndex);
	GlyphData*			tempGlyph = (it!= font->mGlyphs.end())?&it->second:0;

	if (NULL==tempGlyph)
	{
		FT_Error		err = FT_Load_Glyph(font->ftFace, glyphIndex, FT_LOAD_NO_HINTING);
		FT_GlyphSlot	ftGlyph = font->ftFace->glyph;

		if (!err && ftGlyph)
		{
			err = FT_Render_Glyph(ftGlyph, FT_RENDER_MODE_NORMAL);
			if (!err && ftGlyph->format == FT_GLYPH_FORMAT_BITMAP)
			{
				if (font->textureIDList.empty())
				{
					font->textureIDList.push_back(createTexture(font));
					font->xOffset = font->yOffset = font->padding;
				}

				if (font->xOffset > (font->textureWidth-font->glyphWidth))
				{
					font->xOffset = font->padding;
					font->yOffset += font->glyphHeight;

					if (font->yOffset > (font->textureHeight - font->glyphHeight))
					{
						font->textureIDList.push_back(createTexture(font));
						font->yOffset = font->padding;
					}
				}

				tempGlyph = &font->mGlyphs[glyphIndex];

				tempGlyph->glTextureID = font->textureIDList.back();

				FT_BBox bbox;
				FT_Outline_Get_CBox(&(ftGlyph->outline), &bbox);

				tempGlyph->xmin = (float)(bbox.xMin)/64.0f;
				tempGlyph->ymin = (float)(bbox.yMin)/64.0f;
				tempGlyph->xmax = (float)(bbox.xMax)/64.0f;
				tempGlyph->ymax = (float)(bbox.yMax)/64.0f;

				tempGlyph->xadvance = ftGlyph->advance.x/64.0f;
				tempGlyph->yadvance = ftGlyph->advance.y/64.0f;

				FT_Bitmap	bitmap = ftGlyph->bitmap;

				tempGlyph->width  = bitmap.width;
				tempGlyph->height = bitmap.rows;

				if( tempGlyph->width && tempGlyph->height)
				{
					glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
					glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
					glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

					glBindTexture(GL_TEXTURE_2D, tempGlyph->glTextureID);
					glTexSubImage2D(
						GL_TEXTURE_2D, 0,
						font->xOffset, font->yOffset,
						tempGlyph->width, tempGlyph->height,
						GL_ALPHA, GL_UNSIGNED_BYTE,
						bitmap.buffer
					);

					glPopClientAttrib();
				}

				tempGlyph->u0 = (float)(font->xOffset)/(float)(font->textureWidth);
				tempGlyph->v0 = (float)(font->yOffset)/(float)(font->textureHeight);
				tempGlyph->u1 = (float)(font->xOffset+tempGlyph->width) / (float)(font->textureWidth);
				tempGlyph->v1 = (float)(font->yOffset+tempGlyph->height) / (float)(font->textureHeight);

				tempGlyph->xoffset = (float)ftGlyph->bitmap_left;
				tempGlyph->yoffset = (float)ftGlyph->bitmap_top;

				font->xOffset += tempGlyph->xmax - tempGlyph->xmin + font->padding;
			}
		}
	}

	return tempGlyph;
}

void kernAdvance(Font font, unsigned int index1, unsigned int index2, float& x, float& y)
{
	x=0.0f;
	y=0.0f;

	if (FT_HAS_KERNING(font->ftFace) && index1 && index2)
	{
		FT_Vector kernAdvance;

		kernAdvance.x = kernAdvance.y = 0;

		FT_Error err = FT_Get_Kerning(font->ftFace, index1, index2, ft_kerning_unfitted, &kernAdvance);
		if (!err)
		{   
			x = static_cast<float>(kernAdvance.x)/64.0f;
			y = static_cast<float>(kernAdvance.y)/64.0f;
		}
	}
}

Font createFont(const char* fontPath, size_t faceSize)
{
	Font font =  new GLFont;
	font->glyphHeight = 0;
	font->glyphWidth = 0;
	font->xOffset = 0;
	font->yOffset = 0;
	font->padding = 3;
	FT_New_Face(*FTLibrary::Instance().GetLibrary(), fontPath, DEFAULT_FACE_INDEX, &font->ftFace);
	initFaceSize(font, faceSize);

	return font;
}

Font createFont(const unsigned char* fontData, size_t dataSize, size_t faceSize)
{
	Font font =  new GLFont;
	font->glyphHeight = 0;
	font->glyphWidth = 0;
	font->xOffset = 0;
	font->yOffset = 0;
	font->padding = 3;
	FT_New_Memory_Face(*FTLibrary::Instance().GetLibrary(), (FT_Byte*)fontData, dataSize, DEFAULT_FACE_INDEX, &font->ftFace);
	initFaceSize(font, faceSize);

	return font;
}

void destroyFont(Font font)
{
	FT_Done_Face(font->ftFace);
	
	if (!font->textureIDList.empty())
		glDeleteTextures(font->textureIDList.size(), (const GLuint*)&font->textureIDList[0]);
	
	delete font;
}

template<typename T>
void drawString(Font font, float x, float y, const T* str)
{   
	glPushAttrib(GL_ENABLE_BIT|GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE

	font->activeTextureID = 0;

	GlyphData*	glyph;

	unsigned int left = FT_Get_Char_Index(font->ftFace, *str);
	unsigned int right;

	while (*str++)
	{
		right = FT_Get_Char_Index(font->ftFace, *str);
		
		if (glyph = getGlyphData(font, left))
		{
			float xkern, ykern;
			kernAdvance(font, left, right, xkern, ykern);

			if (font->activeTextureID != glyph->glTextureID)
			{
				glBindTexture( GL_TEXTURE_2D, (GLuint)glyph->glTextureID);
				font->activeTextureID = glyph->glTextureID;
			}

			float	u0 = glyph->u0,
					v0 = glyph->v0,
					u1 = glyph->u1,
					v1 = glyph->v1;

			float	px = x+glyph->xoffset,
					py = y+glyph->yoffset,
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

			x += xkern+glyph->xadvance;
			y += ykern+glyph->yadvance;
		}

		left = right;
	}

	glPopAttrib();
}

template<> void drawString<char>(Font font, float x, float y, const char* str)
{
	drawString<unsigned char>(font, x, y, (const unsigned char*)str);
}

template void drawString<wchar_t>(Font font, float x, float y, const wchar_t* str);

//Optimize: advance>0, x always increases
template<typename T>
void getBounds(Font font, const T* str, float& xmin, float& ymin, float& xmax, float& ymax)
{
#	undef min
#	undef max

	xmin=ymin=xmax=ymax=0;

	if ((NULL!=str) && ('\0'!=*str))
	{
		bool		applyAdvance = false;
		float		advance = 0;
		GlyphData*	glyph;

		unsigned int left =FT_Get_Char_Index(font->ftFace, *str);
		unsigned int right = FT_Get_Char_Index(font->ftFace, *++str);

		if (glyph = getGlyphData(font, left))
		{
			xmin = glyph->xmin;
			ymin = glyph->ymin;
			xmax = glyph->xmax;
			ymax = glyph->ymax;

			float xadvance, yadvance;
			kernAdvance(font, left, right, xadvance, yadvance);
			advance = xadvance+glyph->xadvance;
		}

		left = right;

		while (*str++)
		{
			right = FT_Get_Char_Index(font->ftFace, *str);

			if (glyph = getGlyphData(font, left))
			{
				xmin = std::min(xmin, glyph->xmin+advance);
				ymin = std::min(ymin, glyph->ymin);
				xmax = std::max(xmax, glyph->xmax+advance);
				ymax = std::max(ymax, glyph->ymax);
				
				float xadvance, yadvance;
				
				kernAdvance(font, left, right, xadvance, yadvance);
				advance += xadvance+glyph->xadvance;

				applyAdvance = glyph->xmin==glyph->xmax; //is bbox empty?
			}

			left = right;
		}

		//Deal with spaces at the end of string, for spaces bbox is empty and advance is not applied
		//That's why we manually do this:
		if (applyAdvance)
		{
			xmin = std::min(xmin, advance);
			xmax = std::max(xmax, advance);
		}
	}
}

template void getBounds<char>(Font font, const char* string, float& xmin, float& ymin, float& xmax, float& ymax);
template void getBounds<wchar_t>(Font font, const wchar_t* string, float& xmin, float& ymin, float& xmax, float& ymax);

float getTextAscender(Font font)
{
	assert(font->ftSize!=0);
	return  (float)font->ftSize->metrics.ascender/64.0f;
}

float getTextDescender(Font font)
{
	assert(font->ftSize!=0);
	return (float)font->ftSize->metrics.descender/64.0f;
}

float getTextVExtent(Font font)
{
	return getTextAscender(font)-getTextDescender(font);
}

template<typename T>
float getTextHExtent(Font font, const T* str)
{
	float xmin, ymin, xmax, ymax;

	getBounds(font, str, xmin, ymin, xmax, ymax);

	return xmax-xmin;
}

template<> float getTextHExtent<char>(Font font, const char* str)
{
	return getTextHExtent<unsigned char>(font, (const unsigned char*)str);
}

template float getTextHExtent<wchar_t>(Font font, const wchar_t* str);

template<typename T>
void calcTextBasePt(Font font, float w, float h, size_t flags, const T* str, float& bx, float& by)
{
	switch (flags&3)
	{
		case TALIGN_RIGHT:
			bx = w-getTextHExtent(font, str);
			break;
		case TALIGN_HCENTER:
			bx = (w-getTextHExtent(font, str))/2;
			break;
		case TALIGN_LEFT:
		default:
			bx = 0;
			break;
	};

	switch (flags&12)
	{
		case TALIGN_BOTTOM:
			by = getTextDescender(font);
			break;
		case TALIGN_VCENTER:
			by = (h+getTextAscender(font)+getTextDescender(font))/2 ;
			break;
		case TALIGN_TOP:
		default:
			by = getTextAscender(font);
			break;
	};
}

template<> void calcTextBasePt<char>(Font font, float w, float h, size_t flags, const char* str, float& bx, float& by)
{
	return calcTextBasePt<unsigned char>(font, w, h, flags, (const unsigned char*)str, bx, by);
}

template void calcTextBasePt<wchar_t>(Font font, float w, float h, size_t flags, const wchar_t* str, float& bx, float& by);
