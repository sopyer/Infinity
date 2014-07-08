#include <cassert>
#include <opengl.h>
#include <vector>
#include <hash_map>

#include "vg.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

namespace vg
{
    FT_Library  FreeTypeInstance = 0;

    struct GlyphData
    {
        float        xadvance, yadvance; // The advance distance for this glyph
        float        xmin, ymin, xmax, ymax; // The bounding box of this glyph.
        unsigned int width, height;
        float        xoffset, yoffset;
        float        u0, v0, u1, v1;
        int          glTextureID;
    };

    typedef std::hash_map<unsigned int, GlyphData> GlyphMap;

    struct  FontOpaque
    {
        GLint activeTextureID;

        FT_Face ftFace;
        FT_Size ftSize;

        std::vector<GLuint> textureIDList;
        GlyphMap            mGlyphs;

        int glyphHeight, glyphWidth;
        int xOffset, yOffset;

        unsigned int padding; //A value to be added to the height and width to ensure that glyphs don't overlap in the texture
        unsigned int charSize;
    };

    const FT_Long DEFAULT_FACE_INDEX = 0;
    const GLsizei TEXTURE_WIDTH      = 512;
    const GLsizei TEXTURE_HEIGHT     = 512;

    void initFontSubsystem()
    {
        FT_Error err = FT_Init_FreeType(&FreeTypeInstance);
        if (err)
        {
            FreeTypeInstance = 0;
        }
    }

    void shutdownFontSubsystem()
    {
        if (FreeTypeInstance)
        {
            FT_Done_FreeType(FreeTypeInstance);
        }
    }

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
            font->glyphWidth  = (fontFace->bbox.xMax-fontFace->bbox.xMin) * ((float)fontSize->metrics.x_ppem/(float)fontFace->units_per_EM);
            font->glyphHeight = (fontFace->bbox.yMax-fontFace->bbox.yMin) * ((float)fontSize->metrics.y_ppem/(float)fontFace->units_per_EM);
        }
        else
        {
            font->glyphWidth  = static_cast<float>(fontSize->metrics.max_advance)/64.0f;
            font->glyphHeight = static_cast<float>(fontSize->metrics.height)/64.0f;
        }

        return true;
    }

    GLuint createTexture()
    {   
        GLuint textID;
        glGenTextures(1, (GLuint*)&textID);

        glTextureParameteriEXT(textID, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTextureParameteriEXT(textID, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTextureParameteriEXT(textID, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteriEXT(textID, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTextureImage2DEXT(textID, GL_TEXTURE_2D, 0, GL_ALPHA, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

        return textID;
    }

    GlyphData* getGlyphData(Font font, const unsigned int glyphIndex)
    {
        GlyphMap::iterator it = font->mGlyphs.find(glyphIndex);
        GlyphData*         tempGlyph = (it!= font->mGlyphs.end())?&it->second:0;

        if (NULL==tempGlyph)
        {
            FT_Error     err = FT_Load_Glyph(font->ftFace, glyphIndex, FT_LOAD_NO_HINTING);
            FT_GlyphSlot ftGlyph = font->ftFace->glyph;

            if (!err && ftGlyph)
            {
                err = FT_Render_Glyph(ftGlyph, FT_RENDER_MODE_NORMAL);
                if (!err && ftGlyph->format == FT_GLYPH_FORMAT_BITMAP)
                {
                    if (font->textureIDList.empty())
                    {
                        font->textureIDList.push_back(createTexture());
                        font->xOffset = font->yOffset = font->padding;
                    }

                    if (font->xOffset > (TEXTURE_WIDTH-font->glyphWidth))
                    {
                        font->xOffset = font->padding;
                        font->yOffset += font->glyphHeight;

                        if (font->yOffset > (TEXTURE_HEIGHT - font->glyphHeight))
                        {
                            font->textureIDList.push_back(createTexture());
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

                        glTextureSubImage2DEXT(
                            tempGlyph->glTextureID,
                            GL_TEXTURE_2D, 0,
                            font->xOffset, font->yOffset,
                            tempGlyph->width, tempGlyph->height,
                            GL_ALPHA, GL_UNSIGNED_BYTE,
                            bitmap.buffer
                            );

                        glPopClientAttrib();
                    }

                    tempGlyph->u0 = (float)(font->xOffset)/(float)(TEXTURE_WIDTH);
                    tempGlyph->v0 = (float)(font->yOffset)/(float)(TEXTURE_HEIGHT);
                    tempGlyph->u1 = (float)(font->xOffset+tempGlyph->width) / (float)(TEXTURE_WIDTH);
                    tempGlyph->v1 = (float)(font->yOffset+tempGlyph->height) / (float)(TEXTURE_HEIGHT);

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
        Font font = 0;

        if (FreeTypeInstance)
        {
            Font font =  new FontOpaque;
            font->glyphHeight = 0;
            font->glyphWidth = 0;
            font->xOffset = 0;
            font->yOffset = 0;
            font->padding = 3;
            FT_New_Face(FreeTypeInstance, fontPath, DEFAULT_FACE_INDEX, &font->ftFace);
            initFaceSize(font, faceSize);
        }

        return font;
    }

    Font createFont(const unsigned char* fontData, size_t dataSize, size_t faceSize)
    {
        Font font = 0;

        if (FreeTypeInstance)
        {
            font =  new FontOpaque;
            font->glyphHeight = 0;
            font->glyphWidth = 0;
            font->xOffset = 0;
            font->yOffset = 0;
            font->padding = 3;
            FT_New_Memory_Face(FreeTypeInstance, (FT_Byte*)fontData, dataSize, DEFAULT_FACE_INDEX, &font->ftFace);
            initFaceSize(font, faceSize);
        }

        return font;
    }

    void destroyFont(Font font)
    {
        if (font)
        {
            FT_Done_Face(font->ftFace);

            if (!font->textureIDList.empty())
                glDeleteTextures(font->textureIDList.size(), (const GLuint*)&font->textureIDList[0]);

            delete font;
        }
    }

    template<typename T>
    void drawString(Font font, float x, float y, const T* str, size_t len)
    {
        if (!font) return;

        glUseProgram(0);

        glPushAttrib(GL_ENABLE_BIT|GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE

        font->activeTextureID = 0;

        unsigned int left = FT_Get_Char_Index(font->ftFace, *str);
        unsigned int right;

        while (len--)
        {
            ++str;
            right = FT_Get_Char_Index(font->ftFace, *str);

            if (GlyphData* glyph = getGlyphData(font, left))
            {
                float xkern, ykern;
                kernAdvance(font, left, right, xkern, ykern);

                if (font->activeTextureID != glyph->glTextureID)
                {
                    glBindMultiTextureEXT(GL_TEXTURE0, GL_TEXTURE_2D, (GLuint)glyph->glTextureID);
                    font->activeTextureID = glyph->glTextureID;
                }

                float u0 = glyph->u0,
                      v0 = glyph->v0,
                      u1 = glyph->u1,
                      v1 = glyph->v1;

                float px = x+glyph->xoffset,
                      py = y-glyph->yoffset,
                      w  = (float)glyph->width,
                      h  = (float)glyph->height;

                glBegin(GL_QUADS);
                glTexCoord2f(u0, v0);
                glVertex2f(px, py);

                glTexCoord2f(u0, v1);
                glVertex2f(px, py+h);

                glTexCoord2f(u1, v1);
                glVertex2f(w+px, py+h);

                glTexCoord2f(u1, v0);
                glVertex2f(w+px, py);
                glEnd();

                x += xkern+glyph->xadvance;
                y += ykern+glyph->yadvance;
            }

            left = right;
        }

        glPopAttrib();
    }

    template<> void drawString<char>(Font font, float x, float y, const char* str, size_t len)
    {
        drawString<unsigned char>(font, x, y, (const unsigned char*)str, len);
    }

    template void drawString<wchar_t>(Font font, float x, float y, const wchar_t* str, size_t len);

    //Optimize: advance>0, x always increases
    template<typename T>
    void getBounds(Font font, const T* str, float& xmin, float& ymin, float& xmax, float& ymax)
    {
#	undef min
#	undef max

        xmin=ymin=xmax=ymax=0;

        if (!font) return;

        if ((NULL!=str) && ('\0'!=*str))
        {
            float		advance = 0;

            unsigned int left =FT_Get_Char_Index(font->ftFace, *str);
            unsigned int right = FT_Get_Char_Index(font->ftFace, *++str);

            if (GlyphData* glyph = getGlyphData(font, left))
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

                if (GlyphData* glyph = getGlyphData(font, left))
                {
                    xmin = std::min(xmin, glyph->xmin+advance);
                    ymin = std::min(ymin, glyph->ymin);
                    xmax = std::max(xmax, glyph->xmax+advance);
                    ymax = std::max(ymax, glyph->ymax);

                    float xadvance, yadvance;

                    kernAdvance(font, left, right, xadvance, yadvance);
                    advance += xadvance+glyph->xadvance;
                }

                left = right;
            }

            xmax = std::max(xmax, advance);
        }
    }

    template void getBounds<char>(Font font, const char* string, float& xmin, float& ymin, float& xmax, float& ymax);
    template void getBounds<wchar_t>(Font font, const wchar_t* string, float& xmin, float& ymin, float& xmax, float& ymax);

    float getTextAscender(Font font)
    {
        if (!font) return 0;
        return  (float)font->ftSize->metrics.ascender/64.0f;
    }

    float getTextDescender(Font font)
    {
        if (!font) return 0;
        return (float)font->ftSize->metrics.descender/64.0f;
    }

    float getTextVExtent(Font font)
    {
        if (!font) return 0;
        return getTextAscender(font)-getTextDescender(font);
    }

    template<typename T>
    float getTextHExtent(Font font, const T* str)
    {
        if (!font) return 0;

        float	advance = 0;

        if ((NULL!=str) && ('\0'!=*str))
        {
            unsigned int left =FT_Get_Char_Index(font->ftFace, *str);

            while (*str++)
            {
                unsigned int right = FT_Get_Char_Index(font->ftFace, *str);

                if (GlyphData* glyph = getGlyphData(font, left))
                {
                    float xadvance, yadvance;

                    kernAdvance(font, left, right, xadvance, yadvance);
                    advance += xadvance+glyph->xadvance;
                }

                left = right;
            }
        }

        return advance;
    }

    template<> float getTextHExtent<char>(Font font, const char* str)
    {
        return getTextHExtent<unsigned char>(font, (const unsigned char*)str);
    }

    template float getTextHExtent<wchar_t>(Font font, const wchar_t* str);

    template<typename T>
    void calcTextBasePt(Font font, float w, float h, size_t flags, const T* str, float& bx, float& by)
    {
        bx = by = 0;

        if (!font) return;

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
}
