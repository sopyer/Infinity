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

namespace cubic
{
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