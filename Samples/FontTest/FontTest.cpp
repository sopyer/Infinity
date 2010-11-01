#include <ft2build.h>
#include FT_FREETYPE_H

int main()
{
	FT_Library	library;
    FT_Face		face;

    FT_Error error = FT_Init_FreeType(&library);

    error = FT_New_Face(library, "C:\\WINDOWS\\Fonts\\times.ttf", 0, &face);

	//FT_Load_Glyph();
	//FT_Load_Char();
	//FT_OutlineGlyph glyph;
	//FT_Get_Glyph();
	//FT_Outline_Decompose();//Rewrite to my advantage
	//FT_Done_Glyph();

	FT_Done_Face(face);

	error = FT_Done_FreeType(library);

	return 0;
}