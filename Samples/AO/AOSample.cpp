#include <framework.h>

float	cubeVertices[][3] = {
		//Top plane
		{-1,  1,  1}, { 1,  1,  1}, { 1,  1, -1}, {-1,  1, -1}, 
		//Bottom plane
		{-1, -1,  1}, { 1, -1,  1}, { 1, -1, -1}, {-1, -1, -1},
		//Left plane
		{-1, -1,  1}, {-1, -1, -1}, {-1,  1, -1}, {-1,  1,  1},
		//Right plane
		{ 1, -1,  1}, { 1, -1, -1}, { 1,  1, -1}, { 1,  1,  1},
		//Front plane
		{-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, {-1,  1,  1},
		//Back plane
		{-1, -1, -1}, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1},
	};

float	cubeNormals[][3] = {
		//Top plane
		{ 0,  1,  0}, { 0,  1,  0}, { 0,  1,  0}, { 0,  1,  0},
		//Bottom plane
		{ 0, -1,  0}, { 0, -1,  0}, { 0, -1,  0}, { 0, -1,  0},
		//Left plane
		{-1,  0,  0}, {-1,  0,  0}, {-1,  0,  0}, {-1,  0,  0},
		//Right plane
		{ 1,  0,  0}, { 1,  0,  0}, { 1,  0,  0}, { 1,  0,  0},
		//Front plane
		{ 0,  0,  1}, { 0,  0,  1}, { 0,  0,  1}, { 0,  0,  1},
		//Back plane
		{ 0,  0, -1}, { 0,  0, -1}, { 0,  0, -1}, { 0,  0, -1},
	};

u32		cubeIndices[] = {
		 0,  1,  2,  0,  2,  3,
		 4,  5,  6,  4,  6,  7,
		 8,  9, 10,  8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};

float	planeVertices[][3] = {{-1, 0, -1}, {1, 0, -1}, {1, 0, 1}, {-1, 0, 1}};
float	planeNormals[][3] = {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}};
u32		planeIndices[] = {0, 1, 2, 0, 2, 3};

struct Mesh
{
	u32				numVertices;
	const float*	vertices;
	const float*	normals;
	u32				numIndices;
	u32*			indices;

	//In debug should have vertex declaration && buffers in release should use VAO
	//Should have some helper to parse current VAO
	//Should have DIP parameters
	
	void	setupArrays()
	{
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glNormalPointer(GL_FLOAT, 0, normals);
	}

	void	render()
	{
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);
	}
};

Mesh	cube  = {0, (float*)cubeVertices, (float*)cubeNormals, ARRAY_SIZE(cubeIndices), cubeIndices};
Mesh	plane = {0, (float*)planeVertices, (float*)planeNormals, ARRAY_SIZE(planeIndices), planeIndices};

class AOSample: public ui::SDLStage
{
	public:
		static const GLuint VOXEL_DIM_X=32;
		static const GLuint VOXEL_DIM_Y=32;
		static const GLuint VOXEL_DIM_Z=32;

	public:
		AOSample()
		{
			VFS::mount("..\\AppData");
			vg::init();
			genVoxelLookupTexture();

			mLightProgram        = resources::createProgramFromFiles("SimpleLight.vert",  "SimpleLight.frag");
			mVoxelizationProgram = resources::createProgramFromFiles("Voxelization.vert", "Voxelization.frag");
			mGBufferProgram      = resources::createProgramFromFiles("SimpleGBuffer.vert", "SimpleGBuffer.frag");

			mVoxelLookupTextureUniform = glGetUniformLocation(mVoxelizationProgram, "uVoxelLookupTexture");

			add(mRenderAsVoxelsCB.setChecked(false)
				  .setText(L"Render as voxels")
				  .setPos(5, 5)
				  .setSize(16, 16));

			add(mPosImage.setPos(0, 30)
				.setSize(160, 120));
			add(mNormalImage.setPos(0, 160)
				.setSize(160, 120));

			createMRTObjects();

		}
		
		~AOSample()
		{
			destroyMRTObjects();
			glDeleteProgram(mVoxelizationProgram);
			glDeleteProgram(mLightProgram);
			glDeleteProgram(mGBufferProgram);
			glDeleteTextures(1, &mVoxelLookupTexture);
			vg::cleanup();
		}

	protected:
		void genVoxelLookupTexture()
		{
			u32 textureData[VOXEL_DIM_Z];

			for (u32 i=0; i<VOXEL_DIM_Z; ++i)
				textureData[i] = 1L<<i;

			mVoxelLookupTexture = resources::createSpecialTexture1D(
				GL_RGBA8, 32, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		}

		void renderScene()
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(0, 1.5f, 0);
			cube.setupArrays();
			cube.render();
			glPopMatrix();
			
			glPushMatrix();
			glScalef(2.5f, 1, 2.5f);
			plane.setupArrays();
			plane.render();
			glPopMatrix();
		}
		
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

		GLuint createRenderbuffer(GLenum type, GLsizei width, GLsizei height)
		{
			GLuint rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			return rbo;
		}

		//Use framebuffers just like layouts using shared components
		GLuint createFBO(GLuint rt0, GLuint rt1, GLuint depthRenderbuffer)
		{
			GLuint fbo;
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt0, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, rt1, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

			GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
			
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
		
		GLuint mFBO, mDepthRB, mPosTexture, mNormalTexture;
		void createMRTObjects()
		{
			GLsizei w=(GLsizei)mWidth, h=(GLsizei)mHeight;
			mPosTexture = createFBOTexture(GL_RGBA16F, w, h);
			mNormalTexture = createFBOTexture(GL_RGBA16F, w, h);
			mDepthRB = createRenderbuffer(GL_DEPTH_COMPONENT, w, h);
			
			mPosImage.setTexture(mPosTexture);
			mNormalImage.setTexture(mNormalTexture);

			mFBO = createFBO(mPosTexture, mNormalTexture, mDepthRB);
		}

		void destroyMRTObjects()
		{
			glDeleteFramebuffers(1, &mFBO);
			glDeleteRenderbuffers(1, &mDepthRB);
			glDeleteTextures(1, &mPosTexture);
			glDeleteTextures(1, &mNormalTexture);
		}
		
		void renderFBO()
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
			
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_ALPHA_TEST);
			
			glUseProgram(mGBufferProgram);
			
			renderScene();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glPopAttrib();
		}

		virtual void onPaint()
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0, 0, -10);
			glRotatef(25, 1, 0, 0);
			glRotatef(40, 0, 1, 0);
			
			const GLfloat	rangeLeft=-4, rangeRight=4,
							rangeBottom=-4, rangeTop=4,
							rangeFront=-6, rangeBack=-14,
							stepX = (rangeRight-rangeLeft)/VOXEL_DIM_X,
							stepY = (rangeTop-rangeBottom)/VOXEL_DIM_Y,
							stepZ = (rangeBack-rangeFront)/VOXEL_DIM_Z;

			glEnableClientState(GL_VERTEX_ARRAY);
			
			static bool firstPass = true;
			if (firstPass)
			{
				firstPass = false;
				GLint vp[4];
				
				glGetIntegerv(GL_VIEWPORT, vp);
				
				glViewport(0, 0, VOXEL_DIM_X, VOXEL_DIM_Y);

				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
				glLoadMatrixf(glm::ortho3DGTX<float>(
					rangeLeft, rangeRight,
					rangeBottom, rangeTop,
					rangeFront, rangeBack)
				);
				glPushAttrib(GL_ALL_ATTRIB_BITS);

				glClearColor(0, 0, 0, 0);
				glClear(GL_COLOR_BUFFER_BIT);
				
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);
				glDisable(GL_STENCIL_TEST);
				glDisable(GL_ALPHA_TEST);
				
				glEnable(GL_COLOR_LOGIC_OP);
				glLogicOp(GL_OR);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_1D, mVoxelLookupTexture);
				glUseProgram(mVoxelizationProgram);
				glUniform1i(mVoxelLookupTextureUniform, 0);
				
				renderScene();

				glBindTexture(GL_TEXTURE_1D, 0);
				glDisable(GL_COLOR_LOGIC_OP);
				glPopAttrib();
				glPopMatrix();

#if defined(_DEBUG) || defined(DEBUG)
				resources::saveScreenToFile("VoxelSlices.tga");
#endif
				
				glReadPixels(0, 0, VOXEL_DIM_X, VOXEL_DIM_Y, GL_RGBA, GL_UNSIGNED_BYTE, mVoxelField);

				glClear(GL_COLOR_BUFFER_BIT);
				glMatrixMode(GL_MODELVIEW);
				glViewport(vp[0], vp[1], vp[2], vp[3]);
			}

			glEnableClientState(GL_NORMAL_ARRAY);

			renderFBO();

			if (!mRenderAsVoxelsCB.isChecked())
			{
				glUseProgram(mLightProgram);
				renderScene();
			}
			else
			{
				glLoadIdentity();//Should be ViewMatrix;
				glUseProgram(mLightProgram);
				cube.setupArrays();
				for (size_t i=0; i<VOXEL_DIM_Y; ++i)
				{
					for (size_t j=0; j<VOXEL_DIM_X; ++j)
					{
						for (size_t k=0; k<VOXEL_DIM_Z; ++k)
						{
							if (mVoxelField[i][j]&(1L<<k))
							{
								//Pay attention!!! We use left bottom, as GL use left bottom for texture origin
								//That's why [0][0] is also left bottom
								glPushMatrix();
								glTranslatef(
									rangeLeft  +stepX*(j+0.5f),
									rangeBottom+stepY*(i+0.5f),
									rangeFront +stepZ*(k+0.5f)
								);
								glRotatef(45, 1, 0, 0);
								glRotatef(45, 0, 1, 0);
								glScalef(0.5f*abs(stepX), 0.5f*abs(stepY), 0.5f*abs(stepZ));

								cube.render();
	
								glPopMatrix();
							}
						}
					}
				}
			}
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glPopAttrib();
		}

	private:
		ui::CheckBox	mRenderAsVoxelsCB;
		ui::Image		mPosImage;
		ui::Image		mNormalImage;
		GLuint			mVoxelField[32][32];
		GLint			mVoxelLookupTextureUniform;
		GLuint			mVoxelLookupTexture;
		GLuint			mLightProgram;
		GLuint			mGBufferProgram;
		GLuint			mVoxelizationProgram;
		VFS				mVFS;
};

extern "C" int main(int argc, char** argv)
{
	AOSample app;
	app.run();

	return 0;
}