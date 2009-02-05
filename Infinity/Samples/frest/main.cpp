#include <framework.h>
#include "ship_data.h"
#include <FTGLTextureFont.h>
#include "ThirdPersonCamera.h"
#include <UI.h>

class Frest: public UI::Stage, public sigslot::has_slots<>
{
	private:
		VG				mVG;
		gl::Context		mContext;

		FontRef			mTextFont;
		TextureRef		mReflection;
		TextureRef		mHighlight;
		ProgramRef		mProg;

		gl::CommandList	mShip;
		
		GLint	mReflectionLoc;
		GLint	mHighlightLoc;

		float	mPsiDir, mPhiDir;
		bool	mManualRotation;

		ThirdPersonCamera	mCamera;
		float xangle, yangle;
		bool	mShowHelp;

		Timeline		mUpdater;

	protected:
		void compileShipList();
		virtual void paint();
		
		void onKeyDown(uint32 key)
		{
			switch (key)
			{
				case GLFW_KEY_ESC:
					close();
					break;
				case GLFW_KEY_RIGHT:
					mPhiDir = 1;
					mManualRotation = true;
					//yangle += 50.0f * (mTimer.getFrameTime());
					break;
				case GLFW_KEY_LEFT:
					mPhiDir = -1;
					mManualRotation = true;
					//yangle -= 50.0f * (mTimer.getFrameTime());
					break;
				case GLFW_KEY_UP:
					mPsiDir = -1;
					mManualRotation = true;
					//xangle -= 50.0f * (mTimer.getFrameTime());
					break;
				case GLFW_KEY_DOWN:
					mPsiDir = 1;
					mManualRotation = true;
					//xangle += 50.0f * (mTimer.getFrameTime());
					break;
				case 'H':
					mShowHelp = !mShowHelp;
					break;
			}
		}

		void onKeyUp(uint32 key)
		{
			switch (key)
			{
				case GLFW_KEY_RIGHT:
				case GLFW_KEY_LEFT:
				case GLFW_KEY_UP:
				case GLFW_KEY_DOWN:
					mManualRotation = false;
					break;
			}
		}

		void onUpdate(uint32 frame)
		{
			float timeSinceLastFrame = 0;
			static float lastTime = 0;
			static bool  pressed = false;
			float xangle = 0; yangle = 0;

			bool manual = false;

			static int look = 0;
			
			if(!look && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) 
			{
				glfwDisable(GLFW_MOUSE_CURSOR);
				glfwSetMousePos(mWidth / 2, mHeight / 2);
				look = 1;
			}
			else if (look && !glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
			{
				look = false;
				glfwEnable(GLFW_MOUSE_CURSOR);
			}

			if(look)
			{
				manual = true;
				int mouseX=0, mouseY=0;
				glfwGetMousePos(&mouseX, &mouseY);
				float psi = (mouseX - mWidth / 2)*0.05f;
				float phi = (mouseY - mHeight / 2)*0.05f;
				yangle += psi * (mTimer.getFrameTime());
				xangle += phi * (mTimer.getFrameTime());
				
				glfwSetMousePos(mWidth / 2, mHeight / 2);
			} 
			
			if (!mManualRotation)
			{
				xangle = mPhiDir * 0.05f * (mTimer.getFrameTime());
				yangle = mPsiDir * 0.02f * (mTimer.getFrameTime());
			}

			mCamera.rotate(xangle, yangle);
		}

	public:
		Frest():	mVG(mWidth, mHeight),
					mPsiDir(1.0f), mPhiDir(1.0f),
					mManualRotation(false), mShowHelp(true),
					mUpdater(33)
		{
			mUpdater.onFrame.connect(this, &Frest::onUpdate);

			mReflection.create("reflection_texture", "texture1.tga");
			mHighlight.create("highlight_texture", "highlight.tga");

			compileShipList();

			xangle = 0;
			yangle = 0;
			
			mTextFont.create("Times 16px", "C:\\WINDOWS\\Fonts\\times.ttf 16px");

			mProg.create("sph_map_shader", "shader.vert", "shader.frag");
			mReflectionLoc = mProg->bindUniform("reflection");
			mHighlightLoc  = mProg->bindUniform("highlight");
			
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, (float)mWidth/mHeight, 1, 400); 
		}
};

int main()
{
	Frest app;

	app.run();

	return 0;
}

void Frest::paint()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mCamera.getMatrix());

	mContext.begin();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		if (mReflection) mReflection->setWrap(GL_CLAMP, GL_CLAMP);
		if (mHighlight) mHighlight->setWrap(GL_CLAMP, GL_CLAMP);

		mContext.setTexture(GL_TEXTURE0, mReflection.get());
		mContext.setTexture(GL_TEXTURE1, mHighlight.get());
		
		mContext.useProgram(*mProg);
		mContext.setUniform1i(mReflectionLoc, 0);
		mContext.setUniform1i(mHighlightLoc, 1);

		mShip.execute();

		mContext.useProgram(0);

		mContext.setTexture(GL_TEXTURE1, NULL);
		mContext.setTexture(GL_TEXTURE0, NULL);
	mContext.end();
	
	if (mShowHelp)
	{
		mVG.begin();
			//! Temporary hack to change font color
			glColor3f(0.3f,1.0f,0.5f);
			mVG.drawText(*mTextFont, 0, 0, "Use cursor keys or mouse to rotate");
			mVG.drawText(*mTextFont, 0, 20, "Press H to toggle help");
			mVG.drawText(*mTextFont, 0, 40, "Press ESC to exit");

		mVG.end();
	}

	glFlush();
}

void Frest::compileShipList()
{
	mShip.begin();
	glBegin(GL_TRIANGLES);

	GLuint ref_count = 0;
	GLuint material_count = material_ref[ref_count][1];
	GLuint color_index = material_ref[ref_count][0];

	glColor3f(materials[color_index].diffuse[0], materials[color_index].diffuse[1], materials[color_index].diffuse[2]); 
	
	for(GLuint i=0; i<8686; i++)
	{
		if(!material_count)
		{
			ref_count++;
			material_count = material_ref[ref_count][1];
			color_index = material_ref[ref_count][0];

			glColor3f(materials[color_index].diffuse[0], materials[color_index].diffuse[1], materials[color_index].diffuse[2]); 
		}

		glNormal3f(normals[face_indicies[i][3]] [0], normals[face_indicies[i][3]] [1], normals[face_indicies[i][3]] [2]);
		glVertex3f(vertices[face_indicies[i][0]] [0], vertices[face_indicies[i][0]] [1], vertices[face_indicies[i][0]] [2]);
		
		glNormal3f(normals[face_indicies[i][4]] [0], normals[face_indicies[i][4]] [1], normals[face_indicies[i][4]] [2]);
		glVertex3f(vertices[face_indicies[i][1]] [0], vertices[face_indicies[i][1]] [1], vertices[face_indicies[i][1]] [2]);
		
		glNormal3f(normals[face_indicies[i][5]] [0], normals[face_indicies[i][5]] [1], normals[face_indicies[i][5]] [2]);
		glVertex3f(vertices[face_indicies[i][2]] [0], vertices[face_indicies[i][2]] [1], vertices[face_indicies[i][2]] [2]);

		material_count--;
	}
	glEnd();
	mShip.end();
}