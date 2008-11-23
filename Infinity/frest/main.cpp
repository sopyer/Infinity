#include <framework.h>
#include "ship_data.h"
#define FTGL_LIBRARY_STATIC
#include <FTGLTextureFont.h>

class Frest: public Framework
{
	private:
		TextureManager	texMgr;
		FTFont*	times;
		glRenderer	renderer_;
		glDisplayList	ship;
		TextureRef	image;
		TextureRef	highlight;
		float xangle, yangle;
		bool	mShowHelp;
	protected:
		void loadShip()
		{
			image.create("texture1.tga");
			highlight.create("highlight.tga");

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
			mShowHelp = true;
			times = new FTGLTextureFont("C:\\WINDOWS\\Fonts\\times.ttf");
			times->FaceSize(16);
			xangle = 0;
			yangle = 0;
			//vfsAddRoot(".\\Data");
			loadShip();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, (float)width_/height_,1,2048); 
			//makeOrtho();
		}
		
		void OnDestroy()
		{
			delete times;
		}

		void makeOrtho()
		{
			int w = width_;
			int h = height_;
			GLdouble aspect = (GLdouble)w / (GLdouble)h;
			// Use the whole window.
			//glViewport(0, 0, w, h);
			// We are going to do some 2-D orthographic drawing.
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			GLdouble size = (GLdouble)((w >= h) ? w : h) / 2.0;

			if (w <= h)
			{
				aspect = (GLdouble)h/(GLdouble)w;
				glOrtho(-size, size, -size*aspect, size*aspect, -100000.0, 100000.0);
			}
			else
			{
				aspect = (GLdouble)w/(GLdouble)h;
				glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
			}
			// Make the world and window coordinates coincide so that 1.0 in
			// model space equals one pixel in window space.
			glScaled(aspect, aspect, 1.0);
			// Now determine where to draw things.
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void OnRender()
		{
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glEnable(GL_DEPTH_TEST);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, -6.0f);
			glRotatef(xangle, 1.0f, 0.0f, 0.0f);
			glRotatef(yangle, 0.0f, 1.0f, 0.0f);
			//print("DrawShip\n");
			//print("%x\n", ship->handle_);
			renderer_.beginRenderPass();
				glEnable(GL_TEXTURE_2D);
				if (image)
					image->setWrapMode(GL_CLAMP, GL_CLAMP);
				if (highlight)
					image->setWrapMode(GL_CLAMP, GL_CLAMP);
				//Setup the first texture unit.
				renderer_.setTexture(GL_TEXTURE0, image.get());
				//Enable 2D textures.
				//Use sphere mapping auto coord generation.
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				//Turn on auto coord generation. N.B.! This is a per texture unit operation here!
				//Confused me at first, I thought glEnable was global, but this value is local to each
				//Texture unit!!!
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				//Multiply this texture by the ships material colors.
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//Why GL_MULT is not work

				//Setup the 2nd texture unit.
				renderer_.setTexture(GL_TEXTURE1, highlight.get());
				//Enable 2D textures.
				glEnable(GL_TEXTURE_2D);
				//Again setup sphere mapping, and turn it on.
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				//Set to additive. Where the centre (singularity) of the texture appears, will add white
				//Onto the ship, giving a specular "white out" effect.
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
				renderer_.callDisplayList(ship);
				renderer_.setTexture(GL_TEXTURE1, NULL);
				renderer_.setTexture(GL_TEXTURE0, NULL);
				glDisable(GL_TEXTURE_GEN_S);
				glDisable(GL_TEXTURE_GEN_T);
			renderer_.endRenderPass();
			
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (mShowHelp)
			{
				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
				makeOrtho();
				renderer_.beginRenderPass();
				{
					glColor3f(0.3,1,0.5);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glDisable(GL_DEPTH_TEST);
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glTranslatef(-width_/2, height_/2, 0.0);
					float x1, y1, z1, x2, y2, z2;
					/*FTBBox bb = */times->BBox("Hello world", x1, y1, z1, x2, y2, z2);
					glm::vec2 low(x1, y1), up(x2, y2), pos(0,0);
					//pos = -(up-low)/2.0f+pos;
					//glTranslatef(pos.x, pos.y, 0);
					glPushMatrix();
					glTranslatef(0, -20, 0);
					times->Render("Use cursor keys to rotate");
					glPopMatrix();
					//glTranslatef(0, -10, 0);
					//times->Render("or mouse");
					glPushMatrix();
					glTranslatef(0, -40, 0);
					times->Render("or press left mouse button to rotate");
					glPopMatrix();
					glPushMatrix();
					glTranslatef(0, -80, 0);
					times->Render("Press H to toggle help");
					glPopMatrix();
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
				}
				renderer_.endRenderPass();
				glMatrixMode(GL_PROJECTION);
				glPopMatrix();
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
			else if (glfwGetKey(GLFW_KEY_LEFT))
			{
				manual = true;
				yangle -= 50.0f * (frame_time-lastTime);
			}
			else if (glfwGetKey(GLFW_KEY_UP))
			{
				manual = true;
				xangle -= 50.0f * (frame_time-lastTime);;
			}
			else if (glfwGetKey(GLFW_KEY_DOWN))
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
				float psi = (mouseX - width_ / 2)*50;
				float phi = (mouseY - height_ / 2)*50;
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
};

int main()
{
	Frest app;
	app.run();
	return 0;
}