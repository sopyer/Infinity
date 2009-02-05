#include <framework.h>

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
		}
	protected:
		void paint()
		{
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glEnable(GL_DEPTH_TEST);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, -1.0f, -4.0f);
			mRenderer.begin();
				for(float i = -10; i <= 10; i += 1)
				{
					glBegin(GL_LINES);
						glColor3ub(68, 193, 181);						
						glVertex3f(-10, 0, i);					
						glVertex3f(10, 0, i);
						glVertex3f(i, 0,-10);							
						glVertex3f(i, 0, 10);
					glEnd();
				}
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
				glTranslatef(0, 1, 0);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				mRenderer.useProgram(&mProgram);
				glColor3f(0.2f, 0.7f, 0.4f);
				DrawShape2();
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
	mRenderer.end();
}
