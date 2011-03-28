#ifndef     __FTGLTextureFont__
#define     __FTGLTextureFont__

#define  WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <gl/glee.h>

#include <vector>
#include <hash_map>

class  GLFont
{
	public:
		GLFont(const char* fontFilePath, unsigned int size);
		GLFont(const unsigned char *pBufferBytes, size_t bufferSizeInBytes, unsigned int size);

		~GLFont();

		unsigned int FaceSize() const;

		float Ascender() const;
		float Descender() const;
		float LineHeight() const;

		template<typename T> void getBounds(const T* string, float& xmin, float& ymin, float& xmax, float& ymax);

		template<typename T> void Render(const T* string);


	private:
		struct GlyphData //: public FTTextureGlyph
		{
			float xadvance, yadvance; // The advance distance for this glyph

			float xmin, ymin, xmax, ymax; // The bounding box of this glyph.

			unsigned int width, height;
			float xoffset, yoffset;
			float u0, v0, u1, v1;
			int glTextureID;
		};

	private:
		typedef std::hash_map<unsigned int, GlyphData> GlyphMap;

		bool FaceSize(const unsigned int size, const unsigned int res = 72);

		void KernAdvance(unsigned int index1, unsigned int index2, float& x, float& y);

		GlyphData* getGlyphData(const unsigned int glyphIndex);

		inline GLuint CreateTexture();

	private:
		GLint activeTextureID;

		FT_Face ftFace;
		FT_Size ftSize;

		static const GLsizei textureWidth = 512;
		static const GLsizei textureHeight = 512;

		std::vector<GLuint> textureIDList;

		GlyphMap mGlyphs;

		int glyphHeight, glyphWidth;

		unsigned int padding; //A value to be added to the height and width to ensure that glyphs don't overlap in the texture
		unsigned int charSize;

		int xOffset, yOffset;
};

#endif // __FTGLTextureFont__


