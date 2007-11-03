#include <framework.h>
#include "ship_data.h"

class Frest: public Framework
{
	private:
		glRenderer	renderer_;
		glTexture2D	*image_[2];
		glDisplayList	*ship;
		float xangle, yangle;
	protected:
		void loadShip()
		{
			print("Textures\n");
			//Load the 2 textures up.
			image_[0] = loadTargaTexture("TEXTURE1.tga");
			image_[1] = loadTargaTexture("Highlight.tga");
			if(image_[0])
				image_[0]->setWrapMode(GL_CLAMP, GL_CLAMP);
			if(image_[1])
				image_[1]->setWrapMode(GL_CLAMP, GL_CLAMP);
			//Setup the first texture unit.
			renderer_.setTexture(GL_TEXTURE0, image_[0]);
			//Enable 2D textures.
			glEnable(GL_TEXTURE_2D);
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
			renderer_.setTexture(GL_TEXTURE1, image_[1]);
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
			print("DisplayList\n");
			//Make the ship display list here.
			ship = new glDisplayList();

			ship->begin();
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
			ship->end();
			print("%x\n", ship->handle_);
			print("Ship ok.\n");
		}

		void OnCreate()
		{
			xangle = 250;
			yangle = 30;
			//vfsAddRoot(".\\Data");
			loadShip();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, (float)width_/height_,1,2048); 
		}
		
		void OnDestroy()
		{
			delete ship;
			delete image_[0];
			delete image_[1];
		}

		void OnRender()
		{
			print("UpdateParams\n");
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glEnable(GL_DEPTH_TEST);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, -6.0f);
			glRotatef(xangle, 1.0f, 0.0f, 0.0f);
			glRotatef(yangle, 0.0f, 1.0f, 0.0f);
			print("DrawShip\n");
			print("%x\n", ship->handle_);
			//renderer_.beginRenderPass();
				renderer_.callDisplayList(ship);
			//renderer_.endRenderPass();
			glFlush();
		}

		void OnUpdate(float frame_time)
		{
			static float lastTime = 0;
			print("OnUpdateGetKey\n");
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();

			print("OnUpdateXANGLE\n");
			xangle += 20.0f * (frame_time-lastTime);
			
			if(xangle >= 360.0f)
			{
				xangle -= 360.0f;
			}

			yangle += 50.0f * (frame_time-lastTime);
			
			print("OnUpdateYANGLE\n");
			if(yangle >= 360.0f)
			{
				yangle -=360.0f;
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