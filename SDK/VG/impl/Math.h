#pragma once

#include <glm/glm.h>
#include <glm/glmext.h>
#include <vg/openvg.h>
#include <vector>
#include "DataTypes.h"

inline glm::vec2 perpendicularCW(const glm::vec2& vec)
{
	return glm::vec2(vec.y, -vec.x);
}

inline glm::vec2 perpendicularCCW(const glm::vec2& vec)
{
	return glm::vec2(-vec.y, vec.x);
}

inline glm::vec2 calcOffset(const glm::vec2& p0, const glm::vec2& p1)
{
	return glm::normalize(perpendicularCCW(p1-p0));
}

inline glm::vec2 calcOffsetN(const glm::vec2& n0, const glm::vec2& n1)
{
	float		det = (n0.y*n1.x-n0.x*n1.y)/glm::length(n0)/glm::length(n1);
	
	return (det!=0) ? perpendicularCCW(n1-n0)/det : 0.5f*(n0+n1);
}

inline glm::vec2 calcOffset(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
	glm::vec2	n0 = calcOffset(p0, p1),
				n1 = calcOffset(p1, p2);

	return calcOffsetN(n0, n1);
}

namespace ml
{
	const float EPS3 = 0.001f;
	const float EPS5 = 0.00001f;
	const float EPS7 = 0.0000001f;

	inline bool equalE(float a, float b, float tolerance = EPS7)
	{
		return abs(a-b)<tolerance;
	}

	inline bool lessThenE(float a, float b, float tolerance = EPS7)
	{
		return (b-a)>tolerance;
	}

	inline bool greaterThenE(float a, float b, float tolerance = EPS7)
	{
		return (a-b)>tolerance;
	}
}

namespace cubic
{
	float calcImplicit(const glm::vec3& tc);
	void changeOrient(glm::vec3& tc);
	void correctOrient(glm::vec3 tc[4]);

	template<typename T>
	void subdivide(const T cubic[4], float t, T subCubic1[4], T subCubic2[4])
	{
		T p0 = cubic[0];
		T p3 = cubic[3];

		T p01 = glm::mix(cubic[0], cubic[1], t);
		T p12 = glm::mix(cubic[1], cubic[2], t);
		T p23 = glm::mix(cubic[2], cubic[3], t);

		T p012 = glm::mix(p01, p12, t);
		T p123 = glm::mix(p12, p23, t);

		T p0123 = glm::mix(p012, p123, t);

		subCubic1[0] = p0;
		subCubic1[1] = p01;
		subCubic1[2] = p012;
		subCubic1[3] = p0123;

		subCubic2[0] = p0123;
		subCubic2[1] = p123;
		subCubic2[2] = p23;
		subCubic2[3] = p3;
	}
}

void cubicTriVertices(const Array<glm::vec2>& pts, Array<glm::vec2>& pos, Array<glm::vec3>& tc);

void arcTriVertices(VGPathSegment type, const glm::vec2& radius, VGfloat angle,
					const glm::vec2& p0, const glm::vec2& p1,
					Array<glm::vec2>& pos, Array<glm::vec3>& tc);

namespace ml
{
	template<typename T, typename U>
	T* as(U* value)
	{
		return reinterpret_cast<T*>(value);
	}
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

template<typename T>
T perspectiveInterpolate(const glm::vec3& bary, const glm::vec3& www,
						 const T& V0, const T& V1, const T& V2, float& invw)
{
	glm::vec3 scale = bary/www;
	invw = (scale.x + scale.y + scale.z);
	return (scale.x*V0 + scale.y*V1 + scale.z*V2)/invw;
}

void evalRationalBezier(float* cp, float t, float* pt);
bool checkUnitCircle(float* pt);
void calcLinearFunctionals(float r[6], float koef[4]);
void calcLinearFunctionals(float r0[2], float r1[2], float r2[2], float koef[4]);
void solveQuadratic(float k[3], int& count, float r[4]);
void solveCubic(float k[4], int& count, float r[6]);
float area(float* v0, float* v1, float* v2);
void calcBarycentric(/*vec2*/float* v0, /*vec2*/float* v1, /*vec2*/float* v2,/*vec2*/float* pt, /*vec3*/float* bary);
float evalHomogeneousCubic(float d[4], float r[2]);
float evalCubic(float d[4], float rt);
float evalHomogeneousQuadratic(float d[3], float r[2]);
float evalQuadratic(float d[3], float rt);
float calcImplicit(const glm::vec4& tc);
void changeOrient(glm::vec4& tc);
void correctOrient(glm::vec4 tc[4]);
void implicitizeRationalBezierCubic(glm::vec3 bezierBasisCP[4], glm::vec4 klmn[4], int& specialPointsCount, float specialPoints[3]);
void bezier3MakeImplicit(glm::vec2 pos[4], glm::vec3 klm[4], int& count, float* subdPts);

template< typename T >
void orderAscend(T& value1, T& value2)
{
	if (value1>value2) std::swap(value1, value2);
}