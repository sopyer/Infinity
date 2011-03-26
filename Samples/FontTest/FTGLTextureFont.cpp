#include <cassert>

#include "FTGLTextureFont.h"
#include "FTTextureGlyph.h"
#include "FTGlyphContainer.h"
#include "FTLibrary.h"

inline GLuint NextPowerOf2( GLuint in)
{
	in -= 1;

	in |= in >> 16;
	in |= in >> 8;
	in |= in >> 4;
	in |= in >> 2;
	in |= in >> 1;

	return in + 1;
}

const FT_Long DEFAULT_FACE_INDEX = 0;

FTGLTextureFontNew::FTGLTextureFontNew( const char* fontFilePath)
:	textureWidth(0), textureHeight(0),
	glyphHeight(0),	glyphWidth(0),
	xOffset(0),	yOffset(0),
	padding(3)
{
    err = FT_New_Face( *FTLibrary::Instance().GetLibrary(), fontFilePath, DEFAULT_FACE_INDEX, &ftFace);
}

FTGLTextureFontNew::FTGLTextureFontNew( const unsigned char *pBufferBytes, size_t bufferSizeInBytes)
:	textureWidth(0), textureHeight(0),
	glyphHeight(0), glyphWidth(0),
	xOffset(0),	yOffset(0),
	padding(3)
{
    err = FT_New_Memory_Face(*FTLibrary::Instance().GetLibrary(), (FT_Byte *)pBufferBytes, bufferSizeInBytes, DEFAULT_FACE_INDEX, &ftFace);
}


FTGLTextureFontNew::~FTGLTextureFontNew()
{
    FT_Done_Face(ftFace);
	glDeleteTextures(textureIDList.size(), (const GLuint*)&textureIDList[0]);
 
	GlyphVector::iterator glyphIterator;
    for( glyphIterator = glyphs.begin(); glyphIterator != glyphs.end(); ++glyphIterator)
    {
        delete *glyphIterator;
    }
    
    glyphs.clear();
    charMap.clear();
}


FTTextureGlyphNew* FTGLTextureFontNew::MakeGlyph( unsigned int glyphIndex)
{
    err = FT_Load_Glyph(ftFace, glyphIndex, FT_LOAD_NO_HINTING);
    if( err)
    {
        return NULL;
    }

	FT_GlyphSlot ftGlyph = ftFace->glyph;

	if( ftGlyph)
	{
		if( textureIDList.empty())
		{
			textureIDList.push_back( CreateTexture());
			xOffset = yOffset = padding;
		}

		if( xOffset > ( textureWidth - glyphWidth))
		{
			xOffset = padding;
			yOffset += glyphHeight;

			if( yOffset > ( textureHeight - glyphHeight))
			{
				textureIDList.push_back( CreateTexture());
				yOffset = padding;
			}
		}

		FTTextureGlyphNew* tempGlyph = new FTTextureGlyphNew;
		//CreateTexGlyph(*tempGlyph, ftGlyph, textureIDList[textureIDList.size() - 1],	xOffset, yOffset, textureWidth, textureHeight);

		tempGlyph->glTextureID = textureIDList[textureIDList.size() - 1];

		FT_BBox bbox;
		FT_Outline_Get_CBox( &(ftGlyph->outline), &bbox);

		tempGlyph->xmin = static_cast<float>( bbox.xMin) / 64.0f;
		tempGlyph->ymin = static_cast<float>( bbox.yMin) / 64.0f;
		tempGlyph->xmax = static_cast<float>( bbox.xMax) / 64.0f;
		tempGlyph->ymax = static_cast<float>( bbox.yMax) / 64.0f;

		tempGlyph->xadvance = ftGlyph->advance.x / 64.0f;
		tempGlyph->yadvance = ftGlyph->advance.y / 64.0f;

		FT_Error err = FT_Render_Glyph( ftGlyph, FT_RENDER_MODE_NORMAL);
		if( err || ftGlyph->format != ft_glyph_format_bitmap)
		{
			return 0;
		}

		FT_Bitmap      bitmap = ftGlyph->bitmap;

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
		tempGlyph->u1 = static_cast<float>( xOffset + tempGlyph->width) / static_cast<float>(textureWidth);
		tempGlyph->v1 = static_cast<float>( yOffset + tempGlyph->height) / static_cast<float>(textureHeight);

		tempGlyph->xoffset = (float)ftGlyph->bitmap_left;
		tempGlyph->yoffset = (float)ftGlyph->bitmap_top;

		xOffset += tempGlyph->xmax - tempGlyph->xmin + padding;

		--remGlyphs;
		return tempGlyph;
	}

	return NULL;
}


GLuint FTGLTextureFontNew::CreateTexture()
{   
	GLuint textID;
	glGenTextures( 1, (GLuint*)&textID);

	glBindTexture( GL_TEXTURE_2D, textID);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	return textID;
}


bool FTGLTextureFontNew::FaceSize( const unsigned int size, const unsigned int res)
{
	if( err == 0)
	{
		err = FT_Select_Charmap(ftFace, FT_ENCODING_UNICODE );
		glyphs.push_back( NULL);
	}
	hasKerningTable = !!FT_HAS_KERNING(ftFace);
	remGlyphs = numGlyphs = ftFace->num_glyphs;

	if( !textureIDList.empty())
	{
		glDeleteTextures( textureIDList.size(), (const GLuint*)&textureIDList[0]);
		textureIDList.clear();
		remGlyphs = numGlyphs = ftFace->num_glyphs;
	}

    err = FT_Set_Char_Size(ftFace, 0L, size * 64, res, res);
	charSize = size;
	ftSize = ftFace->size;

	if (err != 0)
	{
		return false;
	}

	GlyphVector::iterator glyphIterator;
    for( glyphIterator = glyphs.begin(); glyphIterator != glyphs.end(); ++glyphIterator)
    {
        delete *glyphIterator;
    }
    
    glyphs.clear();
    charMap.clear();

	err = FT_Select_Charmap(ftFace, FT_ENCODING_UNICODE);
	glyphs.push_back(NULL);

    if( FT_IS_SCALABLE((ftFace)))
        glyphHeight = (ftFace->bbox.yMax-ftFace->bbox.yMin) * ((float)ftSize->metrics.y_ppem/(float)ftFace->units_per_EM);
    else
        glyphHeight = static_cast<float>( ftSize->metrics.height) / 64.0f;

    if( FT_IS_SCALABLE(ftFace))
        glyphWidth = (ftFace->bbox.xMax-ftFace->bbox.xMin) * ((float)(ftSize->metrics.x_ppem)/(float)(ftFace->units_per_EM));
    else
        glyphWidth = static_cast<float>( ftSize->metrics.max_advance) / 64.0f;

	GLsizei maximumGLTextureSize;

	glGetIntegerv( GL_MAX_TEXTURE_SIZE, (GLint*)&maximumGLTextureSize);
	assert(maximumGLTextureSize); // If you hit this then you have an invalid OpenGL context.

	textureWidth = NextPowerOf2( (remGlyphs * glyphWidth) + ( padding * 2));
	textureWidth = textureWidth > maximumGLTextureSize ? maximumGLTextureSize : textureWidth;

	int h = static_cast<int>( (textureWidth - ( padding * 2)) / glyphWidth);

	textureHeight = NextPowerOf2( (( numGlyphs / h) + 1) * glyphHeight);
	textureHeight = textureHeight > maximumGLTextureSize ? maximumGLTextureSize : textureHeight;

	return true;
}


unsigned int FTGLTextureFontNew::FaceSize() const
{
	return charSize;
}


float FTGLTextureFontNew::Ascender() const
{
	assert(ftSize!=0);
    return  (float)ftSize->metrics.ascender / 64.0f;
}


float FTGLTextureFontNew::Descender() const
{
	assert(ftSize!=0);
    return (float)ftSize->metrics.descender / 64.0f;
}


float FTGLTextureFontNew::LineHeight() const
{
	return glyphHeight;
}


unsigned int unsignedExpand(const char c) {return (unsigned char) c;}
unsigned int unsignedExpand(const wchar_t c) {return c;}

//Optimize: advance > 0, x always increases
template<typename T>
void FTGLTextureFontNew::getBounds(const T* string, float& xmin, float& ymin, float& xmax, float& ymax)
{
	//FTBBox totalBBox;
	bool	applyAdvance = false;

	if((NULL != string) && ('\0' != *string))
	{
		const T* c = string;
		float advance = 0;

		if(CheckGlyph( *c))
		{
			//totalBBox = Glyph(*c)->bBox;
			xmin = Glyph(*c)->xmin;
			ymin = Glyph(*c)->ymin;
			xmax = Glyph(*c)->xmax;
			ymax = Glyph(*c)->ymax;
			advance = Advance( *c, *(c + 1));
		}

		while( *++c)
		{
			if(CheckGlyph( *c))
			{
				//FTBBox tempBBox = Glyph(*c)->bBox;
				//tempBBox.Move( FTPoint( advance, 0.0f, 0.0f));
				//totalBBox += tempBBox;
				xmin = std::min(xmin, Glyph(*c)->xmin+advance);
				ymin = std::min(ymin, Glyph(*c)->ymin);
				xmax = std::max(xmax, Glyph(*c)->xmax+advance);
				ymax = std::max(ymax, Glyph(*c)->ymax);

				advance += Advance( *c, *(c + 1));
				applyAdvance = Glyph(*c)->xmin==Glyph(*c)->xmax;//tempBBox.IsEmpty();
			}
		}

		//Deal with spaces at the end of string, for spaces BBox.IsEmpty()==true and advance is not apply
		//That's why we manually do this:
		if (applyAdvance)
		{
			//FTBBox tempBBox;
			//tempBBox.Move(FTPoint( advance, 0.0f, 0.0f));
			//totalBBox += tempBBox;
			//llx += advance;
			xmin = std::min(xmin, advance);
			xmax = std::max(xmax, advance);
		}
	}

	//llx = totalBBox.lower.x;
	//lly = totalBBox.lower.y;
	//urx = totalBBox.upper.x;
	//ury = totalBBox.upper.y;
}

template void FTGLTextureFontNew::getBounds<char>(const char* string, float& xmin, float& ymin, float& xmax, float& ymax);
template void FTGLTextureFontNew::getBounds<wchar_t>(const wchar_t* string, float& xmin, float& ymin, float& xmax, float& ymax);

bool FTGLTextureFontNew::CheckGlyph( const unsigned int characterCode)
{
	if( NULL == Glyph( characterCode))
	{
		unsigned int glyphIndex = FontIndex( characterCode);
		FTTextureGlyphNew* tempGlyph = MakeGlyph( glyphIndex);
		if( NULL == tempGlyph)
		{
			if( 0 == err)
			{
				err = 0x13;
			}

			return false;
		}
		
		charMap.insert( characterCode, glyphs.size());
		glyphs.push_back( tempGlyph);
	}

	return true;
}


unsigned int FTGLTextureFontNew::FontIndex( const unsigned int characterCode)
{
    return FT_Get_Char_Index( ftFace, characterCode);
}


const FTTextureGlyphNew* const FTGLTextureFontNew::Glyph( const unsigned int characterCode)
{
    signed int index = charMap.find(characterCode);
    return glyphs[index];
}


float FTGLTextureFontNew::Advance( const unsigned int characterCode, const unsigned int nextCharacterCode)
{
    unsigned int left = FontIndex(characterCode);
    unsigned int right = FontIndex(nextCharacterCode);

    //float width = KernAdvance(left, right).x;
    float xadvance, yadvance;
	KernAdvance(left, right, xadvance, yadvance);
	xadvance += Glyph(characterCode)->xadvance;

    return xadvance;
}

void FTGLTextureFontNew::KernAdvance(unsigned int index1, unsigned int index2, float& x, float& y)
{
    x=0.0f;
	y=0.0f;

	if( !!FT_HAS_KERNING(ftFace) && index1 && index2)
    {
        FT_Vector kernAdvance;
        kernAdvance.x = kernAdvance.y = 0;

        err = FT_Get_Kerning(ftFace, index1, index2, ft_kerning_unfitted, &kernAdvance);
        if( !err)
        {   
            x = static_cast<float>( kernAdvance.x) / 64.0f;
            y = static_cast<float>( kernAdvance.y) / 64.0f;
        }
    }
}

template<typename T>
void FTGLTextureFontNew::Render(const T* string)
{   
	glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE

	activeTextureID = 0;

	const T* c = string;

	//FTPoint pen;
	float xpos=0, ypos=0;

	while (*c)
	{
		if (CheckGlyph(*c))
		{
			//pen += Render(*c, *(c + 1), pen);
			//cache index
			unsigned int left = FontIndex(*c);
			unsigned int right = FontIndex(*(c+1));

			float xkern, ykern;
			KernAdvance(left, right, xkern, ykern);
        
			const FTTextureGlyphNew* glyph = Glyph(*c);
			if( activeTextureID != glyph->glTextureID)
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

			glBegin( GL_QUADS);
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
			//return FTPoint(xadvance, yadvance, 0);
		}
		++c;
	}

	glPopAttrib();
}

template void FTGLTextureFontNew::Render<char>(const char* string);
template void FTGLTextureFontNew::Render<wchar_t>(const wchar_t* string);
