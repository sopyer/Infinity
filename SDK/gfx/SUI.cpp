#include <gfx/gfx.h>
#include "gfx_res.h"

namespace vg
{
    using namespace impl;

    void drawRect(float x0, float y0, float x1, float y1, VGuint fillColor, VGuint borderColor)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        v128 fillColorF   = vi_cvt_ubyte4_to_vec4(fillColor),
            borderColorF = vi_cvt_ubyte4_to_vec4(borderColor);

        glUseProgram(simpleUIProgram);
        glUniform4fv(UNI_SIMPLE_UI_FILL_COLOR, 1, (float*)&fillColorF);
        glUniform4fv(UNI_SIMPLE_UI_BORDER_COLOR, 1, (float*)&borderColorF);
        glUniform2f (UNI_SIMPLE_UI_ZONES, 0, 0);

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, 0);

        glVertex2f( x0, y0);
        glVertex2f( x1, y0);

        glVertex2f( x0, y1);
        glVertex2f( x1, y1);
        glEnd();

        glUseProgram(0);
        glPopAttrib();
    }

    void drawRoundedRect(float x0, float y0, float x1, float y1, float cx, float cy, VGuint fillColor, VGuint borderColor)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        v128 fillColorF   = vi_cvt_ubyte4_to_vec4(fillColor),
            borderColorF = vi_cvt_ubyte4_to_vec4(borderColor);

        glUseProgram(simpleUIProgram);
        glUniform4fv(UNI_SIMPLE_UI_FILL_COLOR, 1, (float*)&fillColorF);
        glUniform4fv(UNI_SIMPLE_UI_BORDER_COLOR, 1, (float*)&borderColorF);
        glUniform2f (UNI_SIMPLE_UI_ZONES, cx - 1, cx - 2);

        float xb = cx;
        float yb = cy;

        float px0 = x0;
        float px1 = x0+cx;
        float px2 = x1-cx;
        float px3 = x1;

        float py0 = y0;
        float py1 = y0+cy;
        float py2 = y1-cy;
        float py3 = y1;

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(xb, yb);
        glVertex2f(px0, py0);
        glTexCoord2f(0, yb);
        glVertex2f(px1, py0);

        glTexCoord2f(xb, 0);
        glVertex2f(px0, py1);
        glTexCoord2f(0, 0);
        glVertex2f(px1, py1);

        glTexCoord2f(xb, 0);
        glVertex2f(px0, py2);
        glTexCoord2f(0, 0);
        glVertex2f(px1, py2);

        glTexCoord2f(xb, yb);
        glVertex2f(px0, py3);
        glTexCoord2f(0, yb);
        glVertex2f(px1, py3);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, yb);
        glVertex2f(px2, py0);
        glTexCoord2f(xb, yb);
        glVertex2f(px3, py0);

        glTexCoord2f(0, 0);
        glVertex2f(px2, py1);
        glTexCoord2f(xb, 0);
        glVertex2f(px3, py1);

        glTexCoord2f(0, 0);
        glVertex2f(px2, py2);
        glTexCoord2f(xb, 0);
        glVertex2f(px3, py2);

        glTexCoord2f(0, yb);
        glVertex2f(px2, py3);
        glTexCoord2f(xb, yb);
        glVertex2f(px3, py3);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, yb);
        glVertex2f(px1, py0);
        glTexCoord2f(0, yb);
        glVertex2f(px2, py0);

        glTexCoord2f(0, 0);
        glVertex2f(px1, py1);
        glTexCoord2f(0, 0);
        glVertex2f(px2, py1);

        glTexCoord2f(0, 0);
        glVertex2f(px1, py2);
        glTexCoord2f(0, 0);
        glVertex2f(px2, py2);

        glTexCoord2f(0, yb);
        glVertex2f(px1, py3);
        glTexCoord2f(0, yb);
        glVertex2f(px2, py3);
        glEnd();

        glUseProgram(0);
        glPopAttrib();
    }

    void drawRoundedRectOutline(float x0, float y0, float x1, float y1, float cx, float cy, VGuint borderColor)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        v128 translucentF = vi_cvt_ubyte4_to_vec4(0x00000000),
            borderColorF = vi_cvt_ubyte4_to_vec4(borderColor);

        glUseProgram(simpleUIProgram);
        glUniform4fv(UNI_SIMPLE_UI_FILL_COLOR, 1, (float*)&translucentF);
        glUniform4fv(UNI_SIMPLE_UI_BORDER_COLOR, 1, (float*)&borderColorF);
        glUniform2f (UNI_SIMPLE_UI_ZONES, cx - 1, cx - 2);

        float xb = cx;
        float yb = cy;

        float px0 = x0;
        float px1 = x0 + cx;
        float px2 = x1 - cx;
        float px3 = x1;

        float py0 = y0;
        float py1 = y0 + cy;
        float py2 = y1 - cy;
        float py3 = y1;

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(xb, yb);
        glVertex2f( px0, py0);
        glTexCoord2f(0, yb);
        glVertex2f( px1, py0);

        glTexCoord2f(xb, 0);
        glVertex2f( px0, py1);
        glTexCoord2f(0, 0);
        glVertex2f( px1, py1);

        glTexCoord2f(xb, 0);
        glVertex2f( px0, py2);
        glTexCoord2f(0, 0);
        glVertex2f( px1, py2);

        glTexCoord2f(xb, yb);
        glVertex2f( px0, py3);
        glTexCoord2f(0, yb);
        glVertex2f( px1, py3);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, yb);
        glVertex2f( px2, py0);
        glTexCoord2f(xb, yb);
        glVertex2f( px3, py0);

        glTexCoord2f(0, 0);
        glVertex2f( px2, py1);
        glTexCoord2f(xb, 0);
        glVertex2f( px3, py1);

        glTexCoord2f(0, 0);
        glVertex2f( px2, py2);
        glTexCoord2f(xb, 0);
        glVertex2f( px3, py2);

        glTexCoord2f(0, yb);
        glVertex2f( px2, py3);
        glTexCoord2f(xb, yb);
        glVertex2f( px3, py3);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, yb);
        glVertex2f( px1, py0);
        glTexCoord2f(0, yb);
        glVertex2f( px2, py0);

        glTexCoord2f(0, 0);
        glVertex2f( px1, py1);
        glTexCoord2f(0, 0);
        glVertex2f( px2, py1);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, 0);
        glVertex2f( px1, py2);
        glTexCoord2f(0, 0);
        glVertex2f( px2, py2);

        glTexCoord2f(0, yb);
        glVertex2f( px1, py3);
        glTexCoord2f(0, yb);
        glVertex2f( px2, py3);
        glEnd();

        glUseProgram(0);
        glPopAttrib();
    }

    //   void drawCircle( const Rect& rect, int fillColorId, int borderColorId );

    void drawImage(float x0, float y0, float x1, float y1, GLuint texture)
    {
        glUseProgram(0);
        glColor4f(1, 1, 1, 1);
        glEnable(GL_TEXTURE_2D);
        glBindMultiTextureEXT(GL_TEXTURE0, GL_TEXTURE_2D, texture);
        glMultiTexParameteriEXT(GL_TEXTURE0, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glMultiTexParameteriEXT(GL_TEXTURE0, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex2f(x0, y0);
            glTexCoord2f(1, 1); glVertex2f(x1, y0);
            glTexCoord2f(1, 0); glVertex2f(x1, y1);
            glTexCoord2f(0, 0); glVertex2f(x0, y1);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}
