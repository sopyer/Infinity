#pragma once

#include <glm/glm.h>

namespace cubic
{
	float calcImplicit(const glm::vec3& tc);

	void calcSerpentineCuspTC(const float D, const float d[4], glm::vec3 tc[4]);
	void calcLoopTC(const float D, const float d[4], glm::vec3 tc[4], float& t1, float& t2);
	void calcInfCuspTC(const float D, const float d[4], glm::vec3 tc[4]);
	void calcQuadraticTC(glm::vec3 tc[4]);

	void calcDets(const glm::vec2 cp[4], float d[4], float& D);

	void triangulate(const float a[4], size_t& numTri, size_t idxTri[][3]);

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