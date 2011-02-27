#include <framework.h>

static unsigned char permutation256[] = { 151,160,137,91,90,15,
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

// gradients for 2d noise
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
		enum TextureNames
		{
			PERLIN0 = 0,
			PERLIN1,
			PERLIN2,
			PERLIN3,
			COMBINED0,
			COMBINED1,
			COMBINED2,
			COMBINED3,
			COMBINED4,

			TEXTURE_NAMES_COUNT
		};

		GLuint mTextures[TEXTURE_NAMES_COUNT];

		static const int texSize = 1024;
		GLuint blurTex[10];
		GLuint fbo;

		GLint vp[4];

		GLuint texGtorProg, perlinGtorProg;

		GLuint permTex, gradTex;

		GLint uniSamplerGrad, uniSamplerPerm, uniInvTexDim;
		
		GLint uniSamplerInput1;
		GLint uniSamplerInput2;
		GLint uniHSCB;

		GLint	uniOctaves;
		GLint	uniAmp;
		GLint	uniAmpScale;
		GLint	uniFreq;
		GLint	uniFreqScale;

		ui::ProfileStatsBox		mStatsBox;
		sui::Font				mFont;

	public:
		void allocTextures()
		{
			glGenTextures(TEXTURE_NAMES_COUNT, mTextures);
			
			for (size_t i=0; i<TEXTURE_NAMES_COUNT; ++i)
			{
				glBindTexture(GL_TEXTURE_2D, mTextures[i]);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texSize, texSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void freeTextures()
		{
			glDeleteTextures(TEXTURE_NAMES_COUNT, mTextures);
		}

		void CreatePerlinNoiseGtor()
		{
			GLuint shader, shaderPerlin;

			perlinGtorProg = glCreateProgram();

			shaderPerlin = resources::createShaderFromFile(GL_FRAGMENT_SHADER, "Perlin.Noise.frag");
			shader = resources::createShaderFromFile(GL_FRAGMENT_SHADER, "TextureGtor.Perlin.frag");
			glAttachShader(perlinGtorProg, shader);
			glAttachShader(perlinGtorProg, shaderPerlin);
			glDeleteShader(shader);
			glDeleteShader(shaderPerlin);

			glLinkProgram(perlinGtorProg);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			uniSamplerGrad = glGetUniformLocation(perlinGtorProg, "samplerGrad");
			uniSamplerPerm = glGetUniformLocation(perlinGtorProg, "samplerPerm");
			uniInvTexDim   = glGetUniformLocation(perlinGtorProg, "invTexDim");
			uniOctaves		= glGetUniformLocation(perlinGtorProg, "uOctaves");
			uniAmp			= glGetUniformLocation(perlinGtorProg, "uAmp");
			uniAmpScale		= glGetUniformLocation(perlinGtorProg, "uAmpScale");
			uniFreq			= glGetUniformLocation(perlinGtorProg, "uFreq");
			uniFreqScale	= glGetUniformLocation(perlinGtorProg, "uFreqScale");

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glUseProgram(perlinGtorProg);
			glUniform1i(uniSamplerPerm, 0);
			glUniform1i(uniSamplerGrad, 1);
			glUniform1f(uniInvTexDim, 1.0f/ARRAY_SIZE(permutation256));
			glUseProgram(0);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			//Gen perlin permutation and gradient textures
			glGenTextures(1, &permTex);
			glBindTexture(GL_TEXTURE_1D, permTex);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, ARRAY_SIZE(permutation256), 0, GL_RED, GL_UNSIGNED_BYTE, permutation256);

			glGenTextures(1, &gradTex);
			glBindTexture(GL_TEXTURE_1D, gradTex);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, 8, 0, GL_RG, GL_FLOAT, g);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void CreateTextureGtor()
		{
			GLuint shader, shaderPerlin;
			texGtorProg = glCreateProgram();

			shader = resources::createShaderFromFile(GL_FRAGMENT_SHADER, "TextureGtor.Metal.frag");
			glAttachShader(texGtorProg, shader);
			glDeleteShader(shader);

			glLinkProgram(texGtorProg);

			uniSamplerInput1 = glGetUniformLocation(texGtorProg, "samInput1");
			uniSamplerInput2 = glGetUniformLocation(texGtorProg, "samInput2");
			uniHSCB = glGetUniformLocation(texGtorProg, "uHSCB");

			glUseProgram(texGtorProg);
			glUniform1i(uniSamplerInput1, 0);
			glUniform1i(uniSamplerInput2, 1);
			glUseProgram(0);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}
		}

		PhysisDemo()
		{
			VFS::mount("AppData");
			VFS::mount("..\\AppData");
			VFS::mount("..\\..\\AppData");

			glGenFramebuffers(1, &fbo);

			glGenTextures(10, blurTex);
			
			for (size_t size = texSize, i=0; i<10; ++i)
			{
				glBindTexture(GL_TEXTURE_2D, blurTex[i]);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				size /= 2;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			allocTextures();

			glGetIntegerv(GL_VIEWPORT, vp);
			
			//Texture program
			CreateTextureGtor();

			//Perlin program
			CreatePerlinNoiseGtor();

			mFont = sui::createFont("C:\\Windows\\Fonts\\consola.ttf");
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
			freeTextures();
			glDeleteProgram(texGtorProg);
			glDeleteProgram(perlinGtorProg);
			glDeleteFramebuffers(1, &fbo);
			glDeleteTextures(10, blurTex);
			glDeleteTextures(1, &permTex);
			glDeleteTextures(1, &gradTex);
		}

	protected:
		void generateTexture(TextureNames dest, TextureNames src1, TextureNames src2)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextures[dest], 0);
			
			glViewport(0, 0, texSize, texSize);

			glClearColor(0, 0, 0, 1);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mTextures[src1]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mTextures[src2]);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glColor3f(1, 0, 1);
			glUseProgram(texGtorProg);
			glUniformMatrix4fv(uniHSCB, 1, false, getHSCB(0.0f, 1.0f, 1.375f, 2.5f));
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

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void generatePerlin(TextureNames dest, int octaves, float amp, float ampScale, float freq, float freqScale)
		{
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextures[dest], 0);
			
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glViewport(0, 0, texSize, texSize);

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
			glUseProgram(perlinGtorProg);

			glUniform1i(uniOctaves,   octaves);
			glUniform1f(uniAmp,       amp);
			glUniform1f(uniAmpScale,  ampScale);
			glUniform1f(uniFreq,      freq);
			glUniform1f(uniFreqScale, freqScale);

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
		}

		void downsample(GLuint tex, GLuint dest, GLuint dim)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dest, 0);
			
			glViewport(0, 0, dim, dim);

			glClearColor(0, 0, 0, 1);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glColor4f(1, 1, 1, 1);
			glUseProgram(0);
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
			glDisable(GL_TEXTURE_2D);

			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glm::mat4 getHSCB(float hue, float saturation, float contrast, float brightness)
		{
			glm::mat4 brightnessMatrix = glm::scale3DGTX(glm::mat4(), brightness, brightness, brightness);
			glm::mat4 contrastMatrix = glm::translate3DGTX(glm::mat4(), 0.5f, 0.5f, 0.5f)*
				glm::scale3DGTX(glm::mat4(), contrast, contrast, contrast)*
				glm::translate3DGTX(glm::mat4(), -0.5f, -0.5f, -0.5f);

			//TODO:add hue and saturation matrices

			glm::mat4 hueMatrix = glm::rotateGTX(hue, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

			const float RED_EYE_SENSIBILITY = 0.3086f;
			const float GREEN_EYE_SENSIBILITY = 0.6094f;
			const float BLUE_EYE_SENSIBILITY = 0.0820f;

			float s = saturation;
			glm::mat4 saturationMatrix = glm::mat4(
					glm::vec4((1.0f-s)*RED_EYE_SENSIBILITY + s, (1.0f-s)*RED_EYE_SENSIBILITY,       (1.0f-s)*RED_EYE_SENSIBILITY,       0.0f),
					glm::vec4((1.0f-s)*GREEN_EYE_SENSIBILITY,   (1.0f-s)*GREEN_EYE_SENSIBILITY + s, (1.0f-s)*GREEN_EYE_SENSIBILITY,     0.0f),
					glm::vec4((1.0f-s)*BLUE_EYE_SENSIBILITY,    (1.0f-s)*BLUE_EYE_SENSIBILITY,      (1.0f-s)*BLUE_EYE_SENSIBILITY + s,  0.0f),
					glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
				);

			return brightnessMatrix*contrastMatrix;
		}

		void drawImage(float x, float y, TextureNames id)
		{
			drawImage(x, y, mTextures[id]);
		}

		void drawImage(float x, float y, GLuint texture)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(x, y, -10);
			
			glColor4f(1, 1, 1, 1);
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}

		virtual void onPaint()
		{
			mCPUTimer.start();
			mGPUTimer.start();

			//generatePerlin(perlinTex, texSize, 5, 1.86, 0.75, 12.0, 2.0);
			generatePerlin(PERLIN0, 6, 1.36, 0.75, 6.0, 2.0);
			generatePerlin(PERLIN1, 1, 1.36, 0.75, 6.0, 2.0);
			generateTexture(COMBINED0, PERLIN0, PERLIN1);
			//generateTexture(COMBINED1, PERLIN1);

			generatePerlin(PERLIN2, 6, 3.36, 0.75, 6.0, 2.0);
			generatePerlin(PERLIN3, 1, 3.36, 0.75, 6.0, 2.0);
			generateTexture(COMBINED2, PERLIN2, PERLIN3);
			//generateTexture(COMBINED3, PERLIN3);

			mGPUTimer.stop();
			mCPUTime = mCPUTimer.elapsed();
			mGPUTime = mGPUTimer.getResult();
			mCPUTimer.stop();

			downsample(mTextures[PERLIN0],  blurTex[0], texSize/2);
			downsample(blurTex[0], blurTex[1], texSize/4);
			downsample(blurTex[1], blurTex[2], texSize/8);
			downsample(blurTex[2], blurTex[3], texSize/16);
			downsample(blurTex[3], blurTex[4], texSize/32);
			downsample(blurTex[4], blurTex[5], texSize/64);
			downsample(blurTex[5], blurTex[6], texSize/128);
			downsample(blurTex[6], blurTex[7], texSize/256);
			downsample(blurTex[7], blurTex[8], texSize/512);
			downsample(blurTex[8], blurTex[9], texSize/1024);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(vp[0], vp[1], vp[2], vp[3]);
			drawImage( -8,  2, PERLIN0);
			drawImage( -5,  2, PERLIN1);
			drawImage( -2,  2, PERLIN2);
			drawImage(  1,  2, PERLIN3);
			drawImage(  4,  2, COMBINED0);
			drawImage(  7,  2, COMBINED2);

			drawImage( -8, -1, blurTex[1]);
			drawImage( -5, -1, blurTex[2]);
			drawImage( -2, -1, blurTex[3]);
			drawImage(  1, -1, blurTex[4]);
			drawImage(  4, -1, blurTex[5]);
			drawImage(  7, -1, blurTex[6]);
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