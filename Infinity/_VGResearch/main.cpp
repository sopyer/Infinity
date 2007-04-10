#include <framework.h>

GLchar buf[1024];
GLsizei len;

#define PRINT_SHADER_LOG(obj)	obj.getInfoLog(1023, &len, buf);\
	print(#obj##"info\n%s", buf)


char* vertSrc = 
"\
void main(void)\
{\
    gl_TexCoord[0] = gl_MultiTexCoord0;\
	gl_Position = ftransform();\
}\
";

char* fragSrc = 
"\
void main(void)\
{\
	vec2 uv = gl_TexCoord[0].st;\
	if( (pow(uv.s, 2.0)-uv.t)>0.0 ) discard;\
	gl_FragColor = vec4(1);\
}\
";

class VGApp: public Framework
{
	private:
		glRenderer	renderer_;
		glProgram	program_;
		glVertexShader	vert_;
		glFragmentShader frag_;
	protected:
		void OnCreate()
		{
			vert_.compile(vertSrc);
			PRINT_SHADER_LOG(vert_);
			frag_.compile(fragSrc);
			PRINT_SHADER_LOG(frag_);
			program_.addShader(&vert_);
			program_.addShader(&frag_);
			program_.link();
			PRINT_SHADER_LOG(program_);
			program_.validate();
			PRINT_SHADER_LOG(program_);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, (float)width_/height_,1,2048); 
		}
		
		void OnDestroy()
		{
		}

		void OnRender()
		{
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glEnable(GL_DEPTH_TEST);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, -1.0f, -4.0f);
			renderer_.beginRenderPass();
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
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glColor3f(1, 1, 1);
				glBegin(GL_TRIANGLES);
					glTexCoord2f(0, 0);
					glVertex3f(-1, 1, 3);
					glTexCoord2f(0.5, 0);
					glVertex3f(2, 0, 0);
					glTexCoord2f(1, 1);
					glVertex3f(1, 2, 2);
				glEnd();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				renderer_.useProgram(&program_);
				glColor3f(1, 1, 1);
				glBegin(GL_TRIANGLES);
					glTexCoord2f(0, 0);
					glVertex3f(-1, 1, 3);
					glTexCoord2f(0.5, 0);
					glVertex3f(2, 0, 0);
					glTexCoord2f(1, 1);
					glVertex3f(1, 2, 2);
				glEnd();
			renderer_.endRenderPass();
			glFlush();
		}

		void OnUpdate(float frame_time)
		{
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();
		}
};

int main()
{
	VGApp app;
	app.run();
	return 0;
}