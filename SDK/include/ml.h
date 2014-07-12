#pragma once

#include <stdint.h>
#include <assert.h>

#include <vi.h>

#define FLT_PI               3.141592653589793f
#define FLT_2_PI             6.283185307179586f
#define FLT_PI_OVER_2        1.570796326794897f
#define FLT_1_OVER_PI        0.318309886183791f
#define FLT_1_OVER_2_PI      0.159154943091895f
#define FLT_RAD_TO_DEG_SCALE 57.29577951308232f
#define FLT_DEG_TO_RAD_SCALE 0.017453292519943f

namespace ml
{
    struct vec2
    {
        float x, y;
    };

    struct vec3
    {
        float x, y, z;
    };

    struct vec4
    {
        float x, y, z, w;
    };

    struct quat
    {
        float x, y, z, w;
    };

    struct dual_quat
    {
        quat real;
        quat dual;
    };

    struct mat4x4
    {
        v128 r0;
        v128 r1;
        v128 r2;
        v128 r3;
    };

    struct aabb
    {
        v128 min, max;
    };

    // quaternions
    void mul_quat        (quat* result, quat* a, quat* b); // multiplies 2 quaternions, returns pointer to result
    void conjugate_quat  (quat* result, quat* q);
    void rotate_vec3_quat(vec3* result, quat* q, vec3* p);

    //v128 version of quaternion operations
    v128 mul_quat             (v128 q0, v128 q1);
    v128 rotate_vec3_quat     (v128 q, v128 v);
    v128 conjugate_quat       (v128 q);
    v128 lerp_quat            (v128 q0, v128 q1, float t);
    v128 translation_dual_quat(v128 rq, v128 dq);
    v128 length_quat          (v128 q);
    v128 normalize_quat       (v128 q);

    v128 normalize            (v128 v);
    v128 length               (v128 v);

    v128 make_p3(v128 v);   // set v.w = 1.0f
    v128 make_v3(v128 v);   // set v.w = 0.0f

    // dual quaternions
    void set_identity_dual_quat        (dual_quat* dq);
    void make_dual_quat                (dual_quat* result, quat* orient, vec3* offset);
    void mul_dual_quat                 (dual_quat* result, dual_quat* dq0, dual_quat* dq1);
    void conjugate_dual_quat           (dual_quat* result, dual_quat* dq);
    void transform_vec3_dual_quat      (vec3* result, dual_quat* dq, vec3* p);
    void get_translation_from_dual_quat(vec3* result, dual_quat* dq);
    v128 transform_vec3_dual_quat      (dual_quat* dq, v128 p);

    // matrices
    v128 mul_mat4_vec4     (v128* m/*[4]*/, v128 v);
    void transpose_mat4    (v128* r/*[4]*/, v128* m/*[4]*/);
    void mul_mat4          (v128* r/*[4]*/, v128* a/*[4]*/, v128* b/*[4]*/);
    void quat_to_mat4x3    (v128* m/*[3]*/, v128 q);
    void quat_to_mat4      (v128* m/*[4]*/, v128 q);
    void quat_vec3_to_mat4 (v128* m/*[4]*/, v128 quat, v128 pos);
    v128 mat4x3_to_quat    (v128* m/*[4]*/);
    v128 axis_angle_to_quat(v128 axis, float angle);

    void make_perspective_mat4    (v128* mat/*[4]*/, float fovy, float aspect, float zn, float zf);
    void make_inf_perspective_mat4(v128* mat/*[4]*/, float fovy, float aspect, float zn);
    void make_ortho2D_mat4        (v128* mat/*[4]*/, float left, float right, float bottom, float top);
    void make_ortho3D_mat4        (v128* mat/*[4]*/, float left, float right, float bottom, float top, float zn, float zf);
    void make_identity_mat4       (v128* mat/*[4]*/);
}

namespace ml
{
    inline v128 length_quat(v128 q)
    {
        return length(q);
    }

    inline v128 normalize_quat(v128 q)
    {
        return normalize(q);
    }


    inline v128 distanceToAABB(v128 vmin, v128 vmax, v128 pt)
    {
        v128  deltaMin, deltaMax;
        v128  maskMin,  maskMax;
        v128  vdist;

        deltaMin = vi_sub(vmin, pt);
        maskMin  = vi_cmp_gt(deltaMin, vi_set_0000());
        deltaMax = vi_sub(pt, vmax);
        maskMax  = vi_cmp_gt(deltaMax, vi_set_0000());
        vdist    = vi_add(vi_and(maskMax, deltaMax), vi_and(maskMin, deltaMin));

        return vdist;
    }

    inline bool sphereAABBTest2(aabb* AABB, v128 center, float radius, v128 res)
    {
        v128  deltaMin, deltaMax;
        v128  maskMin,  maskMax;
        v128  vdist;
        float d;

        deltaMin = vi_sub(AABB->min, center);
        maskMin  = vi_cmp_gt(deltaMin, vi_set_0000());
        deltaMax = vi_sub(center, AABB->max);
        maskMax  = vi_cmp_gt(deltaMax, vi_set_0000());
        vdist    = vi_add(vi_and(maskMax, deltaMax), vi_and(maskMin, deltaMin));
        vdist    = vi_dot3(vdist, vdist);

        assert(vi_all(vi_cmp_eq(vi_and(maskMin, maskMax), vi_set_0000())));
        assert(vi_all(vi_cmp_eq(vdist, res)));

        d = _mm_cvtss_f32(vdist);
        return d<=radius*radius;
    }

    inline bool sphereAABBTest(aabb* AABB, v128 center, float radius)
    {
        v128 deltaMin, deltaMax;
        v128 maskMin, maskMax;
        v128 res;

        deltaMin = vi_sub(AABB->min, center);
        maskMin  = vi_cmp_gt(deltaMin, vi_set_0000());
        deltaMin = vi_and(maskMin, deltaMin);
        res = vi_dot3(deltaMin, deltaMin);

        deltaMax = vi_sub(center, AABB->max);
        maskMax  = vi_cmp_gt(deltaMax, vi_set_0000());
        //maskMax = vi_andnot(maskMax, maskMin);
        assert(vi_all(vi_cmp_eq(vi_and(maskMin, maskMax), vi_set_0000())));
        deltaMax = vi_and(maskMax, deltaMax);
        res = vi_add(res, vi_dot3(deltaMax, deltaMax));

        float d2 = _mm_cvtss_f32(res);

        bool res1 = d2<=radius*radius;
        bool res2 = sphereAABBTest2(AABB, center, radius, res);

        assert(res2==res1);

        return res2;
    }

    inline void transformPointsSOA4(v128* dest, v128 xxxx, v128 yyyy, v128 zzzz, /*vec4 wwww,*/ const mat4x4* matrix)
    {
        v128 tmp;

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
        v128 min = bbox->min;
        v128 max = bbox->max;

        //transform AABB vertices into SOA representation
        v128 xXxX, yyYY, zzzz, ZZZZ;
        const int mask_xXxX = VI_SHUFFLE_MASK(VI_A_X, VI_B_X, VI_A_X, VI_B_X);
        const int mask_yyYY = VI_SHUFFLE_MASK(VI_A_Y, VI_A_Y, VI_B_Y, VI_B_Y);

        xXxX = vi_shuffle<mask_xXxX>(min, max);
        yyYY = vi_shuffle<mask_yyYY>(min, max);
        zzzz = vi_swizzle<VI_SWIZZLE_ZZZZ>(min);
        ZZZZ = vi_swizzle<VI_SWIZZLE_ZZZZ>(max);

        v128 points_cs_0[4];
        v128 points_cs_1[4];

        transformPointsSOA4(points_cs_0, xXxX, yyYY, zzzz, frustum);
        transformPointsSOA4(points_cs_1, xXxX, yyYY, ZZZZ, frustum);

        v128 points_cs_0_negw = vi_neg(points_cs_0[3]);
        v128 points_cs_1_negw = vi_neg(points_cs_1[3]);

#define NOUT(a, b, c, d) vi_or(vi_cmp_gt(a, b), vi_cmp_gt(c, d))
#define TEST_PLANE(a, b, c, d) \
    testRes = NOUT(a, b, c, d);\
    nout &= vi_any(testRes);\
    allin &= vi_all(testRes);

        v128 testRes;
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

// Scalar math functions
namespace ml
{
    int32_t asint(float f);
    float   asfloat(int32_t i);

    float exp  (float x);
    float pow  (float x, float y);
    float sqrt (float x);
    float sin  (float x);
    float cos  (float x);
    float tan  (float x);
    float asin (float x);
    float atan2(float y, float x);

    float abs  (float x);
    float floor(float x);
    float ceil (float x);

    float mod(float x, float y);
}
