#include    "FTTextureGlyph.h"

FT_Error CreateTexGlyph(FTTextureGlyph& texglyph, FT_GlyphSlot glyph, int id, int xOffset, int yOffset, GLsizei width, GLsizei height)
{
	texglyph.glTextureID = id;
	if( glyph)
	{
		texglyph.bBox = FTBBox( glyph);
		texglyph.advance = FTPoint( glyph->advance.x / 64.0f, glyph->advance.y / 64.0f, 0.0f);
	}

	FT_Error err = FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL);
	if( err || glyph->format != ft_glyph_format_bitmap)
	{
		return err;
	}

	FT_Bitmap      bitmap = glyph->bitmap;

	texglyph.destWidth  = bitmap.width;
	texglyph.destHeight = bitmap.rows;

	if( texglyph.destWidth && texglyph.destHeight)
	{
		glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);
		glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE);
		glPixelStorei( GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1);

		glBindTexture( GL_TEXTURE_2D, texglyph.glTextureID);
		glTexSubImage2D( GL_TEXTURE_2D, 0, xOffset, yOffset, texglyph.destWidth, texglyph.destHeight, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.buffer);

		glPopClientAttrib();
	}


	//      0    
	//      +----+
	//      |    |
	//      |    |
	//      |    |
	//      +----+
	//           1

#ifdef _FTGL_NATIVE_
	uv[0].X( static_cast<float>(xOffset) / static_cast<float>(width));
	uv[0].Y( static_cast<float>(yOffset) / static_cast<float>(height));
	uv[1].X( static_cast<float>( xOffset + destWidth) / static_cast<float>(width));
	uv[1].Y( static_cast<float>( yOffset + destHeight) / static_cast<float>(height));

	pos.X( glyph->bitmap_left);
	pos.Y( glyph->bitmap_top);
#else
	texglyph.uv[0].x = static_cast<float>(xOffset) / static_cast<float>(width);
	texglyph.uv[0].y = static_cast<float>(yOffset) / static_cast<float>(height);
	texglyph.uv[1].x = static_cast<float>( xOffset + texglyph.destWidth) / static_cast<float>(width);
	texglyph.uv[1].y = static_cast<float>( yOffset + texglyph.destHeight) / static_cast<float>(height);

	texglyph.pos.x = (float)glyph->bitmap_left;
	texglyph.pos.y = (float)glyph->bitmap_top;
#endif

	return err;
}
