#ifndef     __FTTextureGlyph__
#define     __FTTextureGlyph__


#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_FREETYPE_H
#include FT_GLYPH_H

/**
 * FTTextureGlyph is a specialisation of FTTextureGlyph for creating texture
 * glyphs.
 * 
 * @see FTGlyphContainer
 *
 */
struct FTTextureGlyphNew //: public FTTextureGlyph
{
    // The advance distance for this glyph
    //FTPoint advance;
	float xadvance, yadvance;

    // The bounding box of this glyph.
    float xmin, ymin, xmax, ymax;
    /**
        * Constructor
        *
        * @param glyph     The Freetype glyph to be processed
        * @param id        The id of the texture that this glyph will be
        *                  drawn in
        * @param xOffset   The x offset into the parent texture to draw
        *                  this glyph
        * @param yOffset   The y offset into the parent texture to draw
        *                  this glyph
        * @param width     The width of the parent texture
        * @param height    The height (number of rows) of the parent texture
        */
    //FTTextureGlyph( FT_GlyphSlot glyph, int id, int xOffset, int yOffset, GLsizei width, GLsizei height);

    /**
        * The width of the glyph 'image'
        */

    //int destWidth;

    /**
        * The height of the glyph 'image'
        */
    //int destHeight;

    /**
        * Vector from the pen position to the topleft corner of the pixmap
        */
    //FTPoint pos;
    
	unsigned int width, height;
	float xoffset, yoffset;
	float u0, v0, u1, v1;
    /**
        * The texture co-ords of this glyph within the texture.
        */
    //FTPoint uv[2];
        
    /**
        * The texture index that this glyph is contained in.
        */
    int glTextureID;
};

FT_Error CreateTexGlyph(FTTextureGlyphNew& texglyph, FT_GlyphSlot glyph, int id, int xOffset, int yOffset, GLsizei width, GLsizei height);

#endif  //  __FTTextureGlyph__
