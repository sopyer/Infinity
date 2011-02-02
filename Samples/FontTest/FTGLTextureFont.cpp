#include <cassert>

#include "FTGLTextureFont.h"
#include "FTTextureGlyph.h"
#include "FTGlyphContainer.h"
#include "FTBBox.h"
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

FTGLTextureFont::FTGLTextureFont( const char* fontFilePath)
:	textureWidth(0), textureHeight(0),
	glyphHeight(0),	glyphWidth(0),
	xOffset(0),	yOffset(0),
	padding(3)
{
    err = FT_New_Face( *FTLibrary::Instance().GetLibrary(), fontFilePath, DEFAULT_FACE_INDEX, &ftFace);
}

FTGLTextureFont::FTGLTextureFont( const unsigned char *pBufferBytes, size_t bufferSizeInBytes)
:	textureWidth(0), textureHeight(0),
	glyphHeight(0), glyphWidth(0),
	xOffset(0),	yOffset(0),
	padding(3)
{
    err = FT_New_Memory_Face( *FTLibrary::Instance().GetLibrary(), (FT_Byte *)pBufferBytes, bufferSizeInBytes, DEFAULT_FACE_INDEX, &ftFace);
}


FTGLTextureFont::~FTGLTextureFont()
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


FTTextureGlyph* FTGLTextureFont::MakeGlyph( unsigned int glyphIndex)
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

		FTTextureGlyph* tempGlyph = new FTTextureGlyph;
		CreateTexGlyph(*tempGlyph, ftGlyph, textureIDList[textureIDList.size() - 1],	xOffset, yOffset, textureWidth, textureHeight);
#ifdef _FTGL_NATIVE_
		xOffset += static_cast<int>( tempGlyph->BBox().upperX - tempGlyph->BBox().lowerX + padding);
#else
		xOffset += static_cast<int>( tempGlyph->bBox.upper.x - tempGlyph->bBox.lower.x + padding);
#endif

		--remGlyphs;
		return tempGlyph;
	}

	return NULL;
}


GLuint FTGLTextureFont::CreateTexture()
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


bool FTGLTextureFont::FaceSize( const unsigned int size, const unsigned int res)
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

	err = FT_Select_Charmap(ftFace, FT_ENCODING_UNICODE );
	glyphs.push_back( NULL);

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


unsigned int FTGLTextureFont::FaceSize() const
{
	return charSize;
}


float FTGLTextureFont::Ascender() const
{
	assert(ftSize!=0);
    return  (float)ftSize->metrics.ascender / 64.0f;
}


float FTGLTextureFont::Descender() const
{
	assert(ftSize!=0);
    return (float)ftSize->metrics.descender / 64.0f;
}


float FTGLTextureFont::LineHeight() const
{
	return glyphHeight;
}


unsigned int unsignedExpand(const char c) {return (unsigned char) c;}
unsigned int unsignedExpand(const wchar_t c) {return c;}


template<typename T>
void FTGLTextureFont::BBox( const T* string,
	float& llx, float& lly, float& urx, float& ury)
{
	FTBBox totalBBox;
	bool	applyAdvance = false;

	if((NULL != string) && ('\0' != *string))
	{
		const T* c = string;
		float advance = 0;

		if(CheckGlyph( *c))
		{
			totalBBox = Glyph(*c)->bBox;
			advance = Advance( *c, *(c + 1));
		}

		while( *++c)
		{
			if(CheckGlyph( *c))
			{
				FTBBox tempBBox = Glyph(*c)->bBox;
				tempBBox.Move( FTPoint( advance, 0.0f, 0.0f));
				totalBBox += tempBBox;
				advance += Advance( *c, *(c + 1));
				applyAdvance = tempBBox.IsEmpty();
			}
		}

		//Deal with spaces at the end of string, for spaces BBox.IsEmpty()==true and advance is not apply
		//That's why we manually do this:
		if (applyAdvance)
		{
			FTBBox tempBBox;
			tempBBox.Move(FTPoint( advance, 0.0f, 0.0f));
			totalBBox += tempBBox;
		}
	}

	llx = totalBBox.lower.x;
	lly = totalBBox.lower.y;
	urx = totalBBox.upper.x;
	ury = totalBBox.upper.y;
}

template void FTGLTextureFont::BBox<char>( const char* string, float& llx, float& lly, float& urx, float& ury);
template void FTGLTextureFont::BBox<wchar_t>( const wchar_t* string, float& llx, float& lly, float& urx, float& ury);


bool FTGLTextureFont::CheckGlyph( const unsigned int characterCode)
{
	if( NULL == Glyph( characterCode))
	{
		unsigned int glyphIndex = FontIndex( characterCode);
		FTTextureGlyph* tempGlyph = MakeGlyph( glyphIndex);
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


unsigned int FTGLTextureFont::FontIndex( const unsigned int characterCode)
{
    return FT_Get_Char_Index( ftFace, characterCode);
}


const FTTextureGlyph* const FTGLTextureFont::Glyph( const unsigned int characterCode)
{
    signed int index = charMap.find(characterCode);
    return glyphs[index];
}


float FTGLTextureFont::Advance( const unsigned int characterCode, const unsigned int nextCharacterCode)
{
    unsigned int left = FontIndex(characterCode);
    unsigned int right = FontIndex(nextCharacterCode);

    float width = KernAdvance(left, right).x;
	width += Glyph(characterCode)->advance.x;

    return width;
}

FTPoint FTGLTextureFont::KernAdvance( unsigned int index1, unsigned int index2)
{
    float x=0.0f, y=0.0f;

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
    
    return FTPoint( x, y, 0.0);
}

template<typename T>
void FTGLTextureFont::Render(const T* string)
{   
	glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE

	activeTextureID = 0;

	const T* c = string;

	FTPoint pen;

	while (*c)
	{
		if (CheckGlyph(*c))
		{
			pen += Render(*c, *(c + 1), pen);
		}
		++c;
	}

	glPopAttrib();
}

template void FTGLTextureFont::Render<char>(const char* string);
template void FTGLTextureFont::Render<wchar_t>(const wchar_t* string);

FTPoint FTGLTextureFont::Render( const unsigned int characterCode, const unsigned int nextCharacterCode, FTPoint penPosition)
{
    FTPoint kernAdvance, advance;
    
    unsigned int left = FontIndex( characterCode);
    unsigned int right = FontIndex( nextCharacterCode);

    kernAdvance = KernAdvance( left, right);
        
    {
		const FTTextureGlyph* glyph = Glyph(characterCode);
		if( activeTextureID != glyph->glTextureID)
		{
			glBindTexture( GL_TEXTURE_2D, (GLuint)glyph->glTextureID);
			activeTextureID = glyph->glTextureID;
		}

		float	u0 = glyph->uv[0].x,
				v0 = glyph->uv[0].y,
				u1 = glyph->uv[1].x,
				v1 = glyph->uv[1].y;

		float	px = penPosition.x+glyph->pos.x,
				py = penPosition.y+glyph->pos.y,
				w  = (float)glyph->destWidth,
				h  = (float)glyph->destHeight;

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

		advance = glyph->advance;
    }
    
    kernAdvance += advance;
    return kernAdvance;
}
