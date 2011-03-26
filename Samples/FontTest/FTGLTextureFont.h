#ifndef     __FTGLTextureFont__
#define     __FTGLTextureFont__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

//#include "FTPoint.h"
#include "FTCharToGlyphIndexMap.h"
#include <vector>

struct FTTextureGlyphNew;
class FTGlyphContainerNew;

class  FTGLTextureFontNew
{
	public:
		bool FaceSize( const unsigned int size, const unsigned int res = 72);

		unsigned int FaceSize() const;

		// Get the global ascender height for the face.
		float Ascender() const;

		// Gets the global descender height for the face.
		float Descender() const;

		// Gets the line spacing for the font.
		float LineHeight() const;

		// Get the bounding box for a string.
		template<typename T> void getBounds(const T* string, float& xmin, float& ymin, float& xmax, float& ymax);

		FT_Error Error() const { return err;}

		// Open and read a font file. Sets Error flag.
		FTGLTextureFontNew( const char* fontFilePath);

		// Open and read a font from a buffer in memory. Sets Error flag.
		FTGLTextureFontNew( const unsigned char *pBufferBytes, size_t bufferSizeInBytes);

		~FTGLTextureFontNew();

		template<typename T>
		void Render(const T* string);

        typedef std::vector<FTTextureGlyphNew*> GlyphVector;

		/**
		* Gets the kerning vector between two glyphs
		*/
		void KernAdvance(unsigned int index1, unsigned int index2, float& x, float& y);

		/**
         * Get the font index of the input character.
         *
         * @param characterCode The character code of the requested glyph in the
         *                      current encoding eg apple roman.
         * @return      The font index for the character.
         */

	private:
        unsigned int FontIndex( const unsigned int characterCode );
        
        /**
         * Get a glyph from the glyph list
         *
         * @param characterCode The char code of the glyph NOT the glyph index      
         * @return              An FTTextureGlyph or <code>null</code> is it hasn't been
         * loaded.
         */
        const FTTextureGlyphNew* const Glyph( const unsigned int characterCode);

        /**
        * Returns the kerned advance width for a glyph.
        *
        * @param characterCode     glyph index of the character
        * @param nextCharacterCode the next glyph in a string
        * @return                  advance width
        */
        float Advance( const unsigned int characterCode, const unsigned int nextCharacterCode);
        
        /**
         * Renders a character
         * @param characterCode      the glyph to be Rendered
         * @param nextCharacterCode  the next glyph in the string. Used for kerning.
         * @param penPosition        the position to Render the glyph
         * @return                   The distance to advance the pen position after Rendering
         */
        //FTPoint Render( const unsigned int characterCode, const unsigned int nextCharacterCode, FTPoint penPosition);
        
    private:
        typedef FTCharToGlyphIndexMap CharacterMap;
        CharacterMap charMap;

        /**
         * A structure to hold the glyphs
         */
        GlyphVector glyphs;

        /**
         * The texture index of the currently active texture
         *
         * We keep track of the currently active texture to try to reduce the number
         * of texture bind operations.
         */
        GLint activeTextureID;

		// Construct a glyph of the correct type.
		// @param g The glyph index NOT the char code.
		// @return  An FT****Glyph or <code>null</code> on failure.
		FTTextureGlyphNew* MakeGlyph( unsigned int g);

		FT_Face ftFace;
		int charSize;
		FT_Error err;

		FT_Size ftSize;
		bool hasKerningTable;

		/**
		* Check that the glyph at <code>chr</code> exist. If not load it.
		*
		* @param chr  character index
		* @return <code>true</code> if the glyph can be created.
		*/
		inline bool CheckGlyph( const unsigned int chr);

		/**
		* Creates a 'blank' OpenGL texture object.
		*
		* The format is GL_ALPHA and the params are
		* GL_TEXTURE_WRAP_S = GL_CLAMP
		* GL_TEXTURE_WRAP_T = GL_CLAMP
		* GL_TEXTURE_MAG_FILTER = GL_LINEAR
		* GL_TEXTURE_MIN_FILTER = GL_LINEAR
		* Note that mipmapping is NOT used
		*/
		inline GLuint CreateTexture();

		GLsizei textureWidth;
		GLsizei textureHeight;
		std::vector<GLuint> textureIDList;

		/**
		* The max height for glyphs in the current font
		*/
		int glyphHeight;

		/**
		* The max width for glyphs in the current font
		*/
		int glyphWidth;

		/**
		* A value to be added to the height and width to ensure that
		* glyphs don't overlap in the texture
		*/
		unsigned int padding;

		unsigned int numGlyphs;

		unsigned int remGlyphs;
		int xOffset;
		int yOffset;
};


#endif // __FTGLTextureFont__


