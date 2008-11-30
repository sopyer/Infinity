#include <framework.h>
#include "ship_data.h"
#include <FTGLTextureFont.h>

class Frest: public Framework
{
	private:
		VG				mVG;
		gl::Context		mContext;

		TextureManager	mTextureMgr;
		FontManager		mFontMgr;
		ProgramManager	mProgramMgr;
		ShaderManager	mShaderMgr;

		FontRef			mTextFont;
		TextureRef	image;
		TextureRef	highlight;
		ProgramRef	mProg;

		glDisplayList	ship;
		
		GLint mImageLoc, mHighlightLoc;

		float xangle, yangle;
		bool	mShowHelp;
	protected:

		void loadShip()
		{
			ship.begin();
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
			ship.end();
		}

		void OnCreate()
		{
			image.create("texture1.tga");
			highlight.create("highlight.tga");

			loadShip();

			mShowHelp = true;

			xangle = 0;
			yangle = 0;
			
			mTextFont.create("C:\\WINDOWS\\Fonts\\times.ttf 16px");

			mProg.create("shader.vert;shader.frag");
			mImageLoc = mProg->bindUniform("image");
			mHighlightLoc = mProg->bindUniform("highlight");

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, (float)width_/height_,1,2048); 
		}
		
		void OnRender()
		{
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, -6.0f);
			glRotatef(xangle, 1.0f, 0.0f, 0.0f);
			glRotatef(yangle, 0.0f, 1.0f, 0.0f);

			mContext.begin();
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_TEXTURE_2D);

				if (image) image->setWrap(GL_CLAMP, GL_CLAMP);
				if (highlight) image->setWrap(GL_CLAMP, GL_CLAMP);

				mContext.setTexture(GL_TEXTURE0, image.get());
				mContext.setTexture(GL_TEXTURE1, highlight.get());
				
				mContext.useProgram(*mProg);
				mContext.setUniform1i(mImageLoc, 0);
				mContext.setUniform1i(mHighlightLoc, 1);

				mContext.callCommandList(ship);

				mContext.useProgram(0);

				mContext.setTexture(GL_TEXTURE1, NULL);
				mContext.setTexture(GL_TEXTURE0, NULL);
			mContext.end();
			
			if (mShowHelp)
			{
				mVG.begin();
					//! Temporary hack to change font color
					glColor3f(0.3f,1.0f,0.5f);
					mVG.drawText(*mTextFont, 0, 20, "Use cursor keys or mouse to rotate");
					mVG.drawText(*mTextFont, 0, 40, "Press H to toggle help");
					mVG.drawText(*mTextFont, 0, 60, "Press ESC to exit");
				mVG.end();
			}

			glFlush();
		}

		void OnUpdate(float frame_time)
		{
			static float lastTime = 0;
			static bool  pressed = false;
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();

			bool manual = false;

			if (glfwGetKey(GLFW_KEY_RIGHT))
			{
				manual = true;
				yangle += 50.0f * (frame_time-lastTime);
			}
			if (glfwGetKey(GLFW_KEY_LEFT))
			{
				manual = true;
				yangle -= 50.0f * (frame_time-lastTime);
			}
			if (glfwGetKey(GLFW_KEY_UP))
			{
				manual = true;
				xangle -= 50.0f * (frame_time-lastTime);;
			}
			if (glfwGetKey(GLFW_KEY_DOWN))
			{
				manual = true;
				xangle += 50.0f * (frame_time-lastTime);;
			}

			if (glfwGetKey('H'))
			{
				pressed = true;
			}
			else if (pressed)
			{
				mShowHelp = !mShowHelp;
				pressed = false;
			}

			static int look = 0;
			
			if(!look && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) 
			{
				glfwDisable(GLFW_MOUSE_CURSOR);
				glfwSetMousePos(width_ / 2, height_ / 2);
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
				float psi = (mouseX - width_ / 2)*50.0f;
				float phi = (mouseY - height_ / 2)*50.0f;
				yangle += psi * (frame_time-lastTime);
				xangle += phi * (frame_time-lastTime);
				
				//if(phi < -89) phi = -89;
				//if(phi > 89) phi = 89;
				//camera.rotateLR(psi);
				//camera.rotateUD(-phi);
				glfwSetMousePos(width_ / 2, height_ / 2);
			} 

			if (!manual)
			{
				xangle += 50.0f * (frame_time-lastTime);
				yangle += 20.0f * (frame_time-lastTime);
			}

			if(xangle >= 360.0f)
			{
				xangle -= 360.0f;
			}

			if(xangle <= -360.0f)
			{
				xangle += 360.0f;
			}

			if(yangle >= 360.0f)
			{
				yangle -=360.0f;
			}

			if(yangle <= -360.0f)
			{
				yangle +=360.0f;
			}
			
			lastTime = frame_time;
		}
	public:
		Frest(): mVG(width_, height_) {}
};

int main()
{
	Frest app;
	app.run();
	return 0;
}