#include <gl/glee.h>
#include <vi.h>
#include "vg.h"
#include "impl/SharedResources.h"

namespace vg
{
	using namespace impl;

#define norm255( i ) ( (float) ( i ) / 255.0f )

	const static float colors[cNbColors][4] =
	{
		// cBase
		{ norm255(89), norm255(89), norm255(89), 0.7f },
		{ norm255(166), norm255(166), norm255(166), 0.8f },
		{ norm255(212), norm255(228), norm255(60), 0.5f },
		{ norm255(227), norm255(237), norm255(127), 0.5f },

		// cBool
		{ norm255(99), norm255(37), norm255(35), 1.0f },
		{ norm255(149), norm255(55), norm255(53), 1.0f },
		{ norm255(212), norm255(228), norm255(60), 1.0f },
		{ norm255(227), norm255(237), norm255(127), 1.0f },

		// cOutline
		{ norm255(255), norm255(255), norm255(255), 1.0f },
		{ norm255(255), norm255(255), norm255(255), 1.0f },
		{ norm255(255), norm255(255), norm255(255), 1.0f },
		{ norm255(255), norm255(255), norm255(255), 1.0f },

		// cFont
		{ norm255(255), norm255(255), norm255(255), 1.0f },
		{ norm255(255), norm255(255), norm255(255), 1.0f },
		{ norm255(255), norm255(255), norm255(255), 1.0f },
		{ norm255(255), norm255(255), norm255(255), 1.0f },

		// cFontBack
		{ norm255(79), norm255(129), norm255(189), 1.0 },
		{ norm255(79), norm255(129), norm255(189), 1.0 },
		{ norm255(128), norm255(100), norm255(162), 1.0 },
		{ norm255(128), norm255(100), norm255(162), 1.0 },
	    
		// cTranslucent
		{ norm255(0), norm255(0), norm255(0), 0.0 },
		{ norm255(0), norm255(0), norm255(0), 0.0 },
		{ norm255(0), norm255(0), norm255(0), 0.0 },
		{ norm255(0), norm255(0), norm255(0), 0.0 },
	};

	const static uint32_t colorsUB[cNbColors] =
	{
		// cBase
		//{ norm255(89), norm255(89), norm255(89), 0.7f },
		//{ norm255(166), norm255(166), norm255(166), 0.8f },
		//{ norm255(212), norm255(228), norm255(60), 0.5f },
		//{ norm255(227), norm255(237), norm255(127), 0.5f },
		0xB2595959,
		0xCCA6A6A6,
		0x7F3CE4D4,
		0x7F7FEDE3,

		// cBool
		//{ norm255(99), norm255(37), norm255(35), 1.0f },
		//{ norm255(149), norm255(55), norm255(53), 1.0f },
		//{ norm255(212), norm255(228), norm255(60), 1.0f },
		//{ norm255(227), norm255(237), norm255(127), 1.0f },
		0xFF232563,
		0xFF353795,
		0xFF3CE4D4,
		0xFF7FEDE3,

		// cOutline
		//{ norm255(255), norm255(255), norm255(255), 1.0f },
		//{ norm255(255), norm255(255), norm255(255), 1.0f },
		//{ norm255(255), norm255(255), norm255(255), 1.0f },
		//{ norm255(255), norm255(255), norm255(255), 1.0f },
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,

		// cFont
		//{ norm255(255), norm255(255), norm255(255), 1.0f },
		//{ norm255(255), norm255(255), norm255(255), 1.0f },
		//{ norm255(255), norm255(255), norm255(255), 1.0f },
		//{ norm255(255), norm255(255), norm255(255), 1.0f },
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,

		// cFontBack
		//{ norm255(79), norm255(129), norm255(189), 1.0 },
		//{ norm255(79), norm255(129), norm255(189), 1.0 },
		//{ norm255(128), norm255(100), norm255(162), 1.0 },
		//{ norm255(128), norm255(100), norm255(162), 1.0 },
	    0xFFBD814F,
		0xFFBD814F,
		0xFFA26480,
		0xFFA26480,

		// cTranslucent
		//{ norm255(0), norm255(0), norm255(0), 0.0 },
		//{ norm255(0), norm255(0), norm255(0), 0.0 },
		//{ norm255(0), norm255(0), norm255(0), 0.0 },
		//{ norm255(0), norm255(0), norm255(0), 0.0 },
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
	};

	void drawRectCOLORID(float x0, float y0, float x1, float y1, int fillColor, int borderColor)
	{
		drawRect(x0, y0, x1, y1, colorsUB[fillColor], colorsUB[borderColor]);
	}

	void drawRect(float x0, float y0, float x1, float y1, VGuint fillColor, VGuint borderColor)
	{
		glUseProgram(programs[PRG_SIMPLE_UI]);

		vec4 fillColorF = vi_cvt_ubyte4_to_vec4(fillColor), borderColorF=vi_cvt_ubyte4_to_vec4(borderColor);

		glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, (float*)&fillColorF);
		glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, (float*)&borderColorF);
		glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], 0, 0);

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0, 0);

			glVertex2f( x0, y0);
			glVertex2f( x1, y0);

			glVertex2f( x0, y1);
			glVertex2f( x1, y1);
		glEnd();

		glUseProgram(0);
	}

    void drawRoundedRectCOLORID(
		float x0, float y0, float x1, float y1, float cx, float cy,
		int fillColorId, int borderColorId
	)
	{
		drawRoundedRect(x0, y0, x1, y1, cx, cy, colorsUB[fillColorId], colorsUB[borderColorId]); 
	}

	void drawRoundedRect(float x0, float y0, float x1, float y1, float cx, float cy, VGuint fillColor, VGuint borderColor)
	{
		vec4 fillColorF = vi_cvt_ubyte4_to_vec4(fillColor), borderColorF=vi_cvt_ubyte4_to_vec4(borderColor);

		glUseProgram(programs[PRG_SIMPLE_UI]);
		glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, (float*)&fillColorF);
		glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, (float*)&borderColorF);
		glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], cx - 1, cx - 2);

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
	}

 	void drawFrame(
		float x0, float y0, float x1, float y1, float cx, float cy,
		bool isHover, bool isOn, bool /*isFocus*/
	)
	{
		int lColorNb = cBase + (isHover) + (isOn << 1);// + (isFocus << 2);

		if (cx+cy == 0)
			drawRectCOLORID(x0, y0, x1, y1, lColorNb, cOutline);
		else
			drawRoundedRectCOLORID(x0, y0, x1, y1, cx, cy, lColorNb, cOutline);
	}

	void drawBoolFrame(
		float x0, float y0, float x1, float y1, float cx, float cy,
		bool isHover, bool isOn, bool /*isFocus*/
	)
	{
		int lColorNb = cBool + (isHover) + (isOn << 1);// + (isFocus << 2);
		    
		drawRoundedRectCOLORID(x0, y0, x1, y1, cx, cy, lColorNb, cOutline );
	}

	void drawRoundedRectOutlineCOLORID(float x0, float y0, float x1, float y1, float cx, float cy, int borderColor)
	{
		drawRoundedRectOutline(x0, y0, x1, y1, cx, cy, colorsUB[borderColor]);
	}

	void drawRoundedRectOutline(float x0, float y0, float x1, float y1, float cx, float cy, VGuint borderColor)
	{
		vec4 translucentF = vi_cvt_ubyte4_to_vec4(colorsUB[cTranslucent]), borderColorF=vi_cvt_ubyte4_to_vec4(borderColor);

		glUseProgram(programs[PRG_SIMPLE_UI]);
		glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, (float*)&translucentF);
		glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, (float*)&borderColorF);
		glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], cx - 1, cx - 2);

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
	}
//   void drawCircle( const Rect& rect, int fillColorId, int borderColorId );
//   void drawMinus( const Rect& rect, int width, int fillColorId, int borderColorId );
//   void drawPlus( const Rect& rect, int width, int fillColorId, int borderColorId );
//   void drawDownArrow( const Rect& rect, int width, int fillColorId, int borderColorId );
//   void drawUpArrow( const Rect& rect, int width, int fillColorId, int borderColorId );
}
