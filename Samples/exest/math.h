#pragma once

#include <vi.h>

namespace ml
{
	struct mat4x4
	{
		vec4	r0;
		vec4	r1;
		vec4	r2;
		vec4	r3;
	};

	struct aabb
	{
		vec4 min, max;
	};

	inline void transformPointsSOA4(vec4* dest, vec4 xxxx, vec4 yyyy, vec4 zzzz, /*vec4 wwww,*/ const mat4x4* matrix)
	{
		vec4 tmp;

#define COMP(c) \
	tmp = vi_swizzle<VI_SWIZZLE_MASK(c, c, c, c)>(matrix->r3); \
	tmp = vi_mad(zzzz, vi_swizzle<VI_SWIZZLE_MASK(c, c, c, c)>(matrix->r2), tmp); \
	tmp = vi_mad(yyyy, vi_swizzle<VI_SWIZZLE_MASK(c, c, c, c)>(matrix->r1), tmp); \
	dest[c] = vi_mad(xxxx, vi_swizzle<VI_SWIZZLE_MASK(c, c, c, c)>(matrix->r0), tmp); 

		COMP(0);
		COMP(1);
		COMP(2);
		COMP(3);

#undef COMP
	}

	enum IntersectionResult
	{
		IT_INSIDE,
		IT_OUTSIDE,
		IT_INTERSECT
	};

	inline IntersectionResult intersectionTest(const aabb* bbox, const mat4x4* frustum)
	{
		vec4 min = bbox->min;
		vec4 max = bbox->max;

		//transform AABB vertices into SOA representation
		vec4 xXxX, yyYY, zzzz, ZZZZ;
		const int mask_xXxX = VI_SHUFFLE_MASK(VI_A_X, VI_B_X, VI_A_X, VI_B_X);
		const int mask_yyYY = VI_SHUFFLE_MASK(VI_A_Y, VI_A_Y, VI_B_Y, VI_B_Y);

		xXxX = vi_shuffle<mask_xXxX>(min, max);
		yyYY = vi_shuffle<mask_yyYY>(min, max);
		zzzz = vi_swizzle<VI_SWIZZLE_ZZZZ>(min);
		ZZZZ = vi_swizzle<VI_SWIZZLE_ZZZZ>(max);

		vec4 points_cs_0[4];
		vec4 points_cs_1[4];

		transformPointsSOA4(points_cs_0, xXxX, yyYY, zzzz, frustum);
		transformPointsSOA4(points_cs_1, xXxX, yyYY, ZZZZ, frustum);

		vec4 points_cs_0_negw = vi_neg(points_cs_0[3]);
		vec4 points_cs_1_negw = vi_neg(points_cs_1[3]);

#define NOUT(a, b, c, d) vi_or(vi_cmp_gt(a, b), vi_cmp_gt(c, d))
#define TEST_PLANE(a, b, c, d) \
	testRes = NOUT(a, b, c, d);\
	nout &= vi_any(testRes);\
	allin &= vi_all(testRes);

		vec4 testRes;
		bool nout=true, allin=true;

		TEST_PLANE(points_cs_0[0], points_cs_0_negw, points_cs_1[0], points_cs_1_negw);
		TEST_PLANE(points_cs_0[3], points_cs_0[0], points_cs_1[3], points_cs_1[0]);
		TEST_PLANE(points_cs_0[1], points_cs_0_negw, points_cs_1[1], points_cs_1_negw);
		TEST_PLANE(points_cs_0[3], points_cs_0[1], points_cs_1[3], points_cs_1[1]);
		TEST_PLANE(points_cs_0[2], points_cs_0_negw/*vi_set_all(0.0f)*/, points_cs_1[2], points_cs_1_negw/*vi_set_all(0.0f)*/);
		TEST_PLANE(points_cs_0[3], points_cs_0[2], points_cs_1[3], points_cs_1[2]);

#undef TEST_PLANE
#undef NOUT

		return nout?(allin?IT_INSIDE:IT_INTERSECT):IT_OUTSIDE;
	}
}
