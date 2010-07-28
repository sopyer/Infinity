#include <framework.h>

#include "VGExp.h"

//float w1 = -0.35f;
//float w2 = -0.35f;
//glm::vec3	controlPts[4] = {glm::vec3(0, 0, 1), glm::vec3(10*w1, 10*w1, w1), glm::vec3(0*w2, 10*w2, w2), glm::vec3(10, 0, 1)};
glm::vec2	controlPts2[4] = {glm::vec2(0, 0), glm::vec2(10.0f, 10.0f), glm::vec2(0, 10), glm::vec2(10, 0)};
//glm::vec2	controlPts2[4] = {glm::vec2(10, 0), glm::vec2(20.0f, 40.0f), glm::vec2(40, 40), glm::vec2(50, 0)};
//float w1 = /*1.0f/*/6.0f, w2 = 1.0f/*/3.0f*/;
//glm::vec3 controlPts[4] = {
//	glm::vec3(-10.0f,     0.0f,    1.0f),
//	glm::vec3(-10.0f*w1, 20.0f*w1,   w1),
//	glm::vec3( 10.0f*w2, 20.0f*w2,   w2),
//	glm::vec3( 10.0f,     0.0f,    1.0f),
//};

	//glm::vec3 controlPts[4] = {
	//	glm::vec3(-10.0f,     0.0f,    1.0f),
	//	glm::vec3( 0.0f*w1,  10.0f*w1,   w1),
	//	glm::vec3( 10.0f*w2,  0.0f*w2,   w2),
	//	glm::vec3( 30.0f,    20.0f,    1.0f),
	//};

	//float w1 = 1.0f/3.0f, w2 = 1.0f/3.0f;
	//glm::vec3 controlPts[4] = {
	//	glm::vec3(-10.0f,     0.0f,    1.0f),
	//	glm::vec3(-10.0f*w1, 20.0f*w1,   w1),
	//	glm::vec3( 10.0f*w2, 20.0f*w2,   w2),
	//	glm::vec3( 10.0f,     0.0f,    1.0f),
	//};

	glm::vec3 controlPts[4] = {
		glm::vec3(-20.0f,  0.0f, 1.0f),
		glm::vec3( 30.0f, 40.0f, 1.0f),
		glm::vec3(-30.0f, 40.0f, 1.0f),
		glm::vec3( 20.0f,  0.0f, 1.0f),
	};

Geometry<RCubicVertex>	rcubic;
Geometry<glm::vec2>		rtri;

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

void calcLinearFunctionals(float r0[2], float r1[2], float r2[2], float koef[4])
{
	//assert(false && "Debug me!!!!");
	//I have confused variables and they should be fixed
	float	t1=r0[0], s1 = r0[1],
			t2=r1[0], s2 = r1[1],
			t3=r2[0], s3 = r2[1];

	koef[0] =  t1*t2*t3;
	koef[1] = -t1*t2*s3-t1*s2*t3-s1*t2*t3;
	koef[2] =  s1*s2*t3+s1*t2*s3+t1*s2*s3;
	koef[3] = -s1*s2*s3;
}

void solveQuadratic(float k[3], int& count, float r[4])
{
	//assert(false && "Debug me!!!!");

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

float evalHomogeneousCubic(float d[4], float r[2])
{
	float rt = r[0], rs = r[1];

	float d1 = d[0]*rt*rt*rt;
	float d2 = d[1]*rt*rt*rs;
	float d3 = d[2]*rt*rs*rs;
	float d4 = d[3]*rs*rs*rs;

	float res = d1+d2+d3+d4;

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

enum
{
	RATIONAL_LOOP_CUBIC,
	RATIONAL_SERPENTINE_CUSP_CUBIC,
	INTEGRAL_LOOP_CUBIC,
	INTEGRAL_SERPENTINE_CUSP_CUBIC,
	CUSP_AT_INFINITY_CUBIC,
	RATIONAL_QUADRATIC_CUBIC,
	DEGENERATE_CUBIC
};

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

void addCubic(glm::vec3 cp[4], Geometry<RCubicVertex>& cubicGeom, Geometry<glm::vec2>& triGeom)
{
	glm::vec3	bezierBasisCP[4*4];
	glm::vec4	klmn[4*4];
	int			count;
	float		subdPt[3];

	memcpy(bezierBasisCP, cp, 4*sizeof(glm::vec3));
	implicitizeRationalBezierCubic(bezierBasisCP, klmn, count, subdPt);

	glm::vec2	tri[4] = {
		cp[0]._xy()/cp[0].z,
		cp[1]._xy()/cp[1].z,
		cp[2]._xy()/cp[2].z,
		cp[3]._xy()/cp[3].z,
	};

	glm::vec3 bary;
	glm::vec2 pt;

	float ttt[] = {0, 1, 0.5f, 0.75f, 0.25f, 0.333333f, 0.4f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f};
	for (size_t i=0; i<ARRAY_SIZE(ttt); ++i)
	{
		evalRationalBezier(ml::as<float>(cp),  ttt[i], pt);

		calcBarycentric(tri[0], tri[1], tri[3], pt, bary);
		assert(glm::equalEpsilonGTX(pt, bary.x*tri[0]+bary.y*tri[1]+bary.z*tri[3], ml::EPS7));

		glm::vec3 scale = bary/glm::vec3(cp[0].z, cp[1].z, cp[3].z);
		glm::vec4 klmnI = (scale.x*klmn[0] + scale.y*klmn[1] + scale.z*klmn[3])/(scale.x + scale.y + scale.z);

		float f = klmnI.x*klmnI.x*klmnI.x - klmnI.y*klmnI.z*klmnI.w;

		assert(ml::equalE(f, 0, 0.01f));
	}

	glm::vec2	intervals[4] = {glm::vec2(0, 1)};
	glm::vec2*	intervalLast = intervals+4;
	size_t		intervalCount = 1;

	for (int i=0; i<count; i++)
	{
		float	t = subdPt[i];
		int		left = 0;
		int		right = intervalCount++;

		while (left<right && !(ml::lessThenE(intervals[left].x, t) && ml::lessThenE(t, intervals[left].y)))
			++left;

		if (left==right) continue;

		glm::vec2 oldInterval = intervals[left];
		
		intervals[left]  = glm::vec2(oldInterval.x, t);
		intervals[right] = glm::vec2(t, oldInterval.y);

		left*=4; right*=4;

		float tt = (t-oldInterval.x)/(oldInterval.y-oldInterval.x);

		cubic::subdivide(bezierBasisCP+left, tt, bezierBasisCP+left, bezierBasisCP+right);
		cubic::subdivide(klmn+left, tt, klmn+left, klmn+right);
	}
	
	size_t cvertBase = cubicGeom.vertices.size();
	size_t tvertBase = triGeom.vertices.size();

	cubicGeom.vertices.resize(cvertBase+4*intervalCount);

	for (size_t base=0; base<4*intervalCount; base+=4, cvertBase+=4)
	{
		correctOrient(klmn+base);

		cubicGeom.vertices[cvertBase+0].pos = glm::vec4(bezierBasisCP[base+0].x, bezierBasisCP[base+0].y, 0, bezierBasisCP[base+0].z);
		cubicGeom.vertices[cvertBase+0].klmn = klmn[base+0];
		cubicGeom.vertices[cvertBase+1].pos = glm::vec4(bezierBasisCP[base+1].x, bezierBasisCP[base+1].y, 0, bezierBasisCP[base+1].z);
		cubicGeom.vertices[cvertBase+1].klmn = klmn[base+1];
		cubicGeom.vertices[cvertBase+2].pos = glm::vec4(bezierBasisCP[base+2].x, bezierBasisCP[base+2].y, 0, bezierBasisCP[base+2].z);
		cubicGeom.vertices[cvertBase+2].klmn = klmn[base+2];
		cubicGeom.vertices[cvertBase+3].pos = glm::vec4(bezierBasisCP[base+3].x, bezierBasisCP[base+3].y, 0, bezierBasisCP[base+3].z);
		cubicGeom.vertices[cvertBase+3].klmn = klmn[base+3];

		cubicGeom.indices.pushBack(cvertBase+0); cubicGeom.indices.pushBack(cvertBase+1); cubicGeom.indices.pushBack(cvertBase+2);
		cubicGeom.indices.pushBack(cvertBase+0); cubicGeom.indices.pushBack(cvertBase+2); cubicGeom.indices.pushBack(cvertBase+3);

		if (intervalCount==1) break;

		//we assume first iinterval is at the beginning of the curve
		if (base==0)
		{
			triGeom.vertices.pushBack(glm::vec2(bezierBasisCP[base+0].x, bezierBasisCP[base+0].y)/bezierBasisCP[base+0].z);
		}
		else
		{
			triGeom.vertices.pushBack(glm::vec2(bezierBasisCP[base+0].x, bezierBasisCP[base+0].y)/bezierBasisCP[base+0].z);
			triGeom.vertices.pushBack(glm::vec2(bezierBasisCP[base+3].x, bezierBasisCP[base+3].y)/bezierBasisCP[base+3].z);

			triGeom.indices.pushBack(tvertBase); triGeom.indices.pushBack(tvertBase+base/2-1); triGeom.indices.pushBack(tvertBase+base/2);
		}
	}
}

void addAATriangle(glm::vec3 cp[3], Geometry<RCubicVertex>& cubicGeom)
{
	size_t cvertBase = cubicGeom.vertices.size();

	cubicGeom.vertices.resize(cvertBase+3);

	float d = glm::dot(cp[1], glm::cross(cp[0], cp[2]));

	cubicGeom.vertices[cvertBase+0].pos = glm::vec4(cp[0].x, cp[0].y, 0, cp[0].z);
	cubicGeom.vertices[cvertBase+0].klmn = glm::vec4(0.5f, 0.5f, 0.5f, 1.1f);
	cubicGeom.vertices[cvertBase+1].pos = glm::vec4(cp[1].x, cp[1].y, 0, cp[1].z);
	cubicGeom.vertices[cvertBase+1].klmn = -glm::vec4(d, d, d, 1.0f);
	cubicGeom.vertices[cvertBase+2].pos = glm::vec4(cp[2].x, cp[2].y, 0, cp[2].z);
	cubicGeom.vertices[cvertBase+2].klmn = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

	cubicGeom.indices.pushBack(cvertBase+0); cubicGeom.indices.pushBack(cvertBase+1); cubicGeom.indices.pushBack(cvertBase+2);
}

GLuint createRenderbuffer(GLenum type, GLuint samples, GLsizei width, GLsizei height)
{
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, type, width, height);
	//glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return rbo;
}

//Use framebuffers just like layouts using shared components
GLuint createFramebuffer(GLuint colorRenderbuffer, GLuint depthRenderbuffer)
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	//glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		glDeleteFramebuffers(1, &fbo);
		fbo = 0;
	}
			
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return fbo;
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

			addCubic(controlPts, rcubic, rtri);
			//glm::vec3 cpt[3] = 
			//{
			//	glm::vec3(-20, 0, 1),
			//	glm::vec3(0, 20, 1),
			//	glm::vec3(20, 5, 1)
			//};

			//addAATriangle(cpt, rcubic);
			//testRCubic();
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
			colorRB = createRenderbuffer(GL_RGBA8, 8, 800, 600);
			depthRB = createRenderbuffer(GL_DEPTH24_STENCIL8, 8, 800, 600);
			fbo = createFramebuffer(colorRB, depthRB);
		}

		~VGTest()
		{
			glDeleteFramebuffers(1, &fbo);
			glDeleteRenderbuffers(1, &depthRB);
			glDeleteRenderbuffers(1, &colorRB);

			terminateVGExp();
			sui::destroyFont(mFont);
		}
		
	protected:
		GLuint colorRB, depthRB, fbo;
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
			int numPts = 1300;
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
			float vp[4];
			glGetFloatv(GL_VIEWPORT, vp);
			if (bool multisample = true)
			{
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				glEnable(GL_STENCIL_TEST);
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

				glMatrixMode(GL_MODELVIEW);
				glTranslatef(offsetX, offsetY, 0);
				glScalef(zoom, zoom, 1);

				glPushMatrix();
				glTranslatef(-40, -40, 0);
				glScalef(80, 80, 1);
				clearStencil();
				glPopMatrix();

				glPushMatrix();
					//glScalef(-1, 1, 1);
					drawRCubicA2C(rcubic);
					rasterizeEvenOdd(rtri);
				glPopMatrix();

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

				glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
				
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

				glBlitFramebuffer(
						0, 0, 800, 600,
						0, 0, 800, 600,
						//if enable depth and stencil resolve - label stops to render.
						GL_COLOR_BUFFER_BIT/*|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT*/,
						GL_NEAREST);

				glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			}
			else
			{
				glEnable(GL_STENCIL_TEST);

				glMatrixMode(GL_MODELVIEW);
				glTranslatef(offsetX, offsetY, 0);
				glScalef(zoom, zoom, 1);

				glPushMatrix();
				glTranslatef(-40, -40, 0);
				glScalef(80, 80, 1);
				clearStencil();
				glPopMatrix();
				//rasterizeEvenOdd(mRationalCubic);
				//rasterizeEvenOdd(mRasterCubic);
				//rasterizeEvenOdd(rcubic);
				//rasterizeEvenOdd(rtri);
				glPushMatrix();
				//glScalef(-1, 1, 1);
				drawRCubicAA(rcubic);
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
