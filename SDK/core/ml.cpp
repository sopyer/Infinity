#include <core/core.h>

namespace ml
{
#define MUL_MAT4_VEC4(r, m0, m1, m2, m3, v)         \
    {                                               \
        v128 swizzle;                               \
                                                    \
        swizzle = vi_swizzle<3, 3, 3, 3>(v);        \
        r = vi_mul(swizzle, m3);                    \
        swizzle = vi_swizzle<2, 2, 2, 2>(v);        \
        r = vi_mad(swizzle, m2, r);                 \
        swizzle = vi_swizzle<1, 1, 1, 1>(v);        \
        r = vi_mad(swizzle, m1, r);                 \
        swizzle = vi_swizzle<0, 0, 0, 0>(v);        \
        r = vi_mad(swizzle, m0, r);                 \
    }                                               \


    //                                                  | w0  -z0   y0  x0|   |x1|   | w1   z1  -y1  x1|   |x0|
    //                                                  |                 |   |  |   |                 |   |  |
    //                                                  | z0   w0  -x0  y0|   |y1|   |-z1   w1   x1  y1|   |y0|
    //  q0 * q1 = (x0, y0, z0, w0) * (x1, y1, z1, w1) = |                 | * |  | = |                 | * |  |
    //                                                  |-y0   x0   w0  z0|   |z1|   | y1  -x1   w1  z1|   |z0|
    //                                                  |                 |   |  |   |                 |   |  |
    //                                                  |-x0  -y0  -z0  w0|   |w1|   |-x1  -y1  -z1  w1|   |w0|
    v128 mul_quat(v128 q0, v128 q1)
    {
        v128 r, m, s, m0, m1, m2;

        m  = vi_seti_x(VI_SIGN_MASK);

        s  = vi_swizzle<1, 1, 0, 0>(m);
        m0 = vi_swizzle<3, 2, 1, 0>(q0);
        m0 = vi_xor(s, m0);  

        s  = vi_swizzle<0, 1, 1, 0>(m);
        m1 = vi_swizzle<2, 3, 0, 1>(q0);
        m1 = vi_xor(s, m1);

        s  = vi_swizzle<1, 0, 1, 0>(m);
        m2 = vi_swizzle<1, 0, 3, 2>(q0);
        m2 = vi_xor(s, m2);

        MUL_MAT4_VEC4(r, m0, m1, m2, q0, q1);

        return r;
    }

    v128 rotate_vec3_quat(v128 q, v128 v)
    {
        v128 r, two, wx4;

        r   = vi_cross3(q, v);
        wx4 = vi_swizzle<3, 3, 3, 3>(q);
        r   = vi_mad(wx4, v, r);
        r   = vi_cross3(q, r);
        two = vi_set_all(2.0f);
        r   = vi_mad(two, r, v);

        return r;
    }

    v128 conjugate_quat(v128 q)
    {
        v128 mask, r;
        
        mask = vi_seti_x(VI_SIGN_MASK);
        mask = vi_swizzle<0, 0, 0, 3>(mask);
        r    = vi_xor(mask, q);

        return r;
    }

    v128 lerp_quat(v128 q0, v128 q1, float t)
    {
        float qdot = vi_dot4(q0, q1);

        if (qdot < 0)
        {
            q1 = vi_neg(q1);
        }

        return vi_lerp(q0, q1, t);
    }

    v128 normalize(v128 v)
    {
        v128 r0;

        float norm_sq = vi_dot4(v, v);

        if (norm_sq < ml::asfloat(VI_EPS7_AS_INT))
        {
            return vi_set_zero();
        }
        else
        {
            r0 = vi_sqrt(vi_set_all(norm_sq));
            r0 = vi_div(v, r0);
        }

        return r0;
    }

    v128 length(v128 v)
    {
        return vi_sqrt(vi_set_all(vi_dot4(v, v)));
    }

    v128 make_p3(v128 v)
    {
        return vi_shuffle<VI_AX, VI_AY, VI_AZ, VI_BX>(v, vi_seti_x(VI_ONE_ASINT));
    }

    v128 make_v3(v128 v)
    {
        v128 mask = vi_swizzle<0, 0, 0, 3>(vi_seti_x(0xFFFFFFFF));
        return vi_and(v, mask);
    }

    void mul_quat(quat* result, quat* a, quat* b)
    {
        v128 r, q0, q1;

        q0 = vi_loadu_v4(a);
        q1 = vi_loadu_v4(b);

        r = mul_quat(q0, q1);

        vi_storeu_v4(result, r);
    }

    void conjugate_quat(quat* result, quat* q)
    {
        v128 r, vq;

        vq   = vi_loadu_v4(q);

        r = conjugate_quat(vq);

        vi_storeu_v4(result, r);
    }

    v128 length_quat(v128 q)
    {
        return length(q);
    }

    v128 normalize_quat(v128 q)
    {
        return normalize(q);
    }

    void rotate_vec3_quat(vec3* result, quat* rot, vec3* pos)
    {
        v128 r, q, p;

        q = vi_loadu_v4(rot);
        p = vi_load_v3(pos);

        r = rotate_vec3_quat(q, p);

        vi_store_v3(result, r);
    }

    v128 translation_dual_quat(v128 rq, v128 dq)
    {
        v128 t, w;

        t = vi_cross3(rq, dq);
        w = vi_swizzle<3, 3, 3, 3>(rq);
        t = vi_mad(w, dq, t);
        w = vi_swizzle<3, 3, 3, 3>(dq);
        t = vi_sub(t, vi_mul(w, rq));
        t = vi_mul(vi_set_all(2.0f), t);

        return t;
    }

    void set_identity_dual_quat(dual_quat* dq)
    {
        vi_storeu_v4(&dq->real, vi_set_zero());
        vi_storeu_v4(&dq->dual, vi_set_zero());
        dq->real.w = 1.0f;
    }

#define COPY_QUAT(dst, src)                     \
    {                                           \
        v128 t = vi_loadu_v4(src);              \
        vi_storeu_v4(dst, t);                   \
    }

    void make_dual_quat(dual_quat* result, quat* orient, vec3* offset)
    {
        v128 r, c, q, p;

        COPY_QUAT(&result->real, orient);

        p = vi_load_v3(offset);
        q = vi_loadu_v4(orient);

        r = mul_quat(p, q);
        c = vi_set_all(0.5f);
        vi_storeu_v4(&result->dual, vi_mul(c, r));
    }

    void transform_vec3_dual_quat(vec3* result, dual_quat* dq, vec3* p)
    {
        v128 v, t, r, d;

        v = vi_load_v3(p);
        r = vi_loadu_v4(&dq->real);
        d = vi_loadu_v4(&dq->dual);

        v = rotate_vec3_quat(r, v);
        t = translation_dual_quat(r, d);

        t = vi_add(v, t);
        
        vi_store_v3(result, t);
    }

    void mul_dual_quat(dual_quat* result, dual_quat* dq0, dual_quat* dq1)
    {
        v128 qr0, qd0;
        v128 qr1, qd1;
        v128 r0, r1;

        qr0 = vi_loadu_v4(&dq0->real);
        qd0 = vi_loadu_v4(&dq0->dual);
        qr1 = vi_loadu_v4(&dq1->real);
        qd1 = vi_loadu_v4(&dq1->dual);

        r0 = mul_quat(qr0, qr1);

        vi_storeu_v4(&result->real, r0);

        r0 = mul_quat(qd0, qr1);
        r1 = mul_quat(qr0, qd1);
        r0 = vi_add(r0, r1);

        vi_storeu_v4(&result->dual, r0);
    }

    void conjugate_dual_quat(dual_quat* result, dual_quat* dq)
    {
        v128 r, q;

        q = vi_loadu_v4(&dq->real);
        r = conjugate_quat(q);
        vi_storeu_v4(&result->real, r);

        q = vi_loadu_v4(&dq->dual);
        r = conjugate_quat(q);
        vi_storeu_v4(&result->dual, r);
    }

    void get_translation_from_dual_quat(vec3* result, dual_quat* dq)
    {
        v128 t, r, d;

        r = vi_loadu_v4(&dq->real);
        d = vi_loadu_v4(&dq->dual);

        t = translation_dual_quat(r, d);

        vi_store_v3(result, t);
    }

    v128 transform_vec3_dual_quat(dual_quat* dq, v128 p)
    {
        v128 v, t, r, d;

        r = vi_loadu_v4(&dq->real);
        d = vi_loadu_v4(&dq->dual);

        v = rotate_vec3_quat(r, p);
        t = translation_dual_quat(r, d);

        return vi_add(v, t);
    }

    void transpose_mat4(v128* r/*[4]*/, v128* m/*[4]*/)
    {
        __m128i xmm0 = _mm_unpacklo_epi32(_mm_castps_si128(m[0]), _mm_castps_si128(m[1]));
        __m128i xmm1 = _mm_unpackhi_epi32(_mm_castps_si128(m[0]), _mm_castps_si128(m[1]));
        __m128i xmm2 = _mm_unpacklo_epi32(_mm_castps_si128(m[2]), _mm_castps_si128(m[3]));
        __m128i xmm3 = _mm_unpackhi_epi32(_mm_castps_si128(m[2]), _mm_castps_si128(m[3]));

        r[0] = _mm_castsi128_ps(_mm_unpacklo_epi64(xmm0, xmm2));
        r[1] = _mm_castsi128_ps(_mm_unpackhi_epi64(xmm0, xmm2));
        r[2] = _mm_castsi128_ps(_mm_unpacklo_epi64(xmm1, xmm3));
        r[3] = _mm_castsi128_ps(_mm_unpackhi_epi64(xmm1, xmm3));
    }

    void transpose_mat4x2(v128* r/*[4]*/, v128* m/*[4]*/)
    {
        __m128i xmm0 = _mm_unpacklo_epi32(_mm_castps_si128(m[0]), _mm_castps_si128(m[1]));
        __m128i xmm2 = _mm_unpacklo_epi32(_mm_castps_si128(m[2]), _mm_castps_si128(m[3]));

        r[0] = _mm_castsi128_ps(_mm_unpacklo_epi64(xmm0, xmm2));
        r[1] = _mm_castsi128_ps(_mm_unpackhi_epi64(xmm0, xmm2));
    }

    v128 mul_mat4_vec4(v128* m/*4*/, v128 v)
    {
        v128 res;
        MUL_MAT4_VEC4(res, m[0], m[1], m[2], m[3], v);

        return res;
    }

    void mul_mat4(v128* r/*4*/, v128* a/*4*/, v128* b/*4*/)
    {
        v128 res;
        MUL_MAT4_VEC4(res, a[0], a[1], a[2], a[3], b[0]);
        r[0] = res;
        MUL_MAT4_VEC4(res, a[0], a[1], a[2], a[3], b[1]);
        r[1] = res;
        MUL_MAT4_VEC4(res, a[0], a[1], a[2], a[3], b[2]);
        r[2] = res;
        MUL_MAT4_VEC4(res, a[0], a[1], a[2], a[3], b[3]);
        r[3] = res;
    }

    void make_translation_mat4(v128* res, float dx, float dy, float dz)
    {
        res[0] = vi_set_zero();
        res[1] = vi_set_zero();
        res[2] = vi_set_zero();
        res[0].m128_f32[0] = 1.0f;
        res[1].m128_f32[1] = 1.0f;
        res[2].m128_f32[2] = 1.0f;
        res[3] = vi_set(dx, dy, dz, 1.0f);
    }

    void make_rotation_mat4(v128* res, float angle, float axisx, float axisy, float axisz)
    {
        float c = ml::cos(angle);
        float s = ml::sin(angle);

        res[0].m128_f32[0] = (1 - c) * axisx * axisx + c;
        res[0].m128_f32[1] = (1 - c) * axisx * axisy + s * axisz;
        res[0].m128_f32[2] = (1 - c) * axisx * axisz - s * axisy;
        res[0].m128_f32[3] = 0.0f;

        res[1].m128_f32[0] = (1 - c) * axisy * axisx - s * axisz;
        res[1].m128_f32[1] = (1 - c) * axisy * axisy + c;
        res[1].m128_f32[2] = (1 - c) * axisy * axisz + s * axisx;
        res[1].m128_f32[3] = 0.0f;

        res[2].m128_f32[0] = (1 - c) * axisz * axisx + s * axisy;
        res[2].m128_f32[1] = (1 - c) * axisz * axisy - s * axisx;
        res[2].m128_f32[2] = (1 - c) * axisz * axisz + c;
        res[2].m128_f32[3] = 0.0f;

        res[3].m128_f32[0] = 0.0f;
        res[3].m128_f32[1] = 0.0f;
        res[3].m128_f32[2] = 0.0f;
        res[3].m128_f32[3] = 1.0f;
    }

    //  | a11   a12   a13  a14|   | w -z  y  x|   | w -z  y -x|
    //  |                     |   |           |   |           |
    //  | a21   a22   a23  a24|   | z  w -x  y|   | z  w -x -y|
    //  |                     | = |           | * |           |
    //  | a31   a32   a33  a34|   |-y  x  w  z|   |-y  x  w -z|
    //  |                     |   |           |   |           |
    //  | a41   a42   a43  a44|   |-x -y -z  w|   | x  y  z  w|
    void quat_to_mat4x3(v128* mat/*[3]*/, v128 q)
    {
        v128 r, m, s, m0, m1, m2, v;

        m  = vi_seti_x(VI_SIGN_MASK);

        s  = vi_swizzle<1, 1, 0, 0>(m);
        m0 = vi_swizzle<3, 2, 1, 0>(q);
        m0 = vi_xor(s, m0);  

        s  = vi_swizzle<0, 1, 1, 0>(m);
        m1 = vi_swizzle<2, 3, 0, 1>(q);
        m1 = vi_xor(s, m1);

        s  = vi_swizzle<1, 0, 1, 0>(m);
        m2 = vi_swizzle<1, 0, 3, 2>(q);
        m2 = vi_xor(s, m2);

        s = vi_swizzle<1, 1, 1, 0>(m);
        v = vi_xor(m0, s);
        MUL_MAT4_VEC4(r, m0, m1, m2, q, v);
        mat[0] = r;

        s = vi_swizzle<1, 1, 1, 0>(m);
        v = vi_xor(m1, s);
        MUL_MAT4_VEC4(r, m0, m1, m2, q, v);
        mat[1] = r;

        s = vi_swizzle<1, 1, 1, 0>(m);
        v = vi_xor(m2, s);
        MUL_MAT4_VEC4(r, m0, m1, m2, q, v);
        mat[2] = r;
    }

    void quat_to_mat4(v128* m/*[4]*/, v128 q)
    {
        quat_to_mat4x3(m, q);
        m[3] = vi_swizzle<1, 1, 1, 0>(vi_seti_x(VI_ONE_ASINT));
    }

    //  q.x = sign(m21 - m12) * 0.5 * sqrt( max( 0, 1 + m00 - m11 - m22 ) );
    //  q.y = sign(m02 - m20) * 0.5 * sqrt( max( 0, 1 - m00 + m11 - m22 ) );
    //  q.z = sign(m10 - m01) * 0.5 * sqrt( max( 0, 1 - m00 - m11 + m22 ) );
    //  q.w =                   0.5 * sqrt( max( 0, 1 + m00 + m11 + m22 ) );
    v128 mat4x3_to_quat(v128* m/*[4]*/)
    {
        v128 s, q, sc, m00, m11, m22, ml, mr, cmp;

        s = vi_seti_x(VI_SIGN_MASK);

        q  = vi_seti_all(VI_ONE_ASINT);

        sc  = vi_swizzle<1, 0, 0, 1>(s);
        m00 = vi_set_all(m[0].m128_f32[0]);
        m00 = vi_xor(m00, sc);
        q   = vi_add(m00, q);

        sc  = vi_swizzle<0, 1, 0, 1>(s);
        m11 = vi_set_all(m[1].m128_f32[1]);
        m11 = vi_xor(m11, sc);
        q   = vi_add(m11, q);

        sc  = vi_swizzle<0, 0, 1, 1>(s);
        m22 = vi_set_all(m[2].m128_f32[2]);
        m22 = vi_xor(m22, sc);
        q   = vi_add(m22, q);

        q = vi_mul(vi_set_all(0.5), vi_sqrt(q));

        ml = vi_set(m[1].m128_f32[2], m[2].m128_f32[0], m[0].m128_f32[1], 0.0f);
        mr = vi_set(m[2].m128_f32[1], m[0].m128_f32[2], m[1].m128_f32[0], 0.0f);

        cmp = vi_cmp_lt(ml, mr);
        sc  = vi_and(cmp, vi_seti_all(VI_SIGN_MASK));

        q = vi_or(sc, q);

        return q;
    }

    v128 axis_angle_to_quat(v128 axis, float angle)
    {
        float a  = angle*0.5f;
        v128  co;
        v128  si;

        co = vi_set_x(cos(a));
        si = vi_set_x(sin(a));

        co = vi_swizzle<1, 1, 1, 0>(co);
        si = vi_swizzle<0, 0, 0, 1>(si);

        return vi_xor(vi_mul(axis, si), co);
    }

    void make_perspective_mat4(v128* mat/*[4]*/, float fovy, float aspect, float zn, float zf)
    {
        float e = 1.0f / tan(0.5f * fovy);

        mat[0] = vi_set_zero();
        mat[1] = vi_set_zero();
        mat[2] = vi_set_zero();
        mat[3] = vi_set_zero();

        mat[0].m128_f32[0] = e / aspect;
        mat[1].m128_f32[1] = e;
        mat[2].m128_f32[2] = -(zf + zn) / (zf - zn);
        mat[2].m128_f32[3] = -1;
        mat[3].m128_f32[2] = -2.0f * zn * zf / (zf - zn);
    }

    void make_inf_perspective_mat4(v128* mat/*[4]*/, float fovy, float aspect, float zn)
    {
        mem_zero<v128>(mat, 4);

        float e = 1.0f / ml::tan(0.5f * fovy);

        mat[0].m128_f32[0] = e / aspect;
        mat[1].m128_f32[1] = e;
        mat[2].m128_f32[2] = -1;
        mat[2].m128_f32[3] = -1;
        mat[3].m128_f32[2] = -2.0f * zn;
    }

    void make_ortho2D_mat4(v128* mat/*[4]*/, float left, float right, float bottom, float top)
    {
        mem_zero<v128>(mat, 4);

        mat[0].m128_f32[0] = 2.0f / (right - left);
        mat[1].m128_f32[1] = 2.0f / (top - bottom);
        mat[2].m128_f32[2] = 1.0f;
        mat[3].m128_f32[0] = -(right + left) / (right - left);
        mat[3].m128_f32[1] = -(top + bottom) / (top - bottom);
        mat[3].m128_f32[3] = 1.0f;
    }

    void make_ortho3D_mat4(v128* mat/*[4]*/, float left, float right, float bottom, float top, float zn, float zf)
    {
        mem_zero<v128>(mat, 4);

        mat[0].m128_f32[0] = 2.0f / (right - left);
        mat[1].m128_f32[1] = 2.0f / (top - bottom);
        mat[2].m128_f32[2] = 2.0f / (zf - zn);
        mat[2].m128_f32[2] = 1.0f;
        mat[3].m128_f32[0] = -(right + left) / (right - left);
        mat[3].m128_f32[1] = -(top + bottom) / (top - bottom);
        mat[3].m128_f32[2] = -(zf + zn) / (zf - zn);
        mat[3].m128_f32[3] = 1.0f;
    }

    void make_identity_mat4(v128* mat/*[4]*/)
    {
        v128 one = vi_seti_x(VI_ONE_ASINT);
        mat[0] = one;
        mat[1] = vi_swizzle<1, 0, 1, 1>(one);
        mat[2] = vi_swizzle<1, 1, 0, 1>(one);
        mat[3] = vi_swizzle<1, 1, 1, 0>(one);
    }

    void quat_vec3_to_mat4(v128* m/*[4]*/, v128 quat, v128 pos)
    {
        ml::quat_to_mat4x3(m, quat);
        m[3] = ml::conjugate_quat(ml::rotate_vec3_quat(quat, pos));
    }

    v128 distanceToAABB(v128 vmin, v128 vmax, v128 pt)
    {
        v128  deltaMin, deltaMax;
        v128  maskMin,  maskMax;
        v128  vdist;

        deltaMin = vi_sub(vmin, pt);
        maskMin  = vi_cmp_gt(deltaMin, vi_set_zero());
        deltaMax = vi_sub(pt, vmax);
        maskMax  = vi_cmp_gt(deltaMax, vi_set_zero());
        vdist    = vi_add(vi_and(maskMax, deltaMax), vi_and(maskMin, deltaMin));

        return vdist;
    }

    bool sphereAABBTest2(aabb* AABB, v128 center, float radius, float res)
    {
        v128  deltaMin, deltaMax;
        v128  maskMin,  maskMax;
        v128  vdist;
        float d;

        deltaMin = vi_sub(AABB->min, center);
        maskMin  = vi_cmp_gt(deltaMin, vi_set_zero());
        deltaMax = vi_sub(center, AABB->max);
        maskMax  = vi_cmp_gt(deltaMax, vi_set_zero());
        vdist    = vi_add(vi_and(maskMax, deltaMax), vi_and(maskMin, deltaMin));
        d        = vi_dot3(vdist, vdist);

        assert(vi_all(vi_cmp_eq(vi_and(maskMin, maskMax), vi_set_zero())));
        assert(d == res);

        return d<=radius*radius;
    }

    bool sphereAABBTest(aabb* AABB, v128 center, float radius)
    {
        v128 deltaMin, deltaMax;
        v128 maskMin, maskMax;
        float res;

        deltaMin = vi_sub(AABB->min, center);
        maskMin  = vi_cmp_gt(deltaMin, vi_set_zero());
        deltaMin = vi_and(maskMin, deltaMin);
        res = vi_dot3(deltaMin, deltaMin);

        deltaMax = vi_sub(center, AABB->max);
        maskMax  = vi_cmp_gt(deltaMax, vi_set_zero());
        //maskMax = vi_andnot(maskMax, maskMin);
        assert(vi_all(vi_cmp_eq(vi_and(maskMin, maskMax), vi_set_zero())));
        deltaMax = vi_and(maskMax, deltaMax);
        res += vi_dot3(deltaMax, deltaMax);

        float d2 = res;

        bool res1 = d2<=radius*radius;
        bool res2 = sphereAABBTest2(AABB, center, radius, res);

        assert(res2==res1);

        return res2;
    }

    void transformPointsSOA4(v128* dest, v128 xxxx, v128 yyyy, v128 zzzz, /*vec4 wwww,*/ const mat4x4* matrix)
    {
        v128 tmp;

#define COMP(c) \
    tmp = vi_swizzle<c, c, c, c>(matrix->r3); \
    tmp = vi_mad(zzzz, vi_swizzle<c, c, c, c>(matrix->r2), tmp); \
    tmp = vi_mad(yyyy, vi_swizzle<c, c, c, c>(matrix->r1), tmp); \
    dest[c] = vi_mad(xxxx, vi_swizzle<c, c, c, c>(matrix->r0), tmp); 

        COMP(0);
        COMP(1);
        COMP(2);
        COMP(3);

#undef COMP
    }

    int intersectionTest(const aabb* bbox, const mat4x4* frustum)
    {
        v128 min = bbox->min;
        v128 max = bbox->max;

        //transform AABB vertices into SOA representation
        v128 xXxX, yyYY, zzzz, ZZZZ;

        xXxX = vi_shuffle<VI_AX, VI_BX, VI_AX, VI_BX>(min, max);
        yyYY = vi_shuffle<VI_AY, VI_AY, VI_BY, VI_BY>(min, max);
        zzzz = vi_swizzle<2, 2, 2, 2>(min);
        ZZZZ = vi_swizzle<2, 2, 2, 2>(max);

        v128 points_cs_0[4];
        v128 points_cs_1[4];

        transformPointsSOA4(points_cs_0, xXxX, yyYY, zzzz, frustum);
        transformPointsSOA4(points_cs_1, xXxX, yyYY, ZZZZ, frustum);

        v128 points_cs_0_negw = vi_neg(points_cs_0[3]);
        v128 points_cs_1_negw = vi_neg(points_cs_1[3]);

#define NOUT(a, b, c, d) vi_or(vi_cmp_gt(a, b), vi_cmp_gt(c, d))
#define TEST_PLANE(a, b, c, d)           \
            testRes = NOUT(a, b, c, d);  \
            nout  &= vi_any(testRes);    \
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

#include <math.h>

// Scalar math functions
namespace ml
{
    int32_t asint(float f)
    {
        return _mm_cvtsi128_si32(_mm_castps_si128(_mm_set_ss(f)));
    }

    float asfloat(int32_t i)
    {
        return _mm_cvtss_f32(_mm_castsi128_ps(_mm_cvtsi32_si128(i)));
    }

    float abs(float x)
    {
        return asfloat(asint(x) & 0x7FFFFFFF);
    }

    float exp  (float x)
    {
        return ::exp(x);
    }

    float pow  (float x, float y)
    {
        return ::pow(x, y);
    }

    float ln(float x)
    {
        return ::logf(x);
    }

    float lg2(float x)
    {
        return ::log2f(x);
    }

    float lg10(float x)
    {
        return ::log10f(x);
    }

    float sqrt(float x)
    {
        return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
    }

    float mod(float x, float y)
    {
        return ::fmodf(x, y);
    }

    float floor(float x)
    {
        return ::floorf(x);
    }

    float ceil(float x)
    {
        return ::ceilf(x);
    }

    float sin(float x)
    {
        return ::sinf(x);
    }

    float cos(float x)
    {
        return ::cosf(x);
    }

    float tan(float x)
    {
        return ::tan(x);
    }

    float asin(float x)
    {
        return ::asinf(x);
    }

    float atan2(float y, float x)
    {
        return ::atan2(y, x);
    }
}
