#include <framework.h>

#include "Perlin.h"

struct VertexPosUV
{
	float x, y, u, v;
};

static const VertexPosUV quadVertices[4] = {
	{-1,  1, 0, 1},
	{ 1,  1, 1, 1},
	{ 1, -1, 1, 0},
	{-1, -1, 0, 0}
};

static uint16_t quadIndices[6] = {0, 1, 3, 1, 2, 3};

class PhysisDemo: public ui::SDLStage
{
		enum TextureNames
		{
			PERLIN0 = 0,
			PERLIN1,
			PERLIN2,
			PERLIN3,
			PERLIN4,
			PERLIN5,
			PERLIN6,
			PERLIN7,
			TEXTURE_SRT,
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

		GLuint permTex[9], gradTex;

		GLint uniSamplerGrad, uniSamplerPerm, uniInvTexDim;
		
		GLint uniSamplerInput1;
		GLint uniSamplerInput2;
		GLint uniHSCB;

		GLint	uniOctaves;
		GLint	uniAmp;
		GLint	uniAmpScale;
		GLint	uniFreq;
		GLint	uniFreqScale;
		GLint	uniGamma;

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
			GLuint shader;

			perlinGtorProg = glCreateProgram();

			shader = resources::createShaderFromFile(GL_VERTEX_SHADER, "FF.Copy.PosUV.vert");
			glAttachShader(perlinGtorProg, shader);
			glDeleteShader(shader);
			shader = resources::createShaderFromFile(GL_FRAGMENT_SHADER, "Perlin.Noise.frag");
			glAttachShader(perlinGtorProg, shader);
			glDeleteShader(shader);
			shader = resources::createShaderFromFile(GL_FRAGMENT_SHADER, "TextureGtor.Perlin.frag");
			glAttachShader(perlinGtorProg, shader);
			glDeleteShader(shader);

			glLinkProgram(perlinGtorProg);

			GLint res;
			glGetProgramiv(perlinGtorProg, GL_LINK_STATUS, &res);

			assert(res);

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
			uniGamma        = glGetUniformLocation(perlinGtorProg, "uGamma");

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glUseProgram(perlinGtorProg);
			glUniform1i(uniSamplerPerm, 0);
			glUniform1i(uniSamplerGrad, 1);
			glUniform1f(uniInvTexDim, 1.0f/PERMUTATION_DIM);
			glUseProgram(0);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			//Gen perlin permutation and gradient textures
			glGenTextures(9, permTex);
			for (size_t i=0; i<9; ++i)
			{
				glBindTexture(GL_TEXTURE_1D, permTex[i]);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, PERMUTATION_DIM, 0, GL_RED, GL_UNSIGNED_BYTE, permutation256[i]);
			}

			glGenTextures(1, &gradTex);
			glBindTexture(GL_TEXTURE_1D, gradTex);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RG32F, 8, 0, GL_RG, GL_FLOAT, gradients2D[0]);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void CreateTextureGtor()
		{
			GLuint shader;
			texGtorProg = glCreateProgram();

			shader = resources::createShaderFromFile(GL_VERTEX_SHADER, "FF.Copy.PosUV.vert");
			glAttachShader(texGtorProg, shader);
			glDeleteShader(shader);
			shader = resources::createShaderFromFile(GL_FRAGMENT_SHADER, "TextureGtor.Metal.frag");
			glAttachShader(texGtorProg, shader);
			glDeleteShader(shader);

			glLinkProgram(texGtorProg);

			//uniSamplerInput1 = glGetUniformLocation(texGtorProg, "samInput1");
			//uniSamplerInput2 = glGetUniformLocation(texGtorProg, "samInput2");
			//uniHSCB = glGetUniformLocation(texGtorProg, "uHSCB");

			glUseProgram(texGtorProg);
			glUniform1i(glGetUniformLocation(texGtorProg, "samInput0"), 0);
			glUniform1i(glGetUniformLocation(texGtorProg, "samInput1"), 1);
			glUniform1i(glGetUniformLocation(texGtorProg, "samInput2"), 2);
			glUniform1i(glGetUniformLocation(texGtorProg, "samInput3"), 3);
			glUniform1i(glGetUniformLocation(texGtorProg, "samInput4"), 4);
			glUniform1i(glGetUniformLocation(texGtorProg, "samInput5"), 5);
			glUniform1i(glGetUniformLocation(texGtorProg, "samInput6"), 6);
			glUniform1i(glGetUniformLocation(texGtorProg, "samInput7"), 7);
			glUniform1i(uniSamplerInput2, 1);
			glUseProgram(0);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}
		}

		GLuint srtGtorProg;
		GLint uniSamplerInput, uniRotationScale, uniTranslate;
		void CreateSRTGtor()
		{
			GLuint shader;
			srtGtorProg = glCreateProgram();

			shader = resources::createShaderFromFile(GL_VERTEX_SHADER, "FF.Copy.PosUV.vert");
			glAttachShader(texGtorProg, shader);
			glDeleteShader(shader);
			shader = resources::createShaderFromFile(GL_FRAGMENT_SHADER, "TextureGtor.Metal.frag");
			glAttachShader(texGtorProg, shader);
			glDeleteShader(shader);

			glLinkProgram(texGtorProg);

			uniSamplerInput1 = glGetUniformLocation(texGtorProg, "samInput");
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

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			//Perlin program
			CreatePerlinNoiseGtor();

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

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
			glDeleteTextures(9, permTex);
			glDeleteTextures(1, &gradTex);
		}

	protected:
		void generateTextureCombined(TextureNames dest)
		{
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN1]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN2]);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN3]);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN4]);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN5]);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN6]);
			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D, mTextures[PERLIN7]);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glColor3f(1, 0, 1);
			glUseProgram(texGtorProg);
			//glUniformMatrix4fv(uniHSCB, 1, false, getHSCB(0.0f, 1.0f, 1.375f, 2.5f));

			generateTexture(mTextures[dest], texSize);

			glUseProgram(0);
		}

		void generateTexture(GLuint texID, GLsizei texSize)
		{
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
			
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glViewport(0, 0, texSize, texSize);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(VertexPosUV), &quadVertices[0].x);
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, sizeof(VertexPosUV), &quadVertices[0].u);
			
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, quadIndices);

			glDisableClientState(GL_VERTEX_ARRAY);
			glClientActiveTexture(GL_TEXTURE0);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void generatePerlin(TextureNames dest, int seed, int octaves, float amp, float ampScale, float freq, float freqScale, float gamma)
		{
			//{
			//	GLenum err = glGetError();
			//	assert(err==GL_NO_ERROR);
			//}

			//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextures[dest], 0);
			//
			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			//glViewport(0, 0, texSize, texSize);

			//glClearColor(0, 0, 0, 1);

			//glMatrixMode(GL_PROJECTION);
			//glPushMatrix();
			//glLoadIdentity();
			//glMatrixMode(GL_MODELVIEW);
			//glPushMatrix();
			//glLoadIdentity();
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_1D, permTex[seed%9]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, gradTex);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			glColor3f(1, 0, 1);
			glUseProgram(perlinGtorProg);

			glUniform1i(uniOctaves,   octaves);
			glUniform1f(uniAmp,       amp);
			glUniform1f(uniAmpScale,  ampScale);
			glUniform1f(uniFreq,      freq);
			glUniform1f(uniFreqScale, freqScale);
			glUniform1f(uniGamma,     gamma);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}

			generateTexture(mTextures[dest], texSize);
			//glBegin(GL_QUADS);
			//	glTexCoord2f(0, 1);
			//	glVertex2f(-1,  1);
			//	glTexCoord2f(1, 1);
			//	glVertex2f( 1,  1);
			//	glTexCoord2f(1, 0);
			//	glVertex2f( 1, -1);
			//	glTexCoord2f(0, 0);
			//	glVertex2f(-1, -1);
			//glEnd();
			glUseProgram(0);

			{
				GLenum err = glGetError();
				assert(err==GL_NO_ERROR);
			}
			//glPopMatrix();
			//glMatrixMode(GL_PROJECTION);
			//glPopMatrix();

			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void downsample(GLuint tex, GLuint dest, GLuint dim)
		{
			//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dest, 0);
			//
			//glViewport(0, 0, dim, dim);

			//glClearColor(0, 0, 0, 1);

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
			generateTexture(dest, dim);
			//glBegin(GL_QUADS);
			//	glTexCoord2f(0, 1);
			//	glVertex2f(-1,  1);
			//	glTexCoord2f(1, 1);
			//	glVertex2f( 1,  1);
			//	glTexCoord2f(1, 0);
			//	glVertex2f( 1, -1);
			//	glTexCoord2f(0, 0);
			//	glVertex2f(-1, -1);
			//glEnd();
			//glDisable(GL_TEXTURE_2D);

			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
			
			glUseProgram(0);
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

			generatePerlin(PERLIN0, 8, 6, 1.86f, 0.75f, 4.0f, 2.0f, 2.050f);
			generatePerlin(PERLIN1, 1, 5, 0.50f, 0.32f, 4.0f, 2.0f, 2.120f);
			generatePerlin(PERLIN2, 5, 6, 1.86f, 0.75f, 4.0f, 2.0f, 2.050f);
			generatePerlin(PERLIN3, 5, 1, 1.86f, 0.75f, 4.0f, 2.0f, 2.050f);
			generatePerlin(PERLIN4, 5, 6, 1.36f, 0.75f, 4.0f, 2.0f, 1.090f);
			generatePerlin(PERLIN5, 5, 1, 1.36f, 0.75f, 4.0f, 2.0f, 1.090f);
			generatePerlin(PERLIN6, 0, 6, 0.68f, 1.00f,   8.0f, 2.0f, 1.000f);
			generatePerlin(PERLIN7, 0, 2, 1.00f, 1.00f, 128.0f, 2.0f, 1.000f);

			generateTextureCombined(COMBINED0);

			mGPUTimer.stop();
			mCPUTime = mCPUTimer.elapsed();
			mGPUTime = mGPUTimer.getResult();
			mCPUTimer.stop();

			//downsample(mTextures[PERLIN0],  blurTex[0], texSize/2);
			//downsample(blurTex[0], blurTex[1], texSize/4);
			//downsample(blurTex[1], blurTex[2], texSize/8);
			//downsample(blurTex[2], blurTex[3], texSize/16);
			//downsample(blurTex[3], blurTex[4], texSize/32);
			//downsample(blurTex[4], blurTex[5], texSize/64);
			//downsample(blurTex[5], blurTex[6], texSize/128);
			//downsample(blurTex[6], blurTex[7], texSize/256);
			//downsample(blurTex[7], blurTex[8], texSize/512);
			//downsample(blurTex[8], blurTex[9], texSize/1024);

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
			drawImage(  4,  2, PERLIN4);
			drawImage(  7,  2, PERLIN5);

			drawImage( -8, -1, PERLIN6);
			drawImage( -5, -1, PERLIN7);
			drawImage( -2, -1, COMBINED0);
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