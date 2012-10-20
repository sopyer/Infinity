#include "ml.h"

namespace ml
{
    quat      identity_quat = {0, 0, 0, 1};
    dual_quat identity_dual_quat = {{0, 0, 0, 1}, {0, 0, 0, 0}};

    v128 load_vec2(vec2* v2)
    {
        return _mm_castpd_ps(_mm_load_sd((double*)&v2->x));
    }

    v128 load_vec3(vec3* v3)
    {
        v128 r0, r1;

        r0 = _mm_castpd_ps(_mm_load_sd((double*)&v3->x));
        r1 = _mm_load_ss(&v3->z);
        r1 = _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(r1), 8));

        return _mm_xor_ps(r0, r1);
    }

    v128 load_quat(quat* q)
    {
        return vi_loadu(q);
    }

    void store_vec2(vec2* v2, v128 v4)
    {
        _mm_store_sd((double*)&v2->x, _mm_castps_pd(v4));
    }

    void store_vec3(vec3* v3, v128 v4)
    {
        _mm_store_sd((double*)&v3->x, _mm_castps_pd(v4));
        _mm_store_ss(&v3->z, _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(v4), 8)));
    }

    void store_quat(quat* q, v128 v4)
    {
        vi_storeu(q, v4);
    }

    v128 mul_quat(v128 q0, v128 q1)
    {
        v128 op1, op2, m, r;

        m   = vi_set_x000_i32(0x80000000);

        op1 = vi_swizzle<VI_SWIZZLE_WWWW>(q0);
        r   = vi_mul(op1, q1);  

        m   = vi_swizzle<VI_SWIZZLE_MASK(1, 0, 1, 0)>(m);
        op1 = vi_swizzle<VI_SWIZZLE_XXXX>(q0);
        op1 = vi_xor(m, op1);  
        op2 = vi_swizzle<VI_SWIZZLE_MASK(3, 2, 1, 0)>(q1);
        r   = vi_mad(op1, op2, r);

        m   = vi_swizzle<VI_SWIZZLE_MASK(2, 2, 3, 3)>(m);
        op1 = vi_swizzle<VI_SWIZZLE_YYYY>(q0);
        op1 = vi_xor(m, op1);
        op2 = vi_swizzle<VI_SWIZZLE_MASK(1, 0, 3, 2)>(op2);
        r   = vi_mad(op1, op2, r);

        m   = vi_swizzle<VI_SWIZZLE_MASK(3, 0, 0, 3)>(m);
        op1 = vi_swizzle<VI_SWIZZLE_ZZZZ>(q0);
        op1 = vi_xor(m, op1);
        op2 = vi_swizzle<VI_SWIZZLE_MASK(3, 2, 1, 0)>(op2);
        r   = vi_mad(op1, op2, r);

        return r;
    }

    v128 rotate_quat_vec3(v128 q, v128 v)
    {
        v128 r, two, wx4;

        r   = vi_cross3(q, v);
        wx4 = vi_swizzle<VI_SWIZZLE_WWWW>(q);
        r   = vi_mad(wx4, v, r);
        r   = vi_cross3(q, r);
        two = vi_set_xxxx(2.0f);
        r   = vi_mad(two, r, v);

        return r;
    }

    v128 conjugate_quat(v128 q)
    {
        v128 mask, r;
        
        mask = vi_set_x000_i32(0x80000000);
        mask = vi_swizzle<VI_SWIZZLE_MASK(0, 0, 0, 3)>(mask);
        r    = vi_xor(mask, q);

        return r;
    }

    v128 lerp_quat(v128 q0, v128 q1, float t)
    {
        v128 r0;
        
        r0 = vi_dot4(q0, q1);

        if (vi_cmpx_lt(r0, vi_load_zero()))
        {
            q1 = vi_neg(q1);
        }

        return vi_lerp(q0, q1, t);
    }

    v128 normalize(v128 v)
    {
        v128 r0, r1;

        r0 = vi_dot4(v, v);

        if (_mm_comilt_ss(r0, vi_set_xxxx(VI_EPS7)))
        {
            r0 = vi_load_zero();
        }
        else
        {
            r1 = vi_sqrt(r0);
            r0 = vi_div(v, r1);
        }

        return r0;
    }

    v128 length(v128 v)
    {
        return vi_sqrt(vi_dot4(v, v));

    }

    void mul_quat(quat* result, quat* a, quat* b)
    {
        v128 r, q0, q1;

        q0 = load_quat(a);
        q1 = load_quat(b);

        r = mul_quat(q0, q1);

        store_quat(result, r);
    }

    void conjugate_quat(quat* result, quat* q)
    {
        v128 r, vq;

        vq   = vi_load(q);

        r = conjugate_quat(vq);

        store_quat(result, r);
    }

    void rotate_vec3_quat(vec3* result, quat* rot, vec3* pos)
    {
        v128 r, q, p;

        q = load_quat(rot);
        p = load_vec3(pos);

        r = rotate_quat_vec3(q, p);

        store_vec3(result, r);
    }

    v128 translation_dual_quat(v128 rq, v128 dq)
    {
        v128 t, w;

        t = vi_cross3(rq, dq);
        w = vi_swizzle<VI_SWIZZLE_WWWW>(rq);
        t = vi_mad(w, dq, t);
        w = vi_swizzle<VI_SWIZZLE_WWWW>(dq);
        t = vi_sub(t, vi_mul(w, rq));
        t = vi_mul(vi_set_xxxx(2.0f), t);

        return t;
    }

#define COPY_QUAT(dst, src)                     \
    {                                           \
        v128 t = vi_loadu(src);                 \
        vi_storeu(dst, t);                      \
    }

    void create_dual_quat(dual_quat* result, quat* orient, vec3* offset)
    {
        v128 r, c, q, p;

        COPY_QUAT(&result->real, orient);

        p = load_vec3(offset);
        q = load_quat(orient);

        r = mul_quat(p, q);
        c = vi_set_xxxx(0.5f);
        store_quat(&result->dual, vi_mul(c, r));
    }

    void transform_vec3_dual_quat(vec3* result, dual_quat* dq, vec3* p)
    {
        v128 v, t, r, d;

        v = load_vec3(p);
        r = load_quat(&dq->real);
        d = load_quat(&dq->dual);

        v = rotate_quat_vec3(r, v);
        t = translation_dual_quat(r, d);

        t = vi_add(v, t);
        
        store_vec3(result, t);
    }

    void mul_dual_quat(dual_quat* result, dual_quat* dq0, dual_quat* dq1)
    {
        v128 qr0, qd0;
        v128 qr1, qd1;
        v128 r0, r1;

        qr0 = load_quat(&dq0->real);
        qd0 = load_quat(&dq0->dual);
        qr1 = load_quat(&dq1->real);
        qd1 = load_quat(&dq1->dual);

        r0 = mul_quat(qr0, qr1);

        store_quat(&result->real, r0);

        r0 = mul_quat(qd0, qr1);
        r1 = mul_quat(qr0, qd1);
        r0 = vi_add(r0, r1);

        store_quat(&result->dual, r0);
    }

    void conjugate_dual_quat(dual_quat* result, dual_quat* dq)
    {
        v128 r, q;

        q = load_quat(&dq->real);
        r = conjugate_quat(q);
        store_quat(&result->real, r);

        q = load_quat(&dq->dual);
        r = conjugate_quat(q);
        store_quat(&result->dual, r);
    }

    void get_translation_from_dual_quat(vec3* result, dual_quat* dq)
    {
        v128 t, r, d;

        r = load_quat(&dq->real);
        d = load_quat(&dq->dual);

        t = translation_dual_quat(r, d);

        store_vec3(result, t);
    }

    void transpose(v128* transposed/*[4]*/, v128* mat/*[4]*/)
    {
        __m128i xmm0 = _mm_unpacklo_epi32(_mm_castps_si128(mat[0]), _mm_castps_si128(mat[1]));
        __m128i xmm1 = _mm_unpackhi_epi32(_mm_castps_si128(mat[0]), _mm_castps_si128(mat[1]));
        __m128i xmm2 = _mm_unpacklo_epi32(_mm_castps_si128(mat[2]), _mm_castps_si128(mat[3]));
        __m128i xmm3 = _mm_unpackhi_epi32(_mm_castps_si128(mat[2]), _mm_castps_si128(mat[3]));

        transposed[0] = _mm_castsi128_ps(_mm_unpacklo_epi64(xmm0, xmm2));
        transposed[1] = _mm_castsi128_ps(_mm_unpackhi_epi64(xmm0, xmm2));
        transposed[2] = _mm_castsi128_ps(_mm_unpacklo_epi64(xmm1, xmm3));
        transposed[3] = _mm_castsi128_ps(_mm_unpackhi_epi64(xmm1, xmm3));
    }

#	define MUL_MAT4_VEC4(r, m, v)                    \
    {                                                \
        v128 xxxx = vi_swizzle<VI_SWIZZLE_XXXX>(v);  \
        v128 yyyy = vi_swizzle<VI_SWIZZLE_YYYY>(v);  \
        v128 zzzz = vi_swizzle<VI_SWIZZLE_ZZZZ>(v);  \
        v128 wwww = vi_swizzle<VI_SWIZZLE_WWWW>(v);  \
        r = vi_mul(xxxx, m[0]);                      \
        r = vi_mad(yyyy, m[1], r);                   \
        r = vi_mad(zzzz, m[2], r);                   \
        r = vi_mad(wwww, m[3], r);                   \
    }

    void mul(v128* r, v128* m, v128 v)
    {
        v128 res;
        MUL_MAT4_VEC4(res, m, v);
        *r = res;
    }

    void mul(v128* r, v128* a, v128* b)
    {
        v128 res;
        MUL_MAT4_VEC4(res, a, b[0]);
        r[0] = res;
        MUL_MAT4_VEC4(res, a, b[1]);
        r[1] = res;
        MUL_MAT4_VEC4(res, a, b[2]);
        r[2] = res;
        MUL_MAT4_VEC4(res, a, b[3]);
        r[3] = res;
    }

#undef MUL_M4_V4
}