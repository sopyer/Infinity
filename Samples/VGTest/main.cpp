#include <framework.h>

#include "VGExp.h"

float w1 = -0.35f;
float w2 = -0.35f;
glm::vec3	controlPts[4] = {glm::vec3(0, 0, 1), glm::vec3(10*w1, 10*w1, w1), glm::vec3(0*w2, 10*w2, w2), glm::vec3(10, 0, 1)};
glm::vec2	controlPts2[4] = {glm::vec2(0, 0), glm::vec2(10.0f, 10.0f), glm::vec2(0, 10), glm::vec2(10, 0)};
//glm::vec2	controlPts2[4] = {glm::vec2(10, 0), glm::vec2(20.0f, 40.0f), glm::vec2(40, 40), glm::vec2(50, 0)};

class VGTest: public UI::SDLStage
{
	public:
		VGTest():
			doMove(false),
			offsetX(mWidth/2),
			offsetY(mHeight/2),
			zoom(10)
		{
			initVGExp();

			tessellateCubic();

			Array<glm::vec2> cp;
			cp.assign(controlPts2, controlPts2+4);
			addCubic(mRasterCubic, mTri, cp);

			mFont = sui::createFont("C:\\WINDOWS\\Fonts\\times.ttf");
			mFont.setSize(16);

			wchar_t zoomStr[256];
			swprintf(zoomStr, 256, L"Zoom level %f", zoom);
			add(mZoomLabel.setText(zoomStr)
				  .setFont(mFont)
				  .setPos(300, 10));
			glClearStencil(0x80);
		}

		~VGTest()
		{
			terminateVGExp();
			sui::destroyFont(mFont);
		}
		
	protected:
		Geometry<CubicVertex>	mRasterCubic;
		Geometry<glm::vec2>		mRationalCubic, mTri;
		
		sui::Font		mFont;
		ui::Label		mZoomLabel;
		
		bool doMove;
		float offsetX;
		float offsetY;
		float zoom;
		
		void tessellateCubic()
		{
			int numPts = 300;
			float step = 1.0f/(numPts-1);

			mRationalCubic.vertices.resize(numPts);
			mRationalCubic.indices.resize((numPts-2)*3);
			int i=0;
			for (float t=0; t<=1; t+=step, ++i)
			{
				float it = 1-t;
				glm::vec3	pp = it*it*it*controlPts[0]+3*it*it*t*controlPts[1]+3*it*t*t*controlPts[2]+t*t*t*controlPts[3];
				mRationalCubic.vertices[i] = glm::vec2(pp)/pp.z;
				if (i>1)
				{
					mRationalCubic.indices[(i-2)*3+0] = 0;
					mRationalCubic.indices[(i-2)*3+1] = i-1;
					mRationalCubic.indices[(i-2)*3+2] = i;
				}
			}
		}

		void onPaint()
		{
			glEnable(GL_STENCIL_TEST);
			glTranslatef(offsetX, offsetY, 0);
			glScalef(zoom, zoom, 1);
			glPushMatrix();
			glTranslatef(-40, -40, 0);
			glScalef(70, 70, 1);
			//clearStencil();
			glPopMatrix();
			//rasterizeEvenOdd(mRationalCubic);
			//rasterizeEvenOdd(mRasterCubic);
			glPushMatrix();
			//glScalef(-1, 1, 1);
			drawCubicAA(mRasterCubic);
			glPopMatrix();
			//rasterizeEvenOdd(mTri);
			glUseProgram(0);
			glPushMatrix();
			glTranslatef(-40, -40, 0);
			glScalef(70, 70, 1);
			//drawQuad();
			glPopMatrix();
			glDisable(GL_STENCIL_TEST);
			glColor3f(1, 0, 0);
			glBegin(GL_LINE_STRIP);
			glVertex2f(controlPts[0].x/controlPts[0].z, controlPts[0].y/controlPts[0].z);
			glVertex2f(controlPts[1].x/controlPts[1].z, controlPts[1].y/controlPts[1].z);
			glVertex2f(controlPts[2].x/controlPts[2].z, controlPts[2].y/controlPts[2].z);
			glVertex2f(controlPts[3].x/controlPts[3].z, controlPts[3].y/controlPts[3].z);
			glEnd();
		}

		void onTouch(const ButtonEvent& event)
		{
			if (event.button == SDL_BUTTON_WHEELUP)
			{
				zoom *= 1.2f;
				offsetX -= (event.x-offsetX)*(1.2f - 1);
				offsetY -= (event.y-offsetY)*(1.2f - 1);
			}
			else if (event.button == SDL_BUTTON_WHEELDOWN)
			{
				zoom /= 1.2f;
				if (zoom<1.0f)
				{
					//fix it a bit
					offsetX -= (event.x-offsetX)*(1/zoom/1.2f - 1);
					offsetY -= (event.y-offsetY)*(1/zoom/1.2f - 1);
					zoom = 1.0f;
				}
				else
				{
					offsetX -= (event.x-offsetX)*(1/1.2f - 1);
					offsetY -= (event.y-offsetY)*(1/1.2f - 1);
				}
			}
			else
				doMove = true;

			wchar_t zoomStr[256];
			swprintf(zoomStr, 256, L"Zoom level %f", zoom);
			mZoomLabel.setText(zoomStr);
		}

		void onUntouch(const ButtonEvent& event)
		{
			doMove = false;
		}

		void onMotion(const MotionEvent& event)
		{
			if (doMove)
			{
				offsetX += event.xrel;
				offsetY += event.yrel;
			}
		}
};

int main(int argc, char** argv)
{
	ui::init();
	{
		VGTest app;
		app.run();
	}
	ui::cleanup();

	return 0;
}
