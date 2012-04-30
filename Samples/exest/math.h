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

    inline vec4 distanceToAABB(aabb* AABB, vec4 pt)
    {
		vec4  deltaMin, deltaMax;
        vec4  maskMin,  maskMax;
        vec4  vdist;

        deltaMin = vi_sub(AABB->min, pt);
        maskMin  = vi_cmp_gt(deltaMin, vi_load_zero());
		deltaMax = vi_sub(pt, AABB->max);
        maskMax  = vi_cmp_gt(deltaMax, vi_load_zero());
        vdist    = vi_add(vi_and(maskMax, deltaMax), vi_and(maskMin, deltaMin));

        return vdist;
    }

    inline bool sphereAABBTest2(aabb* AABB, vec4 center, float radius, vec4 res)
    {
		vec4  deltaMin, deltaMax;
        vec4  maskMin,  maskMax;
        vec4  vdist;
        float d;

        deltaMin = vi_sub(AABB->min, center);
        maskMin  = vi_cmp_gt(deltaMin, vi_load_zero());
		deltaMax = vi_sub(center, AABB->max);
        maskMax  = vi_cmp_gt(deltaMax, vi_load_zero());
        vdist    = vi_add(vi_and(maskMax, deltaMax), vi_and(maskMin, deltaMin));
        vdist    = vi_dot3(vdist, vdist);

        assert(vi_all(vi_cmp_eq(vi_and(maskMin, maskMax), vi_load_zero())));
        assert(vi_all(vi_cmp_eq(vdist, res)));
        
        _mm_store_ss(&d, vdist);
        return d<=radius*radius;
    }

	inline bool sphereAABBTest(aabb* AABB, vec4 center, float radius)
	{
		vec4 deltaMin, deltaMax;
        vec4 maskMin, maskMax;
        vec4 res;

		deltaMin = vi_sub(AABB->min, center);
		maskMin  = vi_cmp_gt(deltaMin, vi_load_zero());
		deltaMin = vi_and(maskMin, deltaMin);
		res = vi_dot3(deltaMin, deltaMin);

		deltaMax = vi_sub(center, AABB->max);
		maskMax  = vi_cmp_gt(deltaMax, vi_load_zero());
		//maskMax = vi_andnot(maskMax, maskMin);
        assert(vi_all(vi_cmp_eq(vi_and(maskMin, maskMax), vi_load_zero())));
		deltaMax = vi_and(maskMax, deltaMax);
		res = vi_add(res, vi_dot3(deltaMax, deltaMax));

        float d2;
        _mm_store_ss(&d2, res);
        
        bool res1 = d2<radius*radius;
        bool res2 = sphereAABBTest2(AABB, center, radius, res);
        
        assert(res2==res1);

		return res2;
	}

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

	enum
	{
		IT_INSIDE,
		IT_OUTSIDE,
		IT_INTERSECT
	};

	inline int intersectionTest(const aabb* bbox, const mat4x4* frustum)
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
