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

class AOSample: public UI::SDLStage
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
			
			mVoxelLookupTextureUniform = glGetUniformLocation(mVoxelizationProgram, "uVoxelLookupTexture");

			mFont = sui::createFont("C:\\WINDOWS\\Fonts\\times.ttf");
			mFont.setSize(16);

			add(mRenderAsVoxelsCB.setChecked(false)
				  .setText(L"Render as voxels")
				  .setFont(mFont)
				  .setPos(5, 5)
				  .setSize(16, 16));
		}
		
		~AOSample()
		{
			sui::destroyFont(mFont);
			glDeleteProgram(mVoxelizationProgram);
			glDeleteProgram(mLightProgram);
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

		virtual void onPaint()
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0, 0, -10);
			glRotatef(25, 1, 0, 0);
			glRotatef(40, 0, 1, 0);
			
			const GLfloat	rangeLeft=-2, rangeRight=2,
							rangeBottom=-2, rangeTop=2,
							rangeFront=-8, rangeBack=-12,
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
				glVertexPointer(3, GL_FLOAT, 0, cubeVertices);

				glDrawElements(GL_TRIANGLES, ARRAY_SIZE(cubeIndices), GL_UNSIGNED_INT, cubeIndices);
				
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

			glUseProgram(mLightProgram);
			glEnableClientState(GL_NORMAL_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
			glNormalPointer(GL_FLOAT, 0, cubeNormals);
			if (!mRenderAsVoxelsCB.isChecked())
			{
				glDrawElements(GL_TRIANGLES, ARRAY_SIZE(cubeIndices), GL_UNSIGNED_INT, cubeIndices);
			}
			else
			{
				glLoadIdentity();//Should be ViewMatrix;
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
								glDrawElements(GL_TRIANGLES, ARRAY_SIZE(cubeIndices), GL_UNSIGNED_INT, cubeIndices);
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
		sui::Font		mFont;
		ui::CheckBox	mRenderAsVoxelsCB;
		GLuint			mVoxelField[32][32];
		GLint			mVoxelLookupTextureUniform;
		GLuint			mVoxelLookupTexture;
		GLuint			mLightProgram;
		GLuint			mVoxelizationProgram;
		VFS				mVFS;
};

extern "C" int main(int argc, char** argv)
{
	ui::init();
	{
		AOSample app;
		app.run();
	}
	ui::cleanup();

	return 0;
}