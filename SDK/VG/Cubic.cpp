#include "Cubic.h"
#include <cassert>
#include <algorithm>

namespace cubic
{
	float calcImplicit(const glm::vec3& tc)
	{
		return tc.x*tc.x*tc.x - tc.y*tc.z;
	}

	void calcDets(const glm::vec2 p[4], Determinants& dets/*float d[4], float& D*/)
	{
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

	//Serpentine and cusp with inflection at infinity
	void calcSerpentineCuspTC(Determinants& dets/*const float D, const float d[4]*/, glm::vec3 tc[4], float& t1, float& t2)
	{
		assert(dets.D>=0 && dets.d[1]!=0);

		/********************************************************/
		/*			Calculating koeficients in alternative way
		/*	float ls = d2 - sqrt(d2*d2 - 4.0f/3.0f*d1*d3);
		/*	float lt = 2*d1;
		/*	float ms = d2 + sqrt(d2*d2 - 4.0f/3.0f*d1*d3);
		/*	float mt = 2*d1;
		/*		
		/*	//Calculating linear factors
		/*	float L03 = ls;						float M03 = ms;
		/*	float L13 = ls - 1.0f/3.0f * lt;	float M13 = ms - 1.0f/3.0f * mt;
		/*	float L23 = ls - 2.0f/3.0f * lt;	float M23 = ms - 2.0f/3.0f * mt;
		/*	float L33 = ls - lt;				float M33 = ms - mt;
		/*
		/*	//Calculating functions k, l, m along cubic at positions (0,3), (1,3), (2,3), (3,3)
		/*	float k03 = L03*M03;	float l03 = L03*L03*L03;	float m03 = M03*M03*M03;
		/*	float k13 = L13*M13;	float l13 = L13*L13*L13;	float m13 = M13*M13*M13;
		/*	float k23 = L23*M23;	float l23 = L23*L23*L23;	float m23 = M23*M23*M23;
		/*	float k33 = L33*M33;	float l33 = L33*L33*L33;	float m33 = M33*M33*M33;
		/*
		/*	glm::vec4	kv(k03, k13, k23, k33);
		/*	glm::vec4	lv(l03, l13, l23, l33);
		/*	glm::vec4	mv(m03, m13, m23, m33);
		/*
		/*	glm::vec4	ic13(-5.0f/6.0f, 3.0f, -3.0f/2.0f, 1.0f/3.0f);
		/*	glm::vec4	ic23(1.0f/3.0f, -3.0f/2.0f, 3.0f, -5.0f/6.0f);
		/*
		/*	//Calculation linear functionals at control points
		/*	glm::vec3 texcoo03(-k03, -l03, m03);
		/*	glm::vec3 texcoo13(-glm::dot(kv, ic13), -glm::dot(lv, ic13), glm::dot(mv, ic13));
		/*	glm::vec3 texcoo23(-glm::dot(kv, ic23), -glm::dot(lv, ic23), glm::dot(mv, ic23));
		/*	glm::vec3 texcoo33(-k33, -l33, m33);
		/********************************************************/

		float dd = dets.D/3.0f;

		if (dets.d[2] == 0)
		{
			t1 = -sqrt(dd)/2.0f/dets.d[1];
			t2 = sqrt(dd)/2.0f/dets.d[1];
		}
		else
		{
			dd = 0.5f * (dets.d[2] + glm::sign(dets.d[2])*sqrt(dd));
			
			t1 = dd/dets.d[1];
			t2 = dets.d[3]/3.0f/dd;
		}

		glm::vec3	kk0(t1*t2,   t1*t1*t1,    t2*t2*t2),
					kk1(-t1-t2,  -3.0f*t1*t1, -3.0f*t2*t2),
					kk2(1.0f,    3.0f*t1,     3.0f*t2),
					kk3(0.0f,    -1.0f,       -1.0f);

		tc[0] = kk0;
		tc[1] = kk0+1.0f/3.0f*kk1;
		tc[2] = kk0+2.0f/3.0f*kk1+1.0f/3.0f*kk2;
		tc[3] = kk0+kk1+kk2+kk3;
	}

	// Loop case
	void calcLoopTC(Determinants& dets/*const float D, const float d[4]*/, glm::vec3 tc[4], float& t1, float& t2)
	{
		assert(dets.D<0 && dets.d[1]!=0);

		//float t1 = (d[2] + sqrt(-D))/d[1]/2.0f;
		//float t2 = 2.0f*d[1]/(d[2] - sqrt(-D));

		//glm::vec3	kk0(t1,       t1*t1,             t1),
		//			kk1(-t1*t2-1, -t1*t1*t2-2.0f*t1, -1-2*t1*t2),
		//			kk2(t2,       1+2.0f*t1*t2,      t1*t2*t2+2.0f*t2),
		//			kk3(0.0f,     -t2,               -t2*t2);

		//!!!!! TODO !!!!!
		//Calc roots in way similar to serpantine
		t1 = (dets.d[2] + sqrt(-dets.D))/dets.d[1]/2.0f;
		t2 = (dets.d[2] - sqrt(-dets.D))/dets.d[1]/2.0f;

		glm::vec3	kk0(t1*t2,      t1*t1*t2,             t2*t2*t1),
					kk1(-t1-t2,	    -t1*t1-2.0f*t1*t2,    -t2*t2-2.0f*t1*t2),
					kk2(1.0f,       t2+2.0f*t1,           t1+2.0f*t2),
					kk3(0.0f,		-1.0f,                -1.0f);

		tc[0] = kk0;
		tc[1] = kk0+1.0f/3.0f*kk1;
		tc[2] = kk0+2.0f/3.0f*kk1+1.0f/3.0f*kk2;
		tc[3] = kk0+kk1+kk2+kk3;
	}

	//Cusp at infinity
	void calcInfCuspTC(Determinants& dets/*const float D, const float d[4]*/, glm::vec3 tc[4], float& t)
	{
		assert(dets.d[1]==0 && dets.d[2]!=0);

		t = dets.d[3]/3.0f/dets.d[2];

		glm::vec3	kk0(t,     t*t*t,     1.0f),
					kk1(-1.0f, -3.0f*t*t, 0.0f),
					kk2(0.0f,  3.0f*t,    0.0f),
					kk3(0.0f,  -1,        0.0f);

		tc[0] = kk0;
		tc[1] = kk0+1.0f/3.0f*kk1;
		tc[2] = kk0+2.0f/3.0f*kk1+1.0f/3.0f*kk2;
		tc[3] = kk0+kk1+kk2+kk3;
	}

	// Quadratic curve
	void calcQuadraticTC(glm::vec3 tc[4])
	{
		tc[0] = glm::vec3(0.0f,      0.0f,      0.0f);
		tc[1] = glm::vec3(1.0f/3.0f, 0.0f,      1.0f/3.0f);
		tc[2] = glm::vec3(2.0f/3.0f, 1.0f/3.0f, 2.0f/3.0f);
		tc[3] = glm::vec3(1.0f,      1.0f,      1.0f);
	}

	void changeOrient(glm::vec3& tc)
	{
		tc.x = -tc.x;
		tc.y = -tc.y;
	}

	void calcTriVertices(glm::vec2 pts[4], int& count, glm::vec2 pos[10], glm::vec3 tc[10])
	{
		Determinants	det;
		float	roots[2];
		int		numRoots = 0;

		glm::vec2 cp1[4] = {pts[0], pts[1], pts[2], pts[3]};
		glm::vec2 cp2[4];
		glm::vec3 tc1[4], tc2[4];

		// Calc determinant
		calcDets(cp1, det);
		
		// First calculate t values at which we should subdivide our curve
		if (det.d[1] != 0)
		{
			float t1, t2;

			if (det.D>=0)
			{
				// Handle serpentine and cusp case
				calcSerpentineCuspTC(det, tc1, t1, t2);
			}
			else
			{
				// Handle loop case
				calcLoopTC(det, tc1, t1, t2);
			}
			
			if (t1>t2)
				std::swap(t1, t2);

			if (0<t1 && t1<1)
				roots[numRoots++] = t1;

			if (t1!=t2 && 0<t2 && t2<1)
				roots[numRoots++] = t2;
		}
		else if (det.d[2]!=0)
		{
			//Handle cusp at infinity case
			float t;

			calcInfCuspTC(det, tc1, t);
			roots[numRoots++] = t;
		}
		else if (det.d[3]!=0)
		{
			//Handle quadratic curve case
			calcQuadraticTC(tc1);
		}
		else
		{
			//This is degenarate cases
			pos[0] = cp1[0];
			pos[1] = cp1[3];

			tc[0] = glm::vec3(0);
			tc[1] = glm::vec3(0);

			count = 0;
			return;
		}

		for(int i=0; i<numRoots; ++i)
		{
			float t = i==0?roots[i]:(roots[i]-roots[i-1])/(1-roots[i-1]);

			cubic::subdivide(cp1, t, cp1, cp2);
			cubic::subdivide(tc1, t, tc1, tc2);

			pos[i*3+0] = cp1[0];
			pos[i*3+1] = cp1[1];
			pos[i*3+2] = cp1[2];

			tc[i*3+0] = tc1[0];
			tc[i*3+1] = tc1[1];
			tc[i*3+2] = tc1[2];
		}
		
		pos[i*3+0] = cp2[0];
		pos[i*3+1] = cp2[1];
		pos[i*3+2] = cp2[2];
		pos[i*3+3] = cp2[3];

		tc[i*3+0] = tc2[0];
		tc[i*3+1] = tc2[1];
		tc[i*3+2] = tc2[2];
		tc[i*3+3] = tc2[3];

		count = numRoots+1;
	}
}

namespace arc
{
	glm::vec2	connectingPoint(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& ox, const glm::vec2& oy)
	{
		glm::vec2 d = p1-p0;
		float px = glm::dot(d, ox);
		float py = glm::dot(d, oy);

		if (px*py<0)
			return p0 + ox*px;
		else //if (py>0)
			return p0 + oy*py;
	}

	void calcTriVertices(Type type, VGfloat rx, VGfloat ry, VGfloat angle,
						const glm::vec2& p0, const glm::vec2& p1,
						int& count, glm::vec2 pos[9], glm::vec3 tc[9])
	{
		//rotate by -angle
		glm::vec2 pp0 = glm::rotateGTX(p0, -angle);
		glm::vec2 pp1 = glm::rotateGTX(p1, -angle);

		//scale
		pp0 *= glm::vec2(1, rx/ry);
		pp1 *= glm::vec2(1, rx/ry);

		glm::vec2 delta = pp1-pp0;
		float r=rx, d=glm::length(delta);
		float a = d/2.0f;
		float b;

		//one or all radii is too near to zero
		//or end points too near
		if (!(rx==0 || ry==0 || d==0))
		{
			glm::mat2	invXform = glm::mat2(1, 0, 0, ry/rx)*glm::rotate2DGTX(glm::mat2(), angle);

			bool isSmallArc = IS_SMALL(type);

			glm::vec2 ox = glm::normalize(delta);
			glm::vec2 oy = rotate90CCW(ox);
			glm::vec2 midpt = glm::mix(pp0, pp1, 0.5f);
			
			if (a>rx)
				r = a;

			b = sqrt(r*r-a*a);
			
			//approximate with small arc
			if (b==0)
				isSmallArc = true;

			glm::vec2	center;

			if (type==SMALL_CCW || type==LARGE_CW)
				center = midpt + oy*b;
			else
				center = midpt - oy*b;

			ox *= r;
			oy *= r;

			if (IS_CCW(type))
				oy = -oy;

			glm::vec2 p[9]; 
			
			pp0 -=center;
			pp1 -=center;

			if (isSmallArc)
			{
				//glm::vec2 pt2 = center+r*oy;

				p[0] = pp0;
				p[1] = calcOffsetN(pp0, oy);//connectingPoint(pp0, pt2, ox, oy);
				p[2] = oy;//pt2;
				p[3] = calcOffsetN( oy, pp1);//connectingPoint(pt2, pp1, ox, oy);
				p[4] = pp1;

				count = 5;
			}
			else
			{
				//glm::vec2 pt2 = center-r*ox;
				//glm::vec2 pt3 = center+r*oy;
				//glm::vec2 pt4 = center+r*ox;



				p[0] = pp0;
				p[1] = calcOffsetN(pp0, -ox);//connectingPoint(pp0, pt2, ox, oy);
				p[2] = -ox;
				p[3] = calcOffsetN(-ox,  oy);//connectingPoint(pt2, pt3, ox, oy);
				p[4] = oy;
				p[5] = calcOffsetN( oy,  ox);//connectingPoint(pt3, pt4, ox, oy);
				p[6] = ox;
				p[7] = calcOffsetN( ox, pp1);//connectingPoint(pt4, pp1, ox, oy);
				p[8] = pp1;

				count = 9;
			}

			for (int i = 0; i<count; ++i)
			{
				pos[i] = glm::rotateGTX((center+p[i])*glm::vec2(1, ry/rx), angle);
				tc[i] = glm::vec3(p[i]/r, 0);
			}
		}
		else
		{
			count = 1;
			pos[count-1] = p1;
		}
	}
}
