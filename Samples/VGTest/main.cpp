#include <framework.h>

#include "VGExp.h"

//float w1 = -0.35f;
//float w2 = -0.35f;
//glm::vec3	controlPts[4] = {glm::vec3(0, 0, 1), glm::vec3(10*w1, 10*w1, w1), glm::vec3(0*w2, 10*w2, w2), glm::vec3(10, 0, 1)};
glm::vec2	controlPts2[4] = {glm::vec2(0, 0), glm::vec2(10.0f, 10.0f), glm::vec2(0, 10), glm::vec2(10, 0)};
//glm::vec2	controlPts2[4] = {glm::vec2(10, 0), glm::vec2(20.0f, 40.0f), glm::vec2(40, 40), glm::vec2(50, 0)};
float w1 = 1.0f/3.0f, w2 = 1.0f/3.0f;
glm::vec3 controlPts[4] = {
	glm::vec3(-10.0f,     0.0f,    1.0f),
	glm::vec3(-10.0f*w1, 20.0f*w1,   w1),
	glm::vec3( 10.0f*w2, 20.0f*w2,   w2),
	glm::vec3( 10.0f,     0.0f,    1.0f),
};

Geometry<RCubicVertex>	rcubic;

namespace ml
{
	template<typename T, typename U>
	T* as(U* value)
	{
		return reinterpret_cast<T*>(value);
	}
}

void evalRationalBezier(float* cp, float t, float* pt)
{
	glm::vec3* controlPts = ml::as<glm::vec3>(cp);
	glm::vec2& res = *ml::as<glm::vec2>(pt);

	float it = 1-t;
	glm::vec3	pp = it*it*it*controlPts[0]+3*it*it*t*controlPts[1]+3*it*t*t*controlPts[2]+t*t*t*controlPts[3];
	res = glm::vec2(pp)/pp.z;
}

bool checkUnitCircle(float* pt)
{
	glm::vec2& circlePt = *ml::as<glm::vec2>(pt);
	return ml::equalE(glm::length(circlePt), 1.0f, ml::EPS5);
}

void calcLinearFunctionals(float r[6], float koef[4])
{
	assert(false && "Debug me!!!!");

	float	t1=r[0], s1 = r[1],
			t2=r[2], s2 = r[3],
			t3=r[4], s3 = r[5];

	koef[0] =  t1*t2*t3;
	koef[1] = -t1*t2*s3-t1*s2*t3-s1*t2*t3;
	koef[2] =  s1*s2*t3+s1*t2*s3+t1*s2*s3;
	koef[3] = -s1*s2*s3;
}

void solveQuadratic(float k[3], int& count, float r[4])
{
	assert(false && "Debug me!!!!");

	float a = k[0], b = k[1], c = k[2];
	float D = b*b-4*a*c;

	if (D<0)
	{
		count = 0;
	}
	else
	{
		float rr = b + (b<0?-1:1)*sqrt(D);
		
		count = 2;

		if (D==0)
		{
			r[0] = r[2] = -b;
			r[1] = r[3] = 2*a;
		}
		else
		{
			r[0] = rr;
			r[1] = 2*a;
			r[2] = 2*c;
			r[3] = rr;
		}
	}
}

void solveCubic(float k[4], int& count, float r[6])
{
	float a=k[0], b=k[1], c=k[2], d=k[3];

	float u = 3*a*c-b*b;
	float v = 27*a*a*d-9*a*b*c+2*b*b*b;

	float j = 4*u*u*u+v*v;

	if (j<=0)
	{
		float s = sqrt(-u);
	
		float cs=0, sn=0;

		if (s!=0)
		{
			float t = -v/2/s/s/s;
			float ang = acos(t)/3;

			cs = cos(ang);
			sn = sqrt(3.0f)*sin(ang);
		}

		count = 3;

		r[0] = 2*s*cs-b;
		r[1] = 3*a;
		r[2] = s*(-cs+sn)-b;
		r[3] = 3*a;
		r[4] = s*(-cs-sn)-b;
		r[5] = 3*a;
	}
	else
	{
		float w = sqrt(j);
		float wv2 = (w+abs(v))/2.0f;

		wv2 = pow (wv2, 1.0f/3.0f);

		count = 1;
		r[0] = (v<0?-1:1)*(u/wv2-wv2)-b;
		r[1] = 3*a;
	}
}

	struct Determinants
	{
		float	D;
		float	d[4];
	};

	void calcDets(const Array<glm::vec2>& p, Determinants& dets)
	{
		assert(p.size()==4);

		/**********************************************************/
		/*	Alternative way of calculating di
		/*
		/*	//convert to power basis:
		/*	glm::vec3 b0(cp0);
		/*	glm::vec3 b1(-3.0f*cp0 + 3.0f*cp1);
		/*	glm::vec3 b2( 3.0f*cp0 - 6.0f*cp1 + 3.0f*cp2);
		/*	glm::vec3 b3(     -cp0 + 3.0f*cp1 - 3.0f*cp2 + cp3);
		/*
		/*	float d0 = glm::dot(b3, glm::cross(b2, b1));	
		/*	float d1 = -glm::dot(b3, glm::cross(b2, b0));	
		/*	float d2 = glm::dot(b3, glm::cross(b1, b0));	
		/*	float d3 = -glm::dot(b2, glm::cross(b1, b0));
		/**********************************************************/


		glm::vec3	cp[4] =
		{
			glm::vec3(p[0], 1),
			glm::vec3(p[1], 1),
			glm::vec3(p[2], 1),
			glm::vec3(p[3], 1)
		};

		float a[4] =
		{
			 glm::dot(cp[3], glm::cross(cp[2], cp[1])),
			-glm::dot(cp[3], glm::cross(cp[2], cp[0])),
			 glm::dot(cp[3], glm::cross(cp[1], cp[0])),
			-glm::dot(cp[2], glm::cross(cp[1], cp[0]))
		};

		dets.d[0] = 3*(a[3]+a[2]+a[1]+a[0]);
		dets.d[1] = 3*(3*a[3]+2*a[2]+a[1]);
		dets.d[2] = 3*(3*a[3]+a[2]);
		dets.d[3] = 3*(3*a[3]);

		//following expression has the same sign as d1*d1*(3*d2*d2 - 4*d1*d3)
		//and later it is reused as discriminant for quadratic equation
		dets.D = 3.0f*dets.d[2]*dets.d[2] - 4.0f*dets.d[1]*dets.d[3];
	}

float area(float* v0, float* v1, float* v2)
{
	return 0.5f*((v1[0]-v0[0])*(v1[1]+v0[1]) + 
				 (v2[0]-v1[0])*(v2[1]+v1[1]) + 
				 (v0[0]-v2[0])*(v0[1]+v2[1]));
}

void calcBarycentric(/*vec2*/float* v0, /*vec2*/float* v1, /*vec2*/float* v2,/*vec2*/float* pt, /*vec3*/float* bary)
{
	float triArea = area(v0, v1, v2);
	bary[0] = area(v1, v2, pt)/triArea;
	bary[1] = area(v2, v0, pt)/triArea;
	bary[2] = area(v0, v1, pt)/triArea;
}

void testRCubic()
{
	float w1 = 1.0f/3.0f, w2 = 1.0f/3.0f;

	Array<glm::vec2> pts;

	pts.pushBack(glm::vec2(0, 0));
	pts.pushBack(glm::vec2(0, 1));
	pts.pushBack(glm::vec2(1, 1));
	pts.pushBack(glm::vec2(1, 0));

	Determinants dett;
	calcDets(pts, dett);

	glm::vec3 cp[4] = {
		glm::vec3(-10.0f,     0.0f,    1.0f),
		glm::vec3(-10.0f*w1, 20.0f*w1,   w1),
		glm::vec3( 10.0f*w2, 20.0f*w2,   w2),
		glm::vec3( 10.0f,     0.0f,    1.0f),
	};

	glm::vec2 bzPt;

	float ttt[] = {0, 1, 0.5f, 0.75f, 0.25f, 0.333333f, 0.4f};

	for (int i=0; i<ARRAY_SIZE(ttt); ++i)
	{
		evalRationalBezier(ml::as<float>(cp), ttt[i], bzPt); 
		//assert(checkUnitCircle(bzPt));
	}

	//glm::vec3 cp[4] = {
	//	glm::vec3(0.0f, 0.0f, 1.0f),
	//	glm::vec3(0.0f*w1, 1.0f*w1, w1),
	//	glm::vec3(1.0f*w2, 1.0f*w2, w2),
	//	glm::vec3(1.0f, 0.0f, 1.0f),
	//};

	//glm::vec3 cp[4] = {
	//	glm::vec3(0.0f, 0.0f, 1.0f),
	//	glm::vec3(0.3333f*w1, 0.66667f*w1, w1),
	//	glm::vec3(5.3333f*w2, 0.66667f*w2, w2),
	//	glm::vec3(6.0f, 0.0f, 1.0f),
	//};

	float a[4] =
	{
		 glm::dot(cp[3], glm::cross(cp[2], cp[1])),
		-glm::dot(cp[3], glm::cross(cp[2], cp[0])),
		 glm::dot(cp[3], glm::cross(cp[1], cp[0])),
		-glm::dot(cp[2], glm::cross(cp[1], cp[0]))
	};

	glm::vec3 cp2[4] = {
		      cp[0],
		-3.0f*cp[0] +  3.0f*cp[1],
		 3.0f*cp[0] + -6.0f*cp[1] +  3.0f*cp[2],
		     -cp[0] +  3.0f*cp[1] + -3.0f*cp[2] +  cp[3],
	};

	float a2[4] =
	{
		 glm::dot(cp2[3], glm::cross(cp2[2], cp2[1])),
		-glm::dot(cp2[3], glm::cross(cp2[2], cp2[0])),
		 glm::dot(cp2[3], glm::cross(cp2[1], cp2[0])),
		-glm::dot(cp2[2], glm::cross(cp2[1], cp2[0]))
	};
	
	float d[4];

	d[0] = a2[0];//3*(3*a[3] + 3*a[2] + 3*a[1] + 3*a[0]);
	d[1] = a2[1];//3*(3*a[3] + 2*a[2] +   a[1]);
	d[2] = a2[2];//3*(3*a[3] +   a[2]);
	d[3] = a2[3];//3*(3*a[3]);

	glm::vec3 cr = cross(cp2[2], cp2[1]);

	float dt[3];

	dt[0] = d[0]*d[2]-d[1]*d[1];
	dt[1] = d[1]*d[2]-d[0]*d[3];
	dt[2] = d[1]*d[3]-d[2]*d[2];

	float det = 4*dt[0]*dt[2]-dt[1]*dt[1];

	float r[6];
	int   count;

	d[1] *= -3.0f;
	d[2] *=  3.0f;
	d[3]  = -d[3];
	
	glm::vec4 klmn[4];

	if (ml::equalE(dt[0], 0) && ml::equalE(dt[1], 0) && ml::equalE(dt[2], 0))
	{
		klmn[0] = glm::vec4(     0.0f,      0.0f,      0.0f, 1.0f);
		klmn[1] = glm::vec4(1.0f/3.0f,      0.0f, 1.0f/3.0f, 1.0f);
		klmn[2] = glm::vec4(2.0f/3.0f, 1.0f/3.0f, 2.0f/3.0f, 1.0f);
		klmn[3] = glm::vec4(     1.0f,      1.0f,      1.0f, 1.0f);
	}
	else if (det>=0)
	{
		solveCubic(d, count, r);
		int i = 0;
		++i;
	}
	else
	{
		//loop
		solveCubic(d, count, r);

		int i = 0;
		++i;
	}

	glm::vec2	tri[4] = {
		cp[0]._xy()/cp[0].z,
		cp[1]._xy()/cp[1].z,
		cp[2]._xy()/cp[2].z,
		cp[3]._xy()/cp[3].z,
	};

	glm::vec3 bary;
	glm::vec2 pt;


	for (int i=0; i<ARRAY_SIZE(ttt); ++i)
	{
		evalRationalBezier(ml::as<float>(cp),  ttt[i], pt);

		calcBarycentric(tri[0], tri[1], tri[3], pt, bary);
		assert(glm::equalEpsilonGTX(pt, bary.x*tri[0]+bary.y*tri[1]+bary.z*tri[3], ml::EPS7));

		glm::vec3 scale = bary/glm::vec3(cp[0].z, cp[1].z, cp[3].z);
		glm::vec4 klmnI = (scale.x*klmn[0] + scale.y*klmn[1] + scale.z*klmn[3])/(scale.x + scale.y + scale.z);

		float f = klmnI.x*klmnI.x*klmnI.x - klmnI.y*klmnI.z*klmnI.w;

		assert(ml::equalE(f, 0));
	}

	rcubic.vertices.resize(4);
	for (int i=0; i<4; ++i)
	{

		rcubic.vertices[i].pos = glm::vec4(cp[i].x, cp[i].y, 0, cp[i].z);
		rcubic.vertices[i].klmn = klmn[i];
	}

	rcubic.indices.pushBack(0); rcubic.indices.pushBack(1); rcubic.indices.pushBack(3);
	rcubic.indices.pushBack(1); rcubic.indices.pushBack(2); rcubic.indices.pushBack(3);
}

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

			testRCubic();
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
			glScalef(80, 80, 1);
			clearStencil();
			glPopMatrix();
			//rasterizeEvenOdd(mRationalCubic);
			//rasterizeEvenOdd(mRasterCubic);
			rasterizeEvenOdd(rcubic);
			glPushMatrix();
			//glScalef(-1, 1, 1);
			//drawRCubicAA(rcubic);
			//drawCubicAA(mRasterCubic);
			glPopMatrix();
			//rasterizeEvenOdd(mTri);
			glUseProgram(0);
			glPushMatrix();
			glTranslatef(-40, -40, 0);
			glScalef(80, 80, 1);
			drawQuad();
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
