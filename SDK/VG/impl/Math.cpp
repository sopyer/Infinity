#include <cassert>
#include <algorithm>

#include "Math.h"

namespace cubic
{
	float calcImplicit(const glm::vec3& tc)
	{
		return tc.x*tc.x*tc.x - tc.y*tc.z;
	}

	void changeOrient(glm::vec3& tc)
	{
		tc.x = -tc.x;
		tc.y = -tc.y;
	}

	//Function only works on simple cubic subcurve - which does
	//not change curvature direction on oposite anywhere it is defined
	void correctOrient(glm::vec3 tc[4])
	{
		float valueCP1 = calcImplicit(tc[1]);
		float valueCP2 = calcImplicit(tc[2]);

		//TODO: Test following
		//float valueToCheck = abs(valueCP1)>abs(valueCP2)?valueCP1:valueCP2;
		////using exact comparison!!!! to avoid problems with values near zero of implicit function
		//if (valueToCheck<0)

		bool areCPOutside = ml::lessThenE(valueCP1, 0) || ml::lessThenE(valueCP2, 0);
		if (areCPOutside || (valueCP1<0)&&(valueCP2<0))
		//Test fails if two non-curve control points are outside but curve value is close to zero 
		//if (ml::lessThenE(valueCP1, 0) || ml::lessThenE(valueCP2, 0))
		//Test fails if one of the control points coincides with start or
		//end control point but has a different sign due to a rounding error
		//if (valueCP1<0 || valueCP2<0)
		{
			cubic::changeOrient(tc[0]);
			cubic::changeOrient(tc[1]);
			cubic::changeOrient(tc[2]);
			cubic::changeOrient(tc[3]);
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

	//Serpentine and cusp with inflection at infinity
	void calcSerpentineCuspTC(Determinants& dets, Array<glm::vec3>& tc, float& t1, float& t2)
	{
		assert(tc.size()==4);
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
	void calcLoopTC(Determinants& dets, Array<glm::vec3>& tc, float& t1, float& t2)
	{
		assert(tc.size()==4);
		assert(dets.D<0 && dets.d[1]!=0);

		/********************************************************/
		/*			Calculating koeficients in alternative way
		/* float t1 = (d[2] + sqrt(-D))/d[1]/2.0f;
		/* float t2 = 2.0f*d[1]/(d[2] - sqrt(-D));
		/*
		/* glm::vec3	kk0(t1,       t1*t1,             t1),
		/*				kk1(-t1*t2-1, -t1*t1*t2-2.0f*t1, -1-2*t1*t2),
		/*				kk2(t2,       1+2.0f*t1*t2,      t1*t2*t2+2.0f*t2),
		/*				kk3(0.0f,     -t2,               -t2*t2);
		/********************************************************/

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
	void calcInfCuspTC(Determinants& dets, Array<glm::vec3>& tc, float& t)
	{
		assert(tc.size()==4);
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
	void calcQuadraticTC(Array<glm::vec3>& tc)
	{
		assert(tc.size()==4);

		tc[0] = glm::vec3(0.0f,      0.0f,      0.0f);
		tc[1] = glm::vec3(1.0f/3.0f, 0.0f,      1.0f/3.0f);
		tc[2] = glm::vec3(2.0f/3.0f, 1.0f/3.0f, 2.0f/3.0f);
		tc[3] = glm::vec3(1.0f,      1.0f,      1.0f);
	}
}

void cubicTriVertices(const Array<glm::vec2>& pts, Array<glm::vec2>& pos, Array<glm::vec3>& tc)
{
	cubic::Determinants	det;
	Array<float>		subdPts;

	Array<glm::vec2> cp1, cp2;
	Array<glm::vec3> tc1, tc2;
	
	pos.assign(pts.begin(), pts.end());
	tc.resize(4);

	cubic::calcDets(pos, det);
	
	// Calculate t-values at which we should subdivide our curve
	// and values for explicit curve presentation at control point.
	// See Loop, Blinn. Resolution independent curve rendering using programmable hardware.
	if (det.d[1] != 0)
	{
		float t1, t2;

		if (det.D>=0)
		{
			// Handle serpentine and cusp case
			cubic::calcSerpentineCuspTC(det, tc, t1, t2);
		}
		else
		{
			// Handle loop case
			cubic::calcLoopTC(det, tc, t1, t2);
		}
		
		if (t1>t2)
			std::swap(t1, t2);

		if (0<t1 && t1<1)
			subdPts.pushBack(t1);

		if (t1!=t2 && 0<t2 && t2<1)
			subdPts.pushBack(t2);
	}
	else if (det.d[2]!=0)
	{
		//Handle cusp at infinity case
		float t;

		cubic::calcInfCuspTC(det, tc, t);
		subdPts.pushBack(t);
	}
	else if (det.d[3]!=0)
	{
		//Handle quadratic curve case
		cubic::calcQuadraticTC(tc);
	}
	else
	{
		//This is degenarate cases - line, point
		pos.clear();
		tc.clear();

		return;
	}
	
	pos.resize(subdPts.size()*4+4);
	tc.resize(subdPts.size()*4+4);

	glm::vec2* ptrPos = pos.begin();
	glm::vec3* ptrTC  = tc.begin();

	for(size_t i=0; i<subdPts.size(); ++i, ptrPos+=4, ptrTC+=4)
	{
		float t = i==0?subdPts[i]:(subdPts[i]-subdPts[i-1])/(1-subdPts[i-1]);

		cubic::subdivide(ptrPos, t, ptrPos, ptrPos+4);
		cubic::subdivide(ptrTC, t, ptrTC, ptrTC+4);

		cubic::correctOrient(ptrTC);
	}
	
	cubic::correctOrient(ptrTC);
}

void arcTriVertices(VGPathSegment type, const glm::vec2& radius, VGfloat angle,
					const glm::vec2& p0, const glm::vec2& p1,
					Array<glm::vec2>& pos, Array<glm::vec3>& tc)
{
	assert(type==VG_SCCWARC_TO || type==VG_SCWARC_TO ||
		   type==VG_LCCWARC_TO || type==VG_LCWARC_TO);

	VGfloat rx = radius.x;
	VGfloat ry = radius.y;

	//rotate by -angle
	glm::vec2 pp0 = glm::rotateGTX(p0, -angle);
	glm::vec2 pp1 = glm::rotateGTX(p1, -angle);

	//scale
	pp0 *= glm::vec2(1, rx/ry);
	pp1 *= glm::vec2(1, rx/ry);

	glm::vec2 delta = pp1-pp0;
	float d=glm::length(delta);

	//one or all radii is too near to zero
	//or end points too near
	if (!(rx==0 || ry==0 || d==0))
	{
		bool isSmallArc = type==VG_SCCWARC_TO || type==VG_SCWARC_TO;

		float r = rx;
		float a = d/2.0f;
		float b;

		glm::vec2 ox = glm::normalize(delta);
		glm::vec2 oy = perpendicularCCW(ox);
		glm::vec2 midpt = glm::mix(pp0, pp1, 0.5f);
		
		if (a>rx)
			r = a;

		b = sqrt(r*r-a*a);
		
		//approximate with small arc
		if (b==0)
			isSmallArc = true;

		glm::vec2	center;

		if (type==VG_SCCWARC_TO || type==VG_LCWARC_TO)
			center = midpt + oy*b;
		else
			center = midpt - oy*b;

		ox *= r;
		oy *= r;

		if (type==VG_SCCWARC_TO || type==VG_LCCWARC_TO)
			oy = -oy;

		pp0 -=center;
		pp1 -=center;

		pos.resize(isSmallArc?5:9);
		tc.resize (isSmallArc?5:9);

		if (isSmallArc)
		{
			pos[0] = pp0;
			pos[1] = calcOffsetN(pp0, oy);
			pos[2] = oy;
			pos[3] = calcOffsetN( oy, pp1);
			pos[4] = pp1;
		}
		else
		{
			pos[0] = pp0;
			pos[1] = calcOffsetN(pp0, -ox);
			pos[2] = -ox;
			pos[3] = calcOffsetN(-ox,  oy);
			pos[4] = oy;
			pos[5] = calcOffsetN( oy,  ox);
			pos[6] = ox;
			pos[7] = calcOffsetN( ox, pp1);
			pos[8] = pp1;
		}

		for (size_t i = 0; i<pos.size(); ++i)
		{
			//order is important;)
			tc[i] = glm::vec3(pos[i]/r, 0);
			pos[i] = glm::rotateGTX((center+pos[i])*glm::vec2(1, ry/rx), angle);
		}
	}
	else
	{
		pos.clear();
		tc.clear();
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

void calcLinearFunctionals(float r0[2], float r1[2], float r2[2], float koef[4])
{
	//assert(false && "Debug me!!!!");
	//I have confused variables and they should be fixed
	float	t1=r0[0], s1 = r0[1],
			t2=r1[0], s2 = r1[1],
			t3=r2[0], s3 = r2[1];

	koef[0] =  t1*t2*t3;
	koef[1] = -t1*t2*s3-t1*s2*t3-s1*t2*t3;	//possible loss of precision!!!!
	koef[2] =  s1*s2*t3+s1*t2*s3+t1*s2*s3;	//possible loss of precision!!!!
	koef[3] = -s1*s2*s3;
}

void solveQuadratic(float k[3], int& count, float r[4])
{
	float a = k[0], b = k[1], c = k[2];
	float D = b*b-4*a*c;

	if (D<0)
	{
		count = 0;
	}
	else
	{
		count = 2;

		if (D==0)
		{
			r[0] = r[2] = -b;
			r[1] = r[3] = 2*a;
		}
		else
		{
			float rr = -b + (b<0?1:-1)*sqrt(D);
		
			r[0] = rr;
			r[1] = 2*a;
			float t = rr/2/a;
			float det = a*t*t+b*t+c;
			r[2] = 2*c;
			r[3] = rr;
			t = 2*c/rr;
			det = a*t*t+b*t+c;
			t++;
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

float area(float* v0, float* v1, float* v2)
{
	return 0.5f*((v1[0]-v0[0])*(v1[1]+v0[1]) + 
				 (v2[0]-v1[0])*(v2[1]+v1[1]) + 
				 (v0[0]-v2[0])*(v0[1]+v2[1]));
}

void calcBarycentric(/*vec2*/float* v0, /*vec2*/float* v1, /*vec2*/float* v2,/*vec2*/float* pt, /*vec3*/float* bary)
{
	float triArea = area(v0, v1, v2);
	if (ml::equalE(triArea, 0))
	{
		assert(!"Degenerate triangle!!!");
	}
	else
	{
		bary[0] = area(v1, v2, pt)/triArea;
		bary[1] = area(v2, v0, pt)/triArea;
		bary[2] = area(v0, v1, pt)/triArea;
	}
}

float evalHomogeneousCubic(float d[4], float r[2])
{
	float rt = r[0], rs = r[1];

	return d[0]*rt*rt*rt + d[1]*rt*rt*rs + d[2]*rt*rs*rs + d[3]*rs*rs*rs;
}

float evalCubic(float d[4], float rt)
{
	return d[0]*rt*rt*rt + d[1]*rt*rt + d[2]*rt + d[3];
}

float evalHomogeneousQuadratic(float d[3], float r[2])
{
	float t = r[0], s = r[1];
	return d[0]*t*t+d[1]*t*s+d[2]*s*s;
}

float evalQuadratic(float d[3], float rt)
{
	return d[0]*rt*rt + d[1]*rt + d[2];
}

float calcImplicit(const glm::vec4& tc)
{
	return tc.x*tc.x*tc.x - tc.y*tc.z*tc.w;
}

void changeOrient(glm::vec4& tc)
{
	tc.x = -tc.x;
	tc.y = -tc.y;
}

void correctOrient(glm::vec4 tc[4])
{
	if (ml::lessThenE(calcImplicit(tc[1]), 0) || ml::lessThenE(calcImplicit(tc[2]),0))
	//if (cubic::calcImplicit(tc[1])<0 || cubic::calcImplicit(tc[2])<0)
	{
		changeOrient(tc[0]);
		changeOrient(tc[1]);
		changeOrient(tc[2]);
		changeOrient(tc[3]);
	}
}

void implicitizeRationalBezierCubic(glm::vec3 bezierBasisCP[4], glm::vec4 klmn[4], int& specialPointsCount, float specialPoints[3])
{
	//Transform from Bezier to power basis
	glm::vec3 powerBasisCP[4] = {
		      bezierBasisCP[0],
		-3.0f*bezierBasisCP[0] +  3.0f*bezierBasisCP[1],
		 3.0f*bezierBasisCP[0] + -6.0f*bezierBasisCP[1] +  3.0f*bezierBasisCP[2],
		     -bezierBasisCP[0] +  3.0f*bezierBasisCP[1] + -3.0f*bezierBasisCP[2] + bezierBasisCP[3],
	};

	glm::vec4	d = glm::vec4(
		 glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[2], powerBasisCP[1])),
		-glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[2], powerBasisCP[0])),
		 glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[1], powerBasisCP[0])),
		-glm::dot(powerBasisCP[2], glm::cross(powerBasisCP[1], powerBasisCP[0]))
	);

	//Mitigates precision issues
	d = glm::normalize(d);

	//Hessian coefficients
	glm::vec3	dt = glm::vec3(
		d[0]*d[2]-d[1]*d[1],
		d[1]*d[2]-d[0]*d[3],
		d[1]*d[3]-d[2]*d[2]
	);

	//Evaluate cubic determinant
	float det = 4*dt[0]*dt[2]-dt[1]*dt[1];

	//Final adjustments to make inflection point polynomial
	d[1] *= -3.0f;
	d[2] *=  3.0f;
	d[3]  = -d[3];
	
	glm::vec2	r[3] = 
	{
		glm::vec2(1, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 0)
	};

	glm::mat4	k;
	int			count;

	//det>0 - sepentine, det==0 - cusp, det<0 - loop
	int subCubicType = !!(det>=0);
	//Determine cubic type
	int cubicType = d[0]!=0?subCubicType:
					d[1]!=0?INTEGRAL_LOOP_CUBIC+subCubicType:
					d[2]!=0?CUSP_AT_INFINITY_CUBIC:
					d[3]!=0?RATIONAL_QUADRATIC_CUBIC:DEGENERATE_CUBIC;

	switch (cubicType)
	{
		case RATIONAL_LOOP_CUBIC:
			solveCubic(d, count, ml::as<float>(r));
			assert(count==1);
		
			solveQuadratic(dt, count, ml::as<float>(r+1));
			assert(count==2);
			
			//Mitigate precision issues
			r[0] = glm::normalize(r[0]);
			r[1] = glm::normalize(r[1]);
			r[2] = glm::normalize(r[2]);

			calcLinearFunctionals(r[0], r[1], r[2], k[0]);
			calcLinearFunctionals(r[0], r[0], r[0], k[1]);
			calcLinearFunctionals(r[1], r[1], r[2], k[2]);
			calcLinearFunctionals(r[1], r[2], r[2], k[3]);
		
			break;

		case RATIONAL_SERPENTINE_CUSP_CUBIC:
			solveCubic(d, count, ml::as<float>(r));
 			assert(count==3);

			//Mitigate precision issues
			r[0] = glm::normalize(r[0]);
			r[1] = glm::normalize(r[1]);
			r[2] = glm::normalize(r[2]);

			calcLinearFunctionals(r[0], r[1], r[2], k[0]);
			calcLinearFunctionals(r[0], r[0], r[0], k[1]);
			calcLinearFunctionals(r[1], r[1], r[1], k[2]);
			calcLinearFunctionals(r[2], r[2], r[2], k[3]);
		
			break;

		//TODO : merge with rational case
		case INTEGRAL_LOOP_CUBIC:
			solveQuadratic(dt, count, ml::as<float>(r+1));
			assert(count==2);
			//r[0] = glm::vec2(1, 0);
			
			//Mitigate precision issues
			r[1] = glm::normalize(r[1]);
			r[2] = glm::normalize(r[2]);

			calcLinearFunctionals(r[0], r[1], r[2], k[0]);
			calcLinearFunctionals(r[0], r[0], r[0], k[1]);
			calcLinearFunctionals(r[1], r[1], r[2], k[2]);
			calcLinearFunctionals(r[1], r[2], r[2], k[3]);
		
			break;

		//TODO : merge with rational case
		case INTEGRAL_SERPENTINE_CUSP_CUBIC:
			solveQuadratic((float*)d+1, count, ml::as<float>(r));
			assert(count==2);
			//r[2] = glm::vec2(1, 0);

			//Mitigate precision issues
			r[0] = glm::normalize(r[0]);
			r[1] = glm::normalize(r[1]);

			calcLinearFunctionals(r[0], r[1], r[2], k[0]);
			calcLinearFunctionals(r[0], r[0], r[0], k[1]);
			calcLinearFunctionals(r[1], r[1], r[1], k[2]);
			calcLinearFunctionals(r[2], r[2], r[2], k[3]);

			break;

		//TODO : merge with rational case
		case CUSP_AT_INFINITY_CUBIC:
			r[0] = glm::vec2(-d[3], d[2]);
			//r[1] = glm::vec2(1, 0);
			//r[2] = glm::vec2(1, 0);

			r[0] = glm::normalize(r[0]);

			calcLinearFunctionals(r[0], r[1], r[2], k[0]);
			calcLinearFunctionals(r[0], r[0], r[0], k[1]);
			calcLinearFunctionals(r[1], r[1], r[1], k[2]);
			calcLinearFunctionals(r[2], r[2], r[2], k[3]);

			break;

		case RATIONAL_QUADRATIC_CUBIC:
			k[0] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			k[1] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
			k[2] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			k[3] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			break;

		case DEGENERATE_CUBIC:
			//TODO: implement proper handling of this case

		default:
			assert(false && "Algorithm failed to determine correct cubic curve");
	}

	k = glm::transpose(k);

	klmn[0] = k[0];
	klmn[1] = k[0] + 1.0f/3.0f*k[1];
	klmn[2] = k[0] + 2.0f/3.0f*k[1] + 1.0f/3.0f*k[2];
	klmn[3] = k[0] + k[1] + k[2] + k[3];

	specialPointsCount = 0;
	for (int i=0; i<3; i++)
	{
		r[i] = (r[i].x<0)?-r[i]:r[i];

		if (!(0<r[i].x && r[i].x<r[i].y))
			continue;
		
		specialPoints[specialPointsCount++] = r[i].x/r[i].y;
	}
}

void bezier3Classify(glm::vec2 pos[4], glm::vec4& d, glm::vec3& hessian, float& det, int& cubicType)
{
	//TODO: to many fp safeguards???? Requires cleanup?
	//TODO: what about rational curves???

	//Some black magic to alleviate precision loss
	glm::vec2 mn = pos[0];
	glm::vec2 mx = pos[0];

	mn = glm::min(mn, pos[1]);
	mn = glm::min(mn, pos[2]);
	mn = glm::min(mn, pos[3]);

	mx = glm::max(mx, pos[1]);
	mx = glm::max(mx, pos[2]);
	mx = glm::max(mx, pos[3]);

	glm::vec2 dim = mx-mn;

	float scale = 1.0;//std::max(dim.x, dim.y); // pointless???

	glm::vec2 midPt = (pos[0]+pos[1]+pos[2]+pos[3])*0.25f;

	//It's real black magic!!!!
	glm::vec2 bezierBasisCP[4] = {
		glm::vec2(pos[0]-midPt)*scale,
		glm::vec2(pos[1]-midPt)*scale,
		glm::vec2(pos[2]-midPt)*scale,
		glm::vec2(pos[3]-midPt)*scale,
	};

	//Transform from Bezier to power basis
	//We can improve precision using advanced sumation algorithm
	//and improve speed reusing results from previous power basis
	//control point
	glm::vec2 powerBasisCP[4] = {
			  bezierBasisCP[0],
		-3.0f*bezierBasisCP[0] +  3.0f*bezierBasisCP[1],
		 3.0f*bezierBasisCP[0] + -6.0f*bezierBasisCP[1] +  3.0f*bezierBasisCP[2],						//possible loss of precision!!!!
			 -bezierBasisCP[0] +  3.0f*bezierBasisCP[1] + -3.0f*bezierBasisCP[2] + bezierBasisCP[3],	//possible loss of precision!!!!
	};

	/*glm::vec3*/float cross0 =  0.0f;//glm::cross(powerBasisCP[2], powerBasisCP[1]);
	/*glm::vec3*/float cross1 =  powerBasisCP[2].x*powerBasisCP[3].y - powerBasisCP[2].y*powerBasisCP[3].x;//glm::cross(powerBasisCP[2], powerBasisCP[3]);
	/*glm::vec3*/float cross2 =  powerBasisCP[3].x*powerBasisCP[1].y - powerBasisCP[3].y*powerBasisCP[1].x;//glm::cross(powerBasisCP[3], powerBasisCP[1]);
	/*glm::vec3*/float cross3 =  powerBasisCP[1].x*powerBasisCP[2].y - powerBasisCP[1].y*powerBasisCP[2].x;//glm::cross(powerBasisCP[1], powerBasisCP[2]);

	d = glm::vec4(
		 cross0,//glm::dot(powerBasisCP[3], cross0), //==0!!!!!
		 cross1,//glm::dot(powerBasisCP[0], cross1),
		 cross2,//glm::dot(powerBasisCP[0], cross2),
		 cross3 //glm::dot(powerBasisCP[0], cross3)
	);

	//Mitigates precision issues
	d = glm::normalize(d);

	//Hessian coefficients
	hessian = glm::vec3(
		-d[1]*d[1],
		 d[1]*d[2],
		 d[1]*d[3]-d[2]*d[2]
	);

	//Evaluate cubic determinant
	det = 4*hessian[0]*hessian[2]-hessian[1]*hessian[1];

	//det>0 - sepentine, det==0 - cusp, det<0 - loop
	int subCubicType = !!(det>=0);
	cubicType = d[1]!=0?INTEGRAL_LOOP_CUBIC+subCubicType:
				d[2]!=0?CUSP_AT_INFINITY_CUBIC:
				d[3]!=0?RATIONAL_QUADRATIC_CUBIC:DEGENERATE_CUBIC;
}

void bezier3MakeImplicit(glm::vec2 pos[4], glm::vec3 klm[4], int& subdPtCount, float* subdPts)
{
	//TODO: to many fp safeguards???? Requires cleanup?
	//TODO: what about rational curves???

	//Some black magic to alleviate precision loss
	glm::vec2 midPt = (pos[0]+pos[1]+pos[2]+pos[3])*0.25f;

	glm::vec3 bezierBasisCP[4] = {
		glm::vec3(pos[0]-midPt, 1.0f),
		glm::vec3(pos[1]-midPt, 1.0f),
		glm::vec3(pos[2]-midPt, 1.0f),
		glm::vec3(pos[3]-midPt, 1.0f),
	};

	//Transform from Bezier to power basis
	glm::vec3 powerBasisCP[4] = {
			  bezierBasisCP[0],
		-3.0f*bezierBasisCP[0] +  3.0f*bezierBasisCP[1],
		 3.0f*bezierBasisCP[0] + -6.0f*bezierBasisCP[1] +  3.0f*bezierBasisCP[2],
			 -bezierBasisCP[0] +  3.0f*bezierBasisCP[1] + -3.0f*bezierBasisCP[2] + bezierBasisCP[3],
	};

	glm::vec4	d2 = glm::vec4(
		 glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[2], powerBasisCP[1])),
		-glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[2], powerBasisCP[0])),
		 glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[1], powerBasisCP[0])),
		-glm::dot(powerBasisCP[2], glm::cross(powerBasisCP[1], powerBasisCP[0]))
	);

	glm::vec4	d;
	glm::vec3	dt;
	float		det;
	int			cubicType;

	bezier3Classify(pos, d, dt, det, cubicType);

	//Mitigates precision issues
	d2 = glm::normalize(d2);

	assert(ml::equalE(d[0], 0.0f));
	assert(ml::equalE(d[0], d2[0]));
	assert(ml::equalE(d[1], d2[1]));
	assert(ml::equalE(d[2], d2[2]));
	assert(ml::equalE(d[3], d2[3]));

	//Hessian coefficients
	glm::vec3 dt2 = glm::vec3(
		d[0]*d[2]-d[1]*d[1],
		d[1]*d[2]-d[0]*d[3],
		d[1]*d[3]-d[2]*d[2]
	);

	assert(ml::equalE(dt[0], dt2[0]));
	assert(ml::equalE(dt[1], dt2[1]));
	assert(ml::equalE(dt[2], dt2[2]));

	//Evaluate cubic determinant
	float det2 = 4*dt[0]*dt[2]-dt[1]*dt[1];

	assert(ml::equalE(det, det2));

	//Final adjustments to make inflection point polynomial
	d[1] *= -3.0f;
	d[2] *=  3.0f;
	d[3]  = -d[3];
	
	glm::vec2	r[2] = 
	{
		glm::vec2(1, 0),
		glm::vec2(1, 0),
	};

	glm::mat4	k;
	int			count;

	//det>0 - sepentine, det==0 - cusp, det<0 - loop
	int subCubicType = !!(det>=0);
	int cubicType2 = d[0]!=0?subCubicType:
					d[1]!=0?INTEGRAL_LOOP_CUBIC+subCubicType:
					d[2]!=0?CUSP_AT_INFINITY_CUBIC:
					d[3]!=0?RATIONAL_QUADRATIC_CUBIC:DEGENERATE_CUBIC;

	assert(cubicType==cubicType2);

	assert(cubicType!=RATIONAL_LOOP_CUBIC && cubicType!=RATIONAL_SERPENTINE_CUSP_CUBIC);
	switch (cubicType)
	{
		case INTEGRAL_LOOP_CUBIC:
			solveQuadratic(dt, count, ml::as<float>(r));
			assert(count==2);
		
			//Mitigate precision issues
			r[0] = glm::normalize(r[0]);
			r[1] = glm::normalize(r[1]);

			calcLinearFunctionals(r[0], r[1], glm::vec2(1.0f, 0.0f), k[0]);
			calcLinearFunctionals(r[0], r[0], r[1], k[1]);
			calcLinearFunctionals(r[0], r[1], r[1], k[2]);
			k[3] = glm::vec4(1.0f);
	
			break;

		case INTEGRAL_SERPENTINE_CUSP_CUBIC:
			solveQuadratic((float*)d+1, count, ml::as<float>(r));
			assert(count==2);

			//Mitigate precision issues
			r[0] = glm::normalize(r[0]);
			r[1] = glm::normalize(r[1]);

			calcLinearFunctionals(r[0], r[1], glm::vec2(1.0f, 0.0f), k[0]);
			calcLinearFunctionals(r[0], r[0], r[0], k[1]);
			calcLinearFunctionals(r[1], r[1], r[1], k[2]);
			k[3] = glm::vec4(1.0f);

			break;

		case CUSP_AT_INFINITY_CUBIC:
			r[0] = glm::vec2(-d[3], d[2]);
			count = 1;

			r[0] = glm::normalize(r[0]);

			calcLinearFunctionals(r[0], glm::vec2(1.0f), glm::vec2(1.0f), k[0]);
			calcLinearFunctionals(r[0], r[0], r[0], k[1]);
			k[2] = glm::vec4(1.0f);
			k[3] = glm::vec4(1.0f);

			break;

		case RATIONAL_QUADRATIC_CUBIC:
			count = 0;

			k[0] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			k[1] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
			k[2] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			k[3] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			break;

		case DEGENERATE_CUBIC:
		default:
			assert(false && "Algorithm failed to determine correct cubic curve");
	}

	k = glm::transpose(k);

	klm[0] = glm::vec3(k[0]);
	klm[1] = glm::vec3(k[0] + 1.0f/3.0f*k[1]);
	klm[2] = glm::vec3(k[0] + 2.0f/3.0f*k[1] + 1.0f/3.0f*k[2]);
	klm[3] = glm::vec3(k[0] + k[1] + k[2] + k[3]);

	subdPtCount = 0;
	for (int i=0; i<2; i++)
	{
		r[i] = (r[i].x<0)?-r[i]:r[i];

		if (!(0<r[i].x && r[i].x<r[i].y))
			continue;
	
		subdPts[subdPtCount++] = r[i].x/r[i].y;
	}
}

void bezier3SpecialPts(glm::vec2 pos[4], int& specialPtCount, float* specialPts)
{
	//TODO: to many fp safeguards???? Requires cleanup?
	//TODO: what about rational curves???

	//Some black magic to alleviate precision loss
	glm::vec2 midPt = (pos[0]+pos[1]+pos[2]+pos[3])*0.25f;

	glm::vec3 bezierBasisCP[4] = {
		glm::vec3(pos[0]-midPt, 1.0f),
		glm::vec3(pos[1]-midPt, 1.0f),
		glm::vec3(pos[2]-midPt, 1.0f),
		glm::vec3(pos[3]-midPt, 1.0f),
	};

	//Transform from Bezier to power basis
	glm::vec3 powerBasisCP[4] = {
			  bezierBasisCP[0],
		-3.0f*bezierBasisCP[0] +  3.0f*bezierBasisCP[1],
		 3.0f*bezierBasisCP[0] + -6.0f*bezierBasisCP[1] +  3.0f*bezierBasisCP[2],
			 -bezierBasisCP[0] +  3.0f*bezierBasisCP[1] + -3.0f*bezierBasisCP[2] + bezierBasisCP[3],
	};

	glm::vec4	d2 = glm::vec4(
		 glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[2], powerBasisCP[1])),
		-glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[2], powerBasisCP[0])),
		 glm::dot(powerBasisCP[3], glm::cross(powerBasisCP[1], powerBasisCP[0])),
		-glm::dot(powerBasisCP[2], glm::cross(powerBasisCP[1], powerBasisCP[0]))
	);

	glm::vec4	d;
	glm::vec3	dt;
	float		det;
	int			cubicType;

	bezier3Classify(pos, d, dt, det, cubicType);

	//Mitigates precision issues
	d2 = glm::normalize(d2);

	assert(ml::equalE(d[0], 0.0f));
	assert(ml::equalE(d[0], d2[0]));
	assert(ml::equalE(d[1], d2[1]));
	assert(ml::equalE(d[2], d2[2]));
	assert(ml::equalE(d[3], d2[3]));

	//Hessian coefficients
	glm::vec3	dt2 = glm::vec3(
		d[0]*d[2]-d[1]*d[1],
		d[1]*d[2]-d[0]*d[3],
		d[1]*d[3]-d[2]*d[2]
	);

	assert(ml::equalE(dt[0], dt2[0]));
	assert(ml::equalE(dt[1], dt2[1]));
	assert(ml::equalE(dt[2], dt2[2]));

	//Evaluate cubic determinant
	float det2 = 4*dt[0]*dt[2]-dt[1]*dt[1];

	assert(ml::equalE(det, det2));

	//Final adjustments to make inflection point polynomial
	d[1] *= -3.0f;
	d[2] *=  3.0f;
	d[3]  = -d[3];
	
	glm::vec2	r[2];/* = 
	{
		glm::vec2(1, 0),
		glm::vec2(1, 0),
	};*/

	int			count;

	//det>0 - sepentine, det==0 - cusp, det<0 - loop
	int subCubicType = !!(det>=0);
	int cubicType2 = d[0]!=0?subCubicType:
					d[1]!=0?INTEGRAL_LOOP_CUBIC+subCubicType:
					d[2]!=0?CUSP_AT_INFINITY_CUBIC:
					d[3]!=0?RATIONAL_QUADRATIC_CUBIC:DEGENERATE_CUBIC;

	assert(cubicType==cubicType2);

	assert(cubicType!=RATIONAL_LOOP_CUBIC && cubicType!=RATIONAL_SERPENTINE_CUSP_CUBIC);
	switch (cubicType)
	{
		case INTEGRAL_LOOP_CUBIC:
			solveQuadratic(dt, count, ml::as<float>(r));
			assert(count==2);
		
			//Mitigate precision issues
			//r[0] = glm::normalize(r[0]);
			//r[1] = glm::normalize(r[1]);

			break;

		case INTEGRAL_SERPENTINE_CUSP_CUBIC:
			solveQuadratic((float*)d+1, count, ml::as<float>(r));
			assert(count==2);

			//Mitigate precision issues
			//r[0] = glm::normalize(r[0]);
			//r[1] = glm::normalize(r[1]);

			break;

		case CUSP_AT_INFINITY_CUBIC:
			r[0] = glm::vec2(-d[3], d[2]);
			count = 1;

			//r[0] = glm::normalize(r[0]);

			break;

		case RATIONAL_QUADRATIC_CUBIC:
			count = 0;

		case DEGENERATE_CUBIC:
		default:
			assert(false && "Algorithm failed to determine correct cubic curve");
	}

	specialPtCount = 0;
	for (int i=0; i<count; i++)
	{
		r[i] = (r[i].x<0)?-r[i]:r[i];

		if (!(0<r[i].x && r[i].x<r[i].y))
			continue;
	
		specialPts[specialPtCount++] = r[i].x/r[i].y;
	}
}
