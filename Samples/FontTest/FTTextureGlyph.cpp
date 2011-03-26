#include    "FTTextureGlyph.h"

FT_Error CreateTexGlyph(FTTextureGlyphNew& texglyph, FT_GlyphSlot glyph, int id, int xOffset, int yOffset, GLsizei width, GLsizei height)
{
	texglyph.glTextureID = id;
	if( glyph)
	{
        FT_BBox bbox;
        FT_Outline_Get_CBox( &(glyph->outline), &bbox);

		texglyph.xmin = static_cast<float>( bbox.xMin) / 64.0f;
        texglyph.ymin = static_cast<float>( bbox.yMin) / 64.0f;
        texglyph.xmax = static_cast<float>( bbox.xMax) / 64.0f;
        texglyph.ymax = static_cast<float>( bbox.yMax) / 64.0f;

		texglyph.xadvance = glyph->advance.x / 64.0f;
		texglyph.yadvance = glyph->advance.y / 64.0f;
	}

	FT_Error err = FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL);
	if( err || glyph->format != ft_glyph_format_bitmap)
	{
		return err;
	}

	FT_Bitmap      bitmap = glyph->bitmap;

	texglyph.width  = bitmap.width;
	texglyph.height = bitmap.rows;

	if( texglyph.width && texglyph.height)
	{
		glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);
		glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE);
		glPixelStorei( GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1);

		glBindTexture( GL_TEXTURE_2D, texglyph.glTextureID);
		glTexSubImage2D( GL_TEXTURE_2D, 0, xOffset, yOffset, texglyph.width, texglyph.height, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.buffer);

		glPopClientAttrib();
	}

	texglyph.u0 = static_cast<float>(xOffset) / static_cast<float>(width);
	texglyph.v0 = static_cast<float>(yOffset) / static_cast<float>(height);
	texglyph.u1 = static_cast<float>( xOffset + texglyph.width) / static_cast<float>(width);
	texglyph.v1 = static_cast<float>( yOffset + texglyph.height) / static_cast<float>(height);

	texglyph.xoffset = (float)glyph->bitmap_left;
	texglyph.yoffset = (float)glyph->bitmap_top;

	return err;
}
