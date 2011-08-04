#include <framework.h>
#include <glm\glmext.h>
#include <cassert>
#include <ResourceHelpers.h>
#include <SOIL.h>

class FirstPersonCamera1
{
	public:
		glm::mat4	viewMatrix;
		glm::vec3	pos;
	public:
		FirstPersonCamera1()
		{
			viewMatrix = glm::mat4(1,0,0,0,
								   0,1,0,0,
								   0,0,1,0,
								   0,0,0,1);
		}

		void set(const glm::vec3& _pos, const glm::vec3& lookAt)
		{
			//pos = _pos;
			//viewMatrix = glm::lookAtGTX(pos, lookAt, glm::vec3(0, 1, 0));
		}

		void rotateLR(float angle)
		{
			viewMatrix = glm::mat4GTX(glm::angleAxisGTX(-angle, 0.0f, 1.0f, 0.0f)) * viewMatrix;
		}
		void rotateUD(float angle)
		{
			viewMatrix = glm::mat4GTX(glm::angleAxisGTX(angle, /*1.0f, 0.0f, 0.0f)*/glm::vec3(viewMatrix[0]))) * viewMatrix;
		}
		glm::mat4 getViewMatrix()
		{
			return /*glm::inverseGTX(glm::translate3DGTX(viewMatrix, -1.0f*pos));//*/glm::inverseGTX(viewMatrix);
			//return viewMatrix;
		}
		void moveFB(float dist)
		{
			pos+=glm::vec3(viewMatrix[2])*(-dist);
		}
		void moveLR(float dist)
		{
			pos+=glm::vec3(viewMatrix[0])*(-dist);
		}
		void moveUD(float dist)
		{
			pos+=glm::vec3(viewMatrix[1])*(-dist);
		}

		//void move(glm::vec3 vec)
		//{
		//}

		void rotate(float psi, float phi)
		{
			rotateLR(psi);
			rotateUD(phi);
		}
};

#include "CDLODTerrain.h"

class Exest: public ui::SDLStage
{
	private:
		FirstPersonCamera1	camera;

		VFS				mVFS;

		glm::mat4	mProj, VP;

		float vertAngle, horzAngle;
		CDLODTerrain terrain;

		mt::Task		mUpdateTask;
		ui::ProfileStatsBox		mStatsBox;

	public:
		Exest(): fixedMode(false), wireframeMode(false)
		{
			VFS::mount("..\\..\\AppData");

			terrain.initialize();

			mProj = glm::perspectiveGTX(30.0f, mWidth/mHeight, 0.1f, 1000.0f);
			camera.rotateLR(180);

			terrain.viewPoint = glm::vec3(0, 0, 0);
			glm::mat4 lookAt = glm::lookAtGTX<float>(terrain.viewPoint, glm::vec3(0, 0, 10), glm::vec3(0, 1, 0));
			glm::mat4 proj = glm::perspectiveGTX<float>(33.0f, 1.33333333f, 0.1f, 1200.0f);

			VP = proj*lookAt;

			terrain.setSelectMatrix(VP);

			mt::addTimedTask<Exest, &Exest::handleInput>(this, 20);

			File	src = VFS::openRead("heightmap_small.tga");
			//explicit conversion to avoid warning on 32-bit system
			assert(src.size()<SIZE_MAX);
			size_t fileSize = (size_t)src.size();

			unsigned char* data = new unsigned char[fileSize+1];
			src.read(data, fileSize, 1);

			int	imgWidth, imgHeight, imgChannels;
			unsigned char*	pixelsPtr = SOIL_load_image_from_memory(data, fileSize,
				&imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_L);
			assert(imgChannels==1);
			terrain.setHeightmap(pixelsPtr, imgWidth, imgHeight);

			SOIL_free_image_data(pixelsPtr);
			delete [] data;

			memset(prevKeystate, 0, SDLK_LAST);

			add(mStatsBox.add(mSelectTimeLabel)
				  .add(mDrawTimeLabel)
				  .setPos(10, 10)
				  .setSize(300, 120)
			);
			
			mt::addTimedTask<Exest, &Exest::onUpdateStats>(this, 250);
		}
		
		ui::Label	mDrawTimeLabel;
		ui::Label	mSelectTimeLabel;

		~Exest()
		{
			terrain.cleanup();
		}

		void drawFrustum(const glm::mat4& frustum)
		{
			glm::mat4 inverted = glm::inverseGTX(frustum);

			static const glm::vec4 points[8] = {
				glm::vec4(-1, -1, -1, 1), 
				glm::vec4( 1, -1, -1, 1), 
				glm::vec4( 1,  1, -1, 1), 
				glm::vec4(-1,  1, -1, 1), 
				glm::vec4(-1, -1,  1, 1), 
				glm::vec4( 1, -1,  1, 1), 
				glm::vec4( 1,  1,  1, 1), 
				glm::vec4(-1,  1,  1, 1), 
			};

			glm::vec4 wpoints[8];

			for (size_t i=0; i<8; ++i)
				wpoints[i] = inverted*points[i];

			glUseProgram(0);
			glColor3f(1, 0, 0);
			glBegin(GL_LINES);
				glVertex4fv(wpoints[0]); glVertex4fv(wpoints[1]);
				glVertex4fv(wpoints[1]); glVertex4fv(wpoints[2]);
				glVertex4fv(wpoints[2]); glVertex4fv(wpoints[3]);
				glVertex4fv(wpoints[3]); glVertex4fv(wpoints[0]);

				glVertex4fv(wpoints[4]); glVertex4fv(wpoints[5]);
				glVertex4fv(wpoints[5]); glVertex4fv(wpoints[6]);
				glVertex4fv(wpoints[6]); glVertex4fv(wpoints[7]);
				glVertex4fv(wpoints[7]); glVertex4fv(wpoints[4]);

				glVertex4fv(wpoints[0]); glVertex4fv(wpoints[4]);
				glVertex4fv(wpoints[1]); glVertex4fv(wpoints[5]);
				glVertex4fv(wpoints[2]); glVertex4fv(wpoints[6]);
				glVertex4fv(wpoints[3]); glVertex4fv(wpoints[7]);
			glEnd();
		}

	protected:
		void onPaint()
		{
			glClearDepth(1.0);
			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadMatrixf(mProj);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glm::mat4 view = camera.getViewMatrix();

			glLoadMatrixf(view);
			glm::mat4 xm;
			glGetFloatv(GL_MODELVIEW_MATRIX, xm);
			glTranslatef(-camera.pos[0], -camera.pos[1], -camera.pos[2]);

			glGetFloatv(GL_MODELVIEW_MATRIX, xm);

			glDisable(GL_CULL_FACE);

			glUseProgram(0);
			glBegin(GL_TRIANGLES);
				glColor3f(1, 0, 0);
				glVertex3f(-3, -1, -10);
				glColor3f(0, 1, 0);
				glVertex3f( 3, -1, -10);
				glColor3f(0, 0, 1);
				glVertex3f( 0, 4, -10);
			glEnd();

			if (!fixedMode)
			{
				terrain.viewPoint = camera.pos;
				terrain.setSelectMatrix(glm::translate3DGTX(mProj*camera.getViewMatrix(), -camera.pos));
			}
			else
			{
				terrain.viewPoint = VPpp;
				terrain.setSelectMatrix(VP);
				drawFrustum(VP);
			}

			terrain.setMVPMatrix(glm::translate3DGTX(mProj*camera.getViewMatrix(), -camera.pos));

			terrain.drawTerrain();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();

			glDisable(GL_BLEND);

			GLenum err = glGetError();
			assert(err==GL_NO_ERROR);
		}

		void onUpdateStats()
		{
			mStatsBox.setStats(terrain.getCPUTime(), terrain.getGPUTime());
			wchar_t str[256];
			_snwprintf(str, 256, L"CPU select time - %f ms", terrain.getCPUSelectTime());
			mSelectTimeLabel.setText(str);
			_snwprintf(str, 256, L"CPU draw time - %f ms", terrain.getCPUDrawTime());
			mDrawTimeLabel.setText(str);
		}

		bool fixedMode, wireframeMode;
		glm::vec3 VPpp;
		uint8_t prevKeystate[SDLK_LAST];

		void handleInput()
		{
			uint8_t *keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_ESCAPE])
				close();
			if (keystate[SDLK_w])
				camera.moveFB(keystate[SDLK_LSHIFT]?5.1f:0.5f);
			if (keystate[SDLK_s])
				camera.moveFB(keystate[SDLK_LSHIFT]?-5.1f:-0.5f);
			if (keystate[SDLK_a])
				camera.moveLR(keystate[SDLK_LSHIFT]?5.1f:0.5f);
			if (keystate[SDLK_d])
				camera.moveLR(keystate[SDLK_LSHIFT]?-5.1f:-0.5f);
			if (keystate[SDLK_KP8])
				camera.rotateUD(1.5);
			if (keystate[SDLK_KP5])
				camera.rotateUD(-1.5);
			if (keystate[SDLK_KP4])
				camera.rotateLR(-1.5);
			if (keystate[SDLK_KP6])
				camera.rotateLR(1.5);

			bool lockView = false;
			if (keystate[SDLK_l]&&!prevKeystate[SDLK_l])
			{
				fixedMode = !fixedMode;
				lockView  = fixedMode==true;
			}

			if (keystate[SDLK_f]&&!prevKeystate[SDLK_f])
			{
				wireframeMode = !wireframeMode;
				terrain.drawWireframe = wireframeMode;
			}

			if (lockView)
			{
				printf("%d\n", fixedMode);
				VP = glm::translate3DGTX(mProj*camera.getViewMatrix(), -camera.pos);
				VPpp = camera.pos;
			}

			memcpy(prevKeystate, keystate, SDLK_LAST);
		}
};

int main(int argc, char** argv)
{
	Exest app;
	app.run();

	return 0;
}