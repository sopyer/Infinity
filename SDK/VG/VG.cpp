#include "gl/glee.h"
#include "VG.h"
#include "SharedResources.h"
using namespace impl;

//strange staff fix it!!!!!!!
//#define _USE_MATH_DEFINES
#include <cmath>
#define M_PI       3.14159265358979323846

namespace vg
{
	void init()
	{
		acquire();
	}

	void cleanup()
	{
		release();
	}
}


void VG::begin()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//setOrthoProj();
	setPerspectiveProj();

	//Make window coordinate system y axis points down
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(- (mWidth / 2.0f), mHeight / 2.0f, 0.0f);
	//glScalef(1.0f, -1.0f, 1.0f);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
            
    // fill mode always
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   
    // Stencil / Depth buffer and test disabled
	//glDisable(GL_STENCIL_TEST);
	//glStencilMask( 0 );
	//glDisable(GL_DEPTH_TEST);
	//glDepthMask( GL_FALSE );
	
	//glDisable(GL_STENCIL_TEST);
	//glStencilMask( 0 );
	//glDisable(GL_DEPTH_TEST);
	//glDepthMask( GL_FALSE );
	// Blend on for alpha
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Color active
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
}

void VG::end()
{
	glPopAttrib();
	glPopClientAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void VG::drawText(FTFont& font, GLint x, GLint y, int flags, const char* text)
{
	glm::ivec2 extent = getTextExtent(font, text);
	switch (flags & TextAlign::HMask)
	{
		case TextAlign::Left:
			break;
		case TextAlign::HCenter:
			x -= extent.x/2;
			break;
		case TextAlign::Right:
			x -= extent.x;
			break;
		default:
			return;
	}

	switch (flags & TextAlign::VMask)
	{
		case TextAlign::Top:
			y += extent.y;
			break;
		case TextAlign::VCenter:
			y += extent.y / 2;
			break;
		case TextAlign::Bottom:
			break;
		default:
			return;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)y, 0);
	glScalef(1.0f, -1.0f, 1.0f);//It's hack to compansate text rendering algorithm

	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	
	font.Render(text);
	
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);

	glPopMatrix();
}

glm::ivec2 VG::getTextExtent(FTFont& font, const char* text)
{
	float x1, y1, z1, x2, y2, z2;
	font.BBox(text, x1, y1, z1, x2, y2, z2);

	return glm::ivec2(ceil(x2 - x1), ceil(y2 - y1));
}

void VG::drawRect(glm::vec4 color, glm::ivec2 p1, glm::ivec2 p2)
{
	glColor3fv(color);
	glBegin(GL_LINE_STRIP);
		glVertex2iv(p1);
		glVertex2i(p2.x, p1.y);
		glVertex2iv(p2);
		glVertex2i(p1.x, p2.y);
		glVertex2iv(p1);
	glEnd();
}


void VG::setOrthoProj()
{
	int w = mWidth;
	int h = mHeight;
	GLdouble aspect = (GLdouble)w / (GLdouble)h;

	glViewport(0, 0, w, h);
	// We are going to do some 2-D orthographic drawing.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble size = (GLdouble)((w >= h) ? w : h) / 2.0;

	if (w <= h)
	{
		aspect = (GLdouble)h/(GLdouble)w;
		glOrtho(-size, size, -size*aspect, size*aspect, -100000.0, 100000.0);
	}
	else
	{
		aspect = (GLdouble)w/(GLdouble)h;
		glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
	}
	// Make the world and window coordinates coincide so that 1.0 in
	// model space equals one pixel in window space.
	glScaled(aspect, aspect, 1.0);
	// Now determine where to draw things.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void VG::setPerspectiveProj()
{
	int width = mWidth;
	int height = mHeight;

	glViewport(0, 0, width, height);
	
	// setup matrices: see clutter_setup_viewport for reference
	float fovy = 60.0f;
	float zNear = 0.1f, zFar = (float)width;
	float aspect = 1;//(float)width / height;

	glm::mat4 matProj = glm::perspectiveGTX(fovy, aspect, zNear, zFar);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(matProj);

	float	z_camera = 0.5f * matProj[0][0];

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.5f, -0.5f, -z_camera);  //center of coords in top left corner
	//glTranslatef(0.0f, -1.0f, -z_camera);	//center of coords in center of screen
	glScalef(1.0f / width,
			 -1.0f / height,
			  1.0f / width);
	glTranslatef(0.0f, -1.0f * height, 0.0f);
}

#define norm255( i ) ( (float) ( i ) / 255.0f )

const char* cWidgetVSSource = {
    "																				\n\
    void main()																		\n\
    {																				\n\
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;						\n\
        gl_TexCoord[0] = gl_MultiTexCoord0;											\n\
    }																				\n\
    "
};

// @@ IC: Use standard GLSL. Do not initialize uniforms.
	
const char* cWidgetFSSource = {
    "																				\n\
    uniform vec4 uFillColor;														\n\
    uniform vec4 uBorderColor;														\n\
    uniform vec2 uZones;															\n\
																					\n\
    void main()																		\n\
    {																				\n\
        float doTurn       = float(gl_TexCoord[0].y>0);								\n\
        float radiusOffset = doTurn*abs(gl_TexCoord[0].z);							\n\
        float turnDir      = sign(gl_TexCoord[0].z);								\n\
        vec2  uv = vec2(gl_TexCoord[0].x+turnDir*radiusOffset, gl_TexCoord[0].y);	\n\
        float l  = abs(length(uv) - radiusOffset);									\n\
        float a  = clamp(l - uZones.x, 0.0, 2.0);									\n\
        float b  = clamp(l - uZones.y, 0.0, 2.0);									\n\
        b = exp2(-2.0*b*b);															\n\
        gl_FragColor = (uFillColor*b + (1.0-b)*uBorderColor);						\n\
        gl_FragColor.a *= exp2(-2.0*a*a);											\n\
    }																				\n\
    "
};

const static float s_colors[cNbColors][4] =
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

void VG::drawRoundedRect( const Rect& rect, const Point& corner, int fillColorId, int borderColorId )
{
    glUseProgram(programs[PRG_SIMPLE_UI]);
	glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, s_colors[fillColorId]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, s_colors[borderColorId]);
    glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], corner.x - 1, corner.x - 2);

    float xb = corner.x;
    float yb = corner.y;

    float x0 = rect.x;
    float x1 = rect.x + corner.x;
    float x2 = rect.x + rect.w - corner.x;
    float x3 = rect.x + rect.w;
    
    float y0 = rect.y;
    float y1 = rect.y + corner.y;
    float y2 = rect.y + rect.h - corner.y;
    float y3 = rect.y + rect.h;

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(xb, yb);
        glVertex2f( x0, y0);
        glTexCoord2f(0, yb);
        glVertex2f( x1, y0);

        glTexCoord2f(xb, 0);
        glVertex2f( x0, y1);
        glTexCoord2f(0, 0);
        glVertex2f( x1, y1);

        glTexCoord2f(xb, 0);
        glVertex2f( x0, y2);
        glTexCoord2f(0, 0);
        glVertex2f( x1, y2);

        glTexCoord2f(xb, yb);
        glVertex2f( x0, y3);
        glTexCoord2f(0, yb);
        glVertex2f( x1, y3);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, yb);
        glVertex2f( x2, y0);
        glTexCoord2f(xb, yb);
        glVertex2f( x3, y0);

        glTexCoord2f(0, 0);
        glVertex2f( x2, y1);
        glTexCoord2f(xb, 0);
        glVertex2f( x3, y1);

        glTexCoord2f(0, 0);
        glVertex2f( x2, y2);
        glTexCoord2f(xb, 0);
        glVertex2f( x3, y2);
 
        glTexCoord2f(0, yb);
        glVertex2f( x2, y3);
        glTexCoord2f(xb, yb);
        glVertex2f( x3, y3);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, yb);
        glVertex2f( x1, y0);
        glTexCoord2f(0, yb);
        glVertex2f( x2, y0);

        glTexCoord2f(0, 0);
        glVertex2f( x1, y1);
        glTexCoord2f(0, 0);
        glVertex2f( x2, y1);

        glTexCoord2f(0, 0);
        glVertex2f( x1, y2);
        glTexCoord2f(0, 0);
        glVertex2f( x2, y2);
 
        glTexCoord2f(0, yb);
        glVertex2f( x1, y3);
        glTexCoord2f(0, yb);
        glVertex2f( x2, y3);
    glEnd();

    glUseProgram(0);
}

void VG::drawRoundedRectOutline( const Rect& rect, const Point& corner, int borderColorId )
{
    glUseProgram(programs[PRG_SIMPLE_UI]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, s_colors[cTranslucent]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, s_colors[borderColorId]);
    glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], corner.x - 1, corner.x - 2);

    float xb = corner.x;
    float yb = corner.y;

    float x0 = rect.x;
    float x1 = rect.x + corner.x;
    float x2 = rect.x + rect.w - corner.x;
    float x3 = rect.x + rect.w;
    
    float y0 = rect.y;
    float y1 = rect.y + corner.y;
    float y2 = rect.y + rect.h - corner.y;
    float y3 = rect.y + rect.h;

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(xb, yb);
        glVertex2f( x0, y0);
        glTexCoord2f(0, yb);
        glVertex2f( x1, y0);

        glTexCoord2f(xb, 0);
        glVertex2f( x0, y1);
        glTexCoord2f(0, 0);
        glVertex2f( x1, y1);

        glTexCoord2f(xb, 0);
        glVertex2f( x0, y2);
        glTexCoord2f(0, 0);
        glVertex2f( x1, y2);

        glTexCoord2f(xb, yb);
        glVertex2f( x0, y3);
        glTexCoord2f(0, yb);
        glVertex2f( x1, y3);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, yb);
        glVertex2f( x2, y0);
        glTexCoord2f(xb, yb);
        glVertex2f( x3, y0);

        glTexCoord2f(0, 0);
        glVertex2f( x2, y1);
        glTexCoord2f(xb, 0);
        glVertex2f( x3, y1);

        glTexCoord2f(0, 0);
        glVertex2f( x2, y2);
        glTexCoord2f(xb, 0);
        glVertex2f( x3, y2);
 
        glTexCoord2f(0, yb);
        glVertex2f( x2, y3);
        glTexCoord2f(xb, yb);
        glVertex2f( x3, y3);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, yb);
        glVertex2f( x1, y0);
        glTexCoord2f(0, yb);
        glVertex2f( x2, y0);

        glTexCoord2f(0, 0);
        glVertex2f( x1, y1);
        glTexCoord2f(0, 0);
        glVertex2f( x2, y1);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, 0);
        glVertex2f( x1, y2);
        glTexCoord2f(0, 0);
        glVertex2f( x2, y2);
 
        glTexCoord2f(0, yb);
        glVertex2f( x1, y3);
        glTexCoord2f(0, yb);
        glVertex2f( x2, y3);
    glEnd();

    glUseProgram(0);
}

void VG::drawCircle( const Rect& rect, int fillColorId, int borderColorId )
{
	glUseProgram(programs[PRG_SIMPLE_UI]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, s_colors[fillColorId]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, s_colors[borderColorId]);
    glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], (rect.w / 2) - 1, (rect.w / 2) - 2);


    float xb = rect.w / 2;
    float yb = rect.w / 2;

    float x0 = rect.x;
    float x1 = rect.x + rect.w;

    float y0 = rect.y;
    float y1 = rect.y + rect.h;

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(-xb, -yb);
        glVertex2f( x0, y0);
        glTexCoord2f(xb, -yb);
        glVertex2f( x1, y0);
        glTexCoord2f(-xb, yb);
        glVertex2f( x0, y1);
        glTexCoord2f(xb, yb);
        glVertex2f( x1, y1);
    glEnd();

    glUseProgram(0);
}

void VG::drawMinus( const Rect& rect, int width, int fillColorId, int borderColorId )
{
    float xb = width;
    float yb = width;
    
    float xoff = xb ;
    float yoff = yb ;

    float x0 = rect.x + rect.w * 0.1 ;
    float x1 = rect.x + rect.w * 0.9;

    float y1 = rect.y + rect.h * 0.5;

    glUseProgram(programs[PRG_SIMPLE_UI]);
    
    glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, s_colors[fillColorId]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, s_colors[borderColorId]);
    glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], (xb) - 1, (xb) - 2);

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x0, y1 + yoff);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x0, y1 - yoff);
 
        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x0 + xoff , y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x0 + xoff, y1 - yoff);

        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x1 - xoff , y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x1 - xoff, y1 - yoff);

        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x1, y1 + yoff);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x1, y1 - yoff);
    glEnd();

    glUseProgram(0);
}

void VG::drawPlus( const Rect& rect, int width, int fillColorId, int borderColorId )
{
    float xb = width;
    float yb = width;
    
    float xoff = xb ;
    float yoff = yb ;

    float x0 = rect.x + rect.w * 0.1 ;
    float x1 = rect.x + rect.w * 0.5;
    float x2 = rect.x + rect.w * 0.9;

    float y0 = rect.y + rect.h * 0.1;
    float y1 = rect.y + rect.h * 0.5;
    float y2 = rect.y + rect.h * 0.9;

    glUseProgram(programs[PRG_SIMPLE_UI]);
    
    glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, s_colors[fillColorId]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, s_colors[borderColorId]);
    glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], (xb) - 1, (xb) - 2);

 /*   glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x0, y1 + yoff);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x0, y1 - yoff);
 
        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x0 + xoff , y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x0 + xoff, y1 - yoff);

        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x1 - xoff , y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x1 - xoff, y1 - yoff);

        glTexCoord3f(0, yb, 0);
        glVertex2f( x1, y1);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(0, yb, 0);
        glVertex2f( x1, y1);

        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x1 + xoff , y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x1 + xoff, y1 - yoff);

        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x2 - xoff , y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x2 - xoff, y1 - yoff);

        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x2, y1 + yoff);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x2, y1 - yoff);
    glEnd();

/**/    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x0, y1 + yoff);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x0, y1 - yoff);
 
        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x0 + xoff , y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x0 + xoff, y1 - yoff);

        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x2 - xoff , y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x2 - xoff, y1 - yoff);

        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x2, y1 + yoff);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x2, y1 - yoff);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x1 + yoff, y0);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x1 - yoff, y0);
 
        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x1 + yoff, y0 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x1 - yoff, y0 + yoff);

        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x1 + yoff, y2 - yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x1 - yoff, y2 - yoff);

        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x1 + yoff, y2);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x1 - yoff, y2);
    glEnd();
/**/
    glUseProgram(0);
}

void VG::drawDownArrow( const Rect& rect, int width, int fillColorId, int borderColorId )
{
    float offset = sqrt(2.0f)/2.0f;
   
    float xb = width;
    float yb = width;
    
    float xoff = offset * xb ;
    float yoff = offset * yb ;
    float xoff2 = offset * xb *2.0f;
    float yoff2 = offset * yb *2.0f;

    float x0 = rect.x + xoff2;
    float x1 = rect.x + rect.w * 0.5f;
    float x2 = rect.x + rect.w - xoff2;

    float y0 = rect.y + rect.h * 0.1f + yoff2;
    float y1 = rect.y + rect.h * 0.6f;

    glUseProgram(programs[PRG_SIMPLE_UI]);
    
    glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, s_colors[fillColorId]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, s_colors[borderColorId]);
    glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], (xb) - 1, (xb) - 2);

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x0, y1 + yoff2);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x0 - xoff2, y1);
 
        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x0 + xoff, y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x0 - xoff, y1 - yoff);

        glTexCoord3f(-xb, 0, xb);
        glVertex2f( x1, y0 + yoff2);
        glTexCoord3f(xb, 0, xb);
        glVertex2f( x1 - xoff2, y0);

        glTexCoord3f(xb, 2*yb, xb);
        glVertex2f( x1, y0 - yoff2);

    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x2 + xoff2, y1);
        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x2, y1 + yoff2);

        glTexCoord3f(xb, 0, xb);
        glVertex2f( x2 + xoff, y1 - yoff);
        glTexCoord3f(-xb, 0, xb);
        glVertex2f( x2 - xoff, y1 + yoff);

        glTexCoord3f(xb, 0, xb);
        glVertex2f( x1 + xoff2, y0);
        glTexCoord3f(-xb, 0, xb);
        glVertex2f( x1, y0 + yoff2);

        glTexCoord3f(xb, 2*yb, xb);
        glVertex2f( x1, y0 - yoff2);

    glEnd();

    glUseProgram(0);
}

void VG::drawUpArrow( const Rect& rect, int width, int fillColorId, int borderColorId )
{
    float offset = sqrt(2.0f)/2.0f;
   
    float xb = width;
    float yb = width;
    
    float xoff = offset * xb ;
    float yoff = - offset * yb ;
    float xoff2 = offset * xb * 2.0f;
    float yoff2 = - offset * yb * 2.0f;

    float x0 = rect.x + xoff2;
    float x1 = rect.x + rect.w * 0.5f;
    float x2 = rect.x + rect.w - xoff2;

    float y0 = rect.y + rect.h * 0.9f + yoff2;
    float y1 = rect.y + rect.h * 0.4f;

    glUseProgram(programs[PRG_SIMPLE_UI]);
    
    glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, s_colors[fillColorId]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, s_colors[borderColorId]);
    glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], (xb) - 1, (xb) - 2);

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x0, y1 + yoff2);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x0 - xoff2, y1);
 
        glTexCoord3f(-xb, 0, 0);
        glVertex2f( x0 + xoff, y1 + yoff);
        glTexCoord3f(xb, 0, 0);
        glVertex2f( x0 - xoff, y1 - yoff);

        glTexCoord3f(-xb, 0, xb);
        glVertex2f( x1, y0 + yoff2);
        glTexCoord3f(xb, 0, xb);
        glVertex2f( x1 - xoff2, y0);

        glTexCoord3f(xb, 2*yb, xb);
        glVertex2f( x1, y0 - yoff2);

    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord3f(xb, -yb, 0);
        glVertex2f( x2 + xoff2, y1);
        glTexCoord3f(-xb, -yb, 0);
        glVertex2f( x2, y1 + yoff2);

        glTexCoord3f(xb, 0, xb);
        glVertex2f( x2 + xoff, y1 - yoff);
        glTexCoord3f(-xb, 0, xb);
        glVertex2f( x2 - xoff, y1 + yoff);

        glTexCoord3f(xb, 0, xb);
        glVertex2f( x1 + xoff2, y0);
        glTexCoord3f(-xb, 0, xb);
        glVertex2f( x1, y0 + yoff2);

        glTexCoord3f(xb, 2*yb, xb);
        glVertex2f( x1, y0 - yoff2);

    glEnd();

    glUseProgram(0);
}

void VG::drawRect( const Rect & rect, int fillColorId, int borderColorId )
{
    glUseProgram(programs[PRG_SIMPLE_UI]);

    glUniform4fv(uniforms[UNI_SIMPLE_UI_FILL_COLOR], 1, s_colors[fillColorId]);
    glUniform4fv(uniforms[UNI_SIMPLE_UI_BORDER_COLOR], 1, s_colors[borderColorId]);
    glUniform2f(uniforms[UNI_SIMPLE_UI_ZONES], 0, 0);

    float x0 = rect.x;
    float x1 = rect.x + rect.w;
    
    float y0 = rect.y;
    float y1 = rect.y + rect.h;

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, 0);

        glVertex2f( x0, y0);
        glVertex2f( x1, y0);

        glVertex2f( x0, y1);
        glVertex2f( x1, y1);
    glEnd();

    glUseProgram(0);
}

void VG::drawFrame( const Rect& rect, const Point& corner, bool isHover, bool isOn, bool isFocus )
{
    int lColorNb = cBase + (isHover) + (isOn << 1);// + (isFocus << 2);

    if (corner.x + corner.y == 0)
        drawRect( rect , lColorNb, cOutline);
    else
        drawRoundedRect( rect, corner , lColorNb, cOutline );
}
void VG::drawBoolFrame( const Rect& rect, const Point& corner, bool isHover, bool isOn, bool isFocus )
{
    int lColorNb = cBool + (isHover) + (isOn << 1);// + (isFocus << 2);
        
    drawRoundedRect( rect, corner , lColorNb, cOutline );
}
