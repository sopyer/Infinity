#pragma once

#include <glm/glm.h>

namespace cubic
{
	float calcImplicit(const glm::vec3& tc);

	void calcSerpentineCuspTC(const float D, const float d[4], glm::vec3 tc[4]);
	void calcLoopTC(const float D, const float d[4], glm::vec3 tc[4], float& t1, float& t2);
	void calcInfCuspTC(const float D, const float d[4], glm::vec3 tc[4]);
	void calcQuadraticTC(glm::vec3 tc[4]);

	void calcDets(const glm::vec3 cp[4], float a[4], float d[4], float& D);

	void triangulate(const float a[4], size_t& numTri, size_t idxTri[][3]);

	void subdivide(const glm::vec3 cubic[4], float t, glm::vec3 subCubic1[4], glm::vec3 subCubic2[4]);
}