#include <framework.h>

static unsigned char permutation[] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

// gradients for 3d noise
static float g[] = {
		 2, 1,
		 2,-1,
		-2, 1,
		-2,-1,
         1, 2,
		 1,-2,
		-1, 2,
		-1,-2
};

class PhysisDemo: public ui::SDLStage
{
		GLuint tex;
		GLuint fbo;

		GLint vp[4];

		GLuint texGtorProg;

		GLuint permTex, gradTex;

		GLint uniSamplerGrad, uniSamplerPerm;
	
		ui::ProfileStatsBox		mStatsBox;
		sui::Font				mFont;

	public:
		PhysisDemo()
		{
			VFS::mount("..\\..\\AppData");

			tex = createFBOTexture(GL_RGBA8, 1024, 1024);
			fbo = createFBO(tex);

			glGetIntegerv(GL_VIEWPORT, vp);

			GLuint shader = resources::createShaderFromFile(GL_FRAGMENT_SHADER, /*"TextureGtor.Metal.frag"*/"Perlin.Noise.frag");
			texGtorProg = glCreateProgram();
			glAttachShader(texGtorProg, shader);
			glLinkProgram(texGtorProg);
			glDeleteShader(shader);

			glGenTextures(1, &permTex);
			glBindTexture(GL_TEXTURE_1D, permTex);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}
			glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, 256, 0, GL_RED, GL_UNSIGNED_BYTE, permutation);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glGenTextures(1, &gradTex);
			glBindTexture(GL_TEXTURE_1D, gradTex);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, 8, 0, GL_RG, GL_FLOAT, g);

			glBindTexture(GL_TEXTURE_2D, 0);

			uniSamplerGrad = glGetUniformLocation(texGtorProg, "samplerGrad");
			uniSamplerPerm = glGetUniformLocation(texGtorProg, "samplerPerm");

			glUseProgram(texGtorProg);
			glUniform1i(uniSamplerPerm, 0);
			glUniform1i(uniSamplerGrad, 1);
			glUseProgram(0);

			mFont = sui::createFont("K:\\media\\Fonts\\AnonymousPro-1.002.001\\Anonymous Pro B.ttf");
			mFont.setSize(16);

			add(mStatsBox.setFont(mFont)
				  .setPos(10, 10)
				  .setSize(300, 70)
			);

			mt::addTimedTask<PhysisDemo, &PhysisDemo::onUpdateStats>(this, 250);
		}
		
		CPUTimer	mCPUTimer;
		GPUTimer	mGPUTimer;

		double mCPUTime;
		double mGPUTime;

		void onUpdateStats()
		{
			mStatsBox.setStats(mCPUTime, mGPUTime);
		}

		~PhysisDemo()
		{
			glDeleteProgram(texGtorProg);
			glDeleteFramebuffers(1, &fbo);
			glDeleteTextures(1, &tex);
			glDeleteTextures(1, &permTex);
			glDeleteTextures(1, &gradTex);
		}

	protected:
		GLuint createFBOTexture(GLenum internalFormat, GLsizei width, GLsizei height)
		{
			GLuint texture;

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			return texture;
		}

		GLuint createFBO(GLuint rt0/*, GLuint depthRenderbuffer*/)
		{
			GLuint fbo;
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt0, 0);
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

			GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
			
			glDrawBuffers(ARRAY_SIZE(buffers), buffers);

			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if(status != GL_FRAMEBUFFER_COMPLETE)
			{
				glDeleteFramebuffers(1, &fbo);
				fbo = 0;
			}
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return fbo;
		}

		void generateTexture()
		{
			mCPUTimer.start();
			mGPUTimer.start();

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			
			glViewport(0, 0, 1024, 1024);

			glClearColor(0, 0, 0, 1);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_1D, permTex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, gradTex);

			glColor3f(1, 0, 1);
			glUseProgram(texGtorProg);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 1);
				glVertex2f(-1,  1);
				glTexCoord2f(1, 1);
				glVertex2f( 1,  1);
				glTexCoord2f(1, 0);
				glVertex2f( 1, -1);
				glTexCoord2f(0, 0);
				glVertex2f(-1, -1);
			glEnd();
			glUseProgram(0);

			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			mGPUTimer.stop();
			mCPUTime = mCPUTimer.elapsed();
			mGPUTime = mGPUTimer.getResult();
			mCPUTimer.stop();
		}

		virtual void onPaint()
		{
			generateTexture();

			glViewport(vp[0], vp[1], vp[2], vp[3]);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0, 0, -10);
			
			glColor3f(1, 1, 1);
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 1);
				glVertex2f(-5,  5);
				glTexCoord2f(1, 1);
				glVertex2f( 5,  5);
				glTexCoord2f(1, 0);
				glVertex2f( 5, -5);
				glTexCoord2f(0, 0);
				glVertex2f(-5, -5);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}

	private:
		VFS				mVFS;
};

extern "C" int main(int argc, char** argv)
{
	ui::init();
	{
		PhysisDemo app;
		app.run();
	}
	ui::cleanup();

	return 0;
}