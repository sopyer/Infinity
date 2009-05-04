#include <framework.h>

#include <VG\path.h>

GLchar buf[1024];
GLsizei len;

#define PRINT_SHADER_LOG(obj)	obj.getLog(1023, &len, buf);\
	logMessage(#obj##" info\n%s", buf)


char* vertSrc = 
"\
void main(void)\
{\
    gl_TexCoord[0] = gl_MultiTexCoord0;\
	gl_Position = ftransform();\
}\
";

char* fragSrc1 = 
"\
void main(void)\
{\
	vec2 uv = gl_TexCoord[0].st;\
	if( (pow(uv.s, 2.0)-uv.t)>0.0 ) discard;\
	gl_FragColor = vec4(1);\
}\
",
*fragSrc =
"\
uniform float orient;\n\
void main(void)\n\
{\n\
  vec2 uv = gl_TexCoord[0].st;\n\
  float f = pow(uv.s, 2.0)-uv.t;\n\
  mat2 J = mat2(dFdx(uv), dFdy(uv));\n\
  //vec2 dx = dFdx(uv), dy = dFdy(uv);\n\
  vec2 dF = vec2(2.0*uv.x, -1.0);\n\
  float dist = f/length(dF*J);\n\
  dist*=orient;\n\
  if( dist < 0.0 )\n\
    discard;\n\
  //float a = clamp(dist, 0.0, 1.0);\n\
  float a = smoothstep(0.0, 1.0, dist);\n\
  gl_FragColor = vec4(gl_Color.rgb*a, gl_Color.a);\n\
}\n\
";

class VGSample: public UI::Stage
{
	public:
		VGSample()
		{
			mFragShader.compile(fragSrc);
			PRINT_SHADER_LOG(mFragShader);
			mProgram.addShader(&mFragShader);
			mProgram.link();
			PRINT_SHADER_LOG(mProgram);
			mProgram.validate();
			
			mOrientation = mProgram.bindUniform("orient");
			PRINT_SHADER_LOG(mProgram);
			
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(35.0, (float)mWidth/mHeight,1,2048);

			maskQuad.create("VG.MaskQuad", "vg.mask.vert", "vg.mask.quad.frag");
			maskCubic.create("VG.MaskCubic", "vg.mask.vert", "vg.mask.cubic.frag");
			maskArc.create("VG.MaskArc", "vg.mask.vert", "vg.mask.arc.frag");
			vg::init(maskQuad, maskCubic, maskArc);
			program.create("VG.SolidFill", "vg.vert", "vg.paint.solid.frag");
			uFillColor = program->bindUniform("uFillColor");
			
			mPolygon = vg::createPath(1.0f, 0.0f);

			ubyte segs[] = {VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH};
			float data[] = {100, 100, 100, 0, 0, 100, 0, 0};

			ubyte quadTest[] = {VG_QUAD_TO_ABS, VG_CLOSE_PATH};
			float quadData[] = {50, 100, 100, 0};

			ubyte lineQuadTest[] = {VG_QUAD_TO_ABS, VG_LINE_TO_ABS, VG_LINE_TO_ABS, VG_CLOSE_PATH};
			float lineQuadData[] = {50, 100, 100, 0, 0, 100, 100, 100};


			//ubyte arcTest[] = {VG_LCCWARC_TO_ABS,/* VG_MOVE_TO_ABS,*/ VG_SCCWARC_TO_ABS, VG_CLOSE_PATH};
			////float arcData[] = {0.3, 0.3, 0, 0.5, 0, 0, 0, 0.3, 0.3, 0, 0.5, 0};
			//float arcData[] = {40, 50, 0, 60, 0, /*60, 0,*/ 40, 50, 0, 0, 0};

			//Bug!!!! if main geom actually has zero geometry in fact in covers some pixels on screen
			ubyte arcTest[] = {/*VG_MOVE_TO_REL,*/ VG_SCCWARC_TO_REL, VG_SCCWARC_TO_REL, VG_SCCWARC_TO_REL, VG_SCCWARC_TO_REL, VG_CLOSE_PATH};
			float arcData[] = {/*-25, -25,*/ 25, 25, 0, 0, 50, 25, 25, 0, 50, 0, 25, 25, 0, 0, -50, 25, 25, 0, -50, 0};

			//Bug!!!!
			//ubyte arcTest[] = {/*VG_MOVE_TO_REL,*/ VG_SCCWARC_TO_REL, VG_LINE_TO_REL, VG_SCCWARC_TO_REL, VG_CLOSE_PATH};
			//float arcData[] = {25, 25, 0, 0, 50, -25, -25, 25, 25, 0, -50, 0};

			//ubyte arcTest[] = {/*VG_MOVE_TO_REL,*/ VG_SCCWARC_TO_REL, VG_LINE_TO_REL, VG_SCCWARC_TO_REL, VG_CLOSE_PATH};
			//float arcData[] = {25, 25, 0, 0, 50, 50, -50, 25, 25, 0, -50, 0};

			ubyte squadTest[] = {VG_QUAD_TO_ABS, VG_SQUAD_TO_ABS, VG_CLOSE_PATH};
			float squadData[] = {50, 100, 100, 0, 200, 0};

			ubyte cubicTest[] = {VG_MOVE_TO_ABS, VG_CUBIC_TO_ABS, VG_CLOSE_PATH};
			float cubicData[] = {0, 0, 100, 100, 100, 0, -25, -25};
			//float cubicData[] = {-200, 200, -100, 100, -200, 0, 150, 0};
			//float cubicData[] = {200, 100, -100, 100, 100, 0, 150, 0};
			//float cubicData[] = {50, 40, 100, 0, 150, 50, 150, 0};

			//mPolygon.appendData(sizeof(segs), segs, data);
			//mPolygon.appendData(sizeof(quadTest), quadTest, quadData);
			//mPolygon.appendData(sizeof(lineQuadTest), lineQuadTest, lineQuadData);
			//mPolygon.appendData(sizeof(arcTest), arcTest, arcData);
			//mPolygon.appendData(sizeof(squadTest), squadTest, squadData);
			mPolygon.appendData(sizeof(cubicTest), cubicTest, cubicData);

			glClearDepth(1.0);
			glClearStencil(0);
		}

		~VGSample()
		{
			vg::destroyPath(mPolygon);
			vg::deinit();
		}


		//void run()
		//{
		//	while (glfwGetWindowParam(GLFW_OPENED))
		//	{
		//		render();
		//		glFlush();
		//		glfwSwapBuffers();
		//	}
		//}

	protected:
		void onPaint(VG& vg)
		{
 			glStencilMask(0xFF);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			//glLoadIdentity();
			glTranslatef(400.0f, 300.0f, -4.0f);
			glScalef(1, -1, 1);
			mRenderer.begin();
				//for(float i = -10; i <= 10; i += 1)
				//{
				//	glBegin(GL_LINES);
				//		glColor3ub(68, 193, 181);						
				//		glVertex3f(-10, 0, i);					
				//		glVertex3f(10, 0, i);
				//		glVertex3f(i, 0,-10);							
				//		glVertex3f(i, 0, 10);
				//	glEnd();
				//}
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//glColor3f(1, 1, 1);
				//glBegin(GL_TRIANGLES);
				//	glTexCoord2f(0, 0);
				//	glVertex3f(-1, 1, 3);
				//	glTexCoord2f(0.5, 0);
				//	glVertex3f(2, 0, 0);
				//	glTexCoord2f(1, 1);
				//	glVertex3f(1, 2, 2);
				//glEnd();
				//glTranslatef(0, 1, 0);
				glScalef(2, 2, 1);
				glCullFace(GL_FRONT_AND_BACK);
				glUseProgram(*program);
				glUniform4f(uFillColor, 0.45f, 0.33f, 0.89f, 1.0f);
				mPolygon.draw(program);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				//mRenderer.useProgram(&mProgram);
				//glColor3f(0.2f, 0.7f, 0.4f);
				//DrawShape2();
			//glCullFace(GL_FRONT/*_AND_BACK*/);
			//mPolygon.drawPath();
			mRenderer.end();
			glFlush();
		}

		void onUpdate(float frame_time)
		{
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();
		}
		
		void DrawShape1();
		void DrawShape2();

	private:
		vg::Path	mPolygon;

		ProgramRef	program;
		ProgramRef	maskQuad;
		ProgramRef	maskCubic;
		ProgramRef	maskArc;

		GLint		uFillColor;

		glRenderer			mRenderer;

		glProgram			mProgram;
		glVertexShader		mVertShader;
		glFragmentShader	mFragShader;

		GLint				mOrientation;

};

int main()
{
	VGSample app;
	app.run();
	return 0;
}

void VGSample::DrawShape1()
{
	glUniform1f(mOrientation, 0.5);
	glBegin(GL_TRIANGLES);
		//glTexCoord2f(0, 0);
		//glVertex3f(-1, 0, 0);
		//glTexCoord2f(0.5, 0);
		//glVertex3f(0, 1, 0);
		//glTexCoord2f(1, 1);
		//glVertex3f(1, 0, 0);

		//glTexCoord2f(0, 0);
		//glVertex3f(-1, 0, 0);
		//glTexCoord2f(0, 0);
		//glVertex3f(-2, -1, 0);
		//glTexCoord2f(1, 1);
		//glVertex3f(-1, -2, 0);

		//glTexCoord2f(0, 0);
		//glVertex3f(-1, 0, 0);
		//glTexCoord2f(1, 1);
		//glVertex3f(-1, -2, 0);
		//glTexCoord2f(1, 1);
		//glVertex3f(1, 0, 0);
	glEnd();
}

void VGSample::DrawShape2()
{
	glUniform1f(mOrientation, 0.5);
	glBegin(GL_TRIANGLES);
		glColor3f(0.2f, 0.7f, 0.4f);
		glTexCoord2f(0, 0);
		glVertex3f(-1, 0, 0);
		glColor3f(0.2f, 0.2f, 0.4f);
		glTexCoord2f(0.5, 0);
		glVertex3f(-0.5, 1, 0);
		glColor3f(0.5f, 0.3f, 0.4f);
		glTexCoord2f(1, 1);
		glVertex3f(0, 0, 0);
		
		glColor3f(0.2f, 0.2f, 0.4f);
		glTexCoord2f(0.5, 0);
		glVertex3f(-0.5, 1, 0);
		glColor3f(0.5f, 0.3f, 0.4f);
		glTexCoord2f(1, 1);
		glVertex3f(0, 0, 0);
		glColor3f(0.6f, 0.3f, 0.4f);
		glTexCoord2f(1, 0);
		glVertex3f(0.5, 1, 0);
	glEnd();
	glUniform1f(mOrientation, -0.5);
	glBegin(GL_TRIANGLES);
		glColor3f(0.5f, 0.8f, 0.4f);
		glTexCoord2f(0, 0);
		glVertex3f(1, 0, 0);
		glColor3f(0.2f, 0.8f, 0.3f);
		glTexCoord2f(0.5, 0);
		glVertex3f(0.5, -1, 0);
		glColor3f(0.5f, 0.3f, 0.4f);
		glTexCoord2f(1, 1);
		glVertex3f(0, 0, 0);
		
		glColor3f(0.6f, 0.3f, 0.4f);
		glTexCoord2f(0, 0.5);
		glVertex3f(0.5, 1, 0);
		glColor3f(0.5f, 0.3f, 0.4f);
		glTexCoord2f(1, 1);
		glVertex3f(0, 0, 0);
		glColor3f(0.5f, 0.8f, 0.4f);
		glTexCoord2f(0, 0);
		glVertex3f(1, 0, 0);
	glEnd();
}
