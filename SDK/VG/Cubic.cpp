#include "Cubic.h"
#include <cassert>

namespace cubic
{
	float calcImplicit(const glm::vec3& tc)
	{
		return tc.x*tc.x*tc.x - tc.y*tc.z;
	}

	void calcDets(const glm::vec2 p[4], float d[4], float& D)
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

		d[0] = 3*(a[3]+a[2]+a[1]+a[0]);
		d[1] = 3*(3*a[3]+2*a[2]+a[1]);
		d[2] = 3*(3*a[3]+a[2]);
		d[3] = 3*(3*a[3]);

		//following expression has the same sign as d1*d1*(3*d2*d2 - 4*d1*d3)
		//and later it is reused as discriminant for quadratic equation
		D = 3.0f*d[2]*d[2] - 4.0f*d[1]*d[3];
	}

	//Serpentine and cusp with inflection at infinity
	void calcSerpentineCuspTC(const float D, const float d[4], glm::vec3 tc[4], float& t1, float& t2)
	{
		assert(D>=0 && d[1]!=0);

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

		float dd = D/3.0f;

		if (d[2] == 0)
		{
			t1 = -sqrt(dd)/2.0f/d[1];
			t2 = sqrt(dd)/2.0f/d[1];
		}
		else
		{
			dd = 0.5f * (d[2] + glm::sign(d[2])*sqrt(dd));
			
			t1 = dd/d[1];
			t2 = d[3]/3.0f/dd;
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
	void calcLoopTC(const float D, const float d[4], glm::vec3 tc[4], float& t1, float& t2)
	{
		assert(D<0 && d[1]!=0);

		//float t1 = (d[2] + sqrt(-D))/d[1]/2.0f;
		//float t2 = 2.0f*d[1]/(d[2] - sqrt(-D));

		//glm::vec3	kk0(t1,       t1*t1,             t1),
		//			kk1(-t1*t2-1, -t1*t1*t2-2.0f*t1, -1-2*t1*t2),
		//			kk2(t2,       1+2.0f*t1*t2,      t1*t2*t2+2.0f*t2),
		//			kk3(0.0f,     -t2,               -t2*t2);

		//!!!!! TODO !!!!!
		//Calc roots in way similar to serpantine
		t1 = (d[2] + sqrt(-D))/d[1]/2.0f;
		t2 = (d[2] - sqrt(-D))/d[1]/2.0f;

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
	void calcInfCuspTC(const float D, const float d[4], glm::vec3 tc[4], float& t)
	{
		assert(d[1]==0 && d[2]!=0);

		t = d[3]/3.0f/d[2];

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
}