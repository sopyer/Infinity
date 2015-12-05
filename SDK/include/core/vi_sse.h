// x86/x64

#define VI_INLINE __forceinline

template<int c0, int c1, int c2, int c3>
VI_INLINE v128 vi_swizzle(v128 a)
{
    static_assert(c0 >= 0, "c0 template parameter out of range");
    static_assert(c1 >= 0, "c1 template parameter out of range");
    static_assert(c2 >= 0, "c2 template parameter out of range");
    static_assert(c3 >= 0, "c3 template parameter out of range");
    static_assert(c0 <= 3, "c0 template parameter out of range");
    static_assert(c1 <= 3, "c1 template parameter out of range");
    static_assert(c2 <= 3, "c2 template parameter out of range");
    static_assert(c3 <= 3, "c3 template parameter out of range");

    return _mm_shuffle_ps(a, a, _MM_SHUFFLE(c3, c2, c1, c0));
}

template<int c0, int c1, int c2, int c3>
VI_INLINE v128 vi_shuffle(v128 a, v128 b)
{
    static_assert(c0 >= 0, "c0 template parameter out of range");
    static_assert(c1 >= 0, "c1 template parameter out of range");
    static_assert(c2 >= 0, "c2 template parameter out of range");
    static_assert(c3 >= 0, "c3 template parameter out of range");
    static_assert(c0 <= 7, "c0 template parameter out of range");
    static_assert(c1 <= 7, "c1 template parameter out of range");
    static_assert(c2 <= 7, "c2 template parameter out of range");
    static_assert(c3 <= 7, "c3 template parameter out of range");
    
    enum {
        swizzle_mask = _MM_SHUFFLE(c3&3, c2&3, c1&3, c0&3),
        select_mask = ((c3&4)<<1) | (c2&4) | ((c1&4)>>1) | ((c0&4)>>2),
    };
    
    v128 t1 = _mm_shuffle_ps(a, a, swizzle_mask);
    v128 t2 = _mm_shuffle_ps(b, b, swizzle_mask);
    return _mm_blend_ps(t1, t2, select_mask);
}

template<unsigned int mask>
VI_INLINE v128 vi_select(v128 a, v128 b)
{
    return _mm_blend_ps(a, b, mask);
}

#if 1
VI_INLINE v128 vi_select(v128 a, v128 b, v128 mask)
{
    return _mm_blendv_ps(a, b, mask);
}
#else
// http://realtimecollisiondetection.net/blog/?p=90
VI_INLINE v128 vi_select(v128 a, v128 b, v128 mask)
{
    return _mm_xor_ps(_mm_and_ps(_mm_xor_ps(a, b), mask), a);
}
#endif

VI_INLINE v128 vi_set(float x, float y, float z, float w)
{
    return _mm_set_ps(w, z, y, x);
}

VI_INLINE v128 vi_set_x(float x)
{
    return _mm_set_ss(x);
}

VI_INLINE v128 vi_set_all(float x)
{
    return _mm_set_ps1(x);
}

VI_INLINE v128 vi_seti_x(int x)
{
    return _mm_castsi128_ps(_mm_cvtsi32_si128(x));
}

VI_INLINE v128 vi_seti_all(int x)
{
    return _mm_castsi128_ps(_mm_set1_epi32(x));
}

VI_INLINE v128 vi_set_zero()
{
    return _mm_setzero_ps();
}

VI_INLINE float vi_get_x(v128 v)
{
    return _mm_cvtss_f32(v);
}

VI_INLINE float vi_get_y(v128 v)
{
    return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
}

VI_INLINE float vi_get_z(v128 v)
{
    return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
}

VI_INLINE float vi_get_w(v128 v)
{
    return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
}

VI_INLINE v128 vi_load_x(const void* m32)
{
    return _mm_load_ss((const float*)m32);
}

VI_INLINE v128 vi_load_v2(const void* m64)
{
    return _mm_castpd_ps(_mm_load_sd((const double*)m64));
}

VI_INLINE v128 vi_load_v3(const void* m96)
{
    v128 r0, r1;

    r0 = _mm_castpd_ps(_mm_load_sd((const double*)m96));
    r1 = _mm_load_ss((const float*)m96 + 2);
    return _mm_movelh_ps(r0, r1);
}

VI_INLINE v128 vi_load_v4(const void* m128)
{
    return _mm_load_ps((const float*)m128);
}

VI_INLINE v128 vi_loadu_v4(const void* m128)
{
    return _mm_loadu_ps((const float*)m128);
}

VI_INLINE void vi_store_x(void* m32, v128 v)
{
    _mm_store_ss((float*)m32, v);
}

VI_INLINE void vi_store_v2(void* m64,  v128 v)
{
    _mm_store_sd((double*)m64, _mm_castps_pd(v));
}

VI_INLINE void vi_store_v3(void* m96,  v128 v)
{
    _mm_store_sd((double*)m96, _mm_castps_pd(v));
    _mm_store_ss((float*)m96 + 2, _mm_movehl_ps(v, v));
}

VI_INLINE void vi_store_v4(void* m128, v128 v)
{
    _mm_store_ps((float*)m128, v);
}

VI_INLINE void vi_storeu_v4(void* m128, v128 v)
{
    _mm_storeu_ps((float*)m128, v);
}

VI_INLINE float vi_dot2(v128 a, v128 b)
{
    v128 res   = _mm_mul_ps(a, b);
    v128 shuff = _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 2, 1, 1));
    return _mm_cvtss_f32(_mm_add_ps(res, shuff));
}

VI_INLINE float vi_dot3(v128 a, v128 b)
{
    v128 res = _mm_mul_ps(a, b);
    v128 shuffle0 = _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 2, 1, 1));
    v128 shuffle1 = _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 2, 1, 2));
    res = _mm_add_ps(res, shuffle0);
    res = _mm_add_ps(res, shuffle1);
    return _mm_cvtss_f32(res);
}

#if 1
VI_INLINE float vi_dot4(v128 a, v128 b)
{
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0xFF));
}
#else
VI_INLINE float vi_dot4(v128 a, v128 b)
{
    v128 res, shuffle;
    res = _mm_mul_ps(a, b);
    shuffle = _mm_shuffle_ps(res, res, _MM_SHUFFLE(2, 3, 0, 1));
    res = _mm_add_ps(res, shuffle);
    shuffle = _mm_shuffle_ps(res, res, _MM_SHUFFLE(2, 3, 0, 1));
    res = _mm_add_ps(res, shuffle)
    return _mm_cvtss_f32(res);
}
#endif

VI_INLINE v128 vi_cross3(v128 a, v128 b)
{
    v128 yzx1, zxy1;
    v128 yzx2, zxy2;
    v128 r;

    yzx1 = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
    zxy1 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
    r = _mm_mul_ps(yzx1, zxy1);
    zxy2 = _mm_shuffle_ps(yzx1, yzx1, _MM_SHUFFLE(3, 0, 2, 1));
    yzx2 = _mm_shuffle_ps(zxy1, zxy1, _MM_SHUFFLE(3, 1, 0, 2));

    return _mm_sub_ps(r, _mm_mul_ps(yzx2, zxy2));
}

VI_INLINE v128 vi_mad(v128 a, v128 b, v128 c)
{
    return _mm_add_ps(_mm_mul_ps(a, b), c);
}

VI_INLINE v128 vi_mul(v128 a, v128 b)
{
    return _mm_mul_ps(a, b);
}

VI_INLINE v128 vi_div(v128 a, v128 b)
{
    return _mm_div_ps(a, b);
}

VI_INLINE v128 vi_add(v128 a, v128 b)
{
    return _mm_add_ps(a, b);
}

VI_INLINE v128 vi_sub(v128 a, v128 b)
{
    return _mm_sub_ps(a, b);
}

VI_INLINE v128 vi_neg(v128 a)
{
    return _mm_xor_ps(a, vi_seti_all(VI_SIGN_MASK));
}

VI_INLINE v128 vi_abs(v128 a)
{
    return _mm_and_ps(a, vi_seti_all(0x7FFFFFFF));
}

VI_INLINE v128 vi_sign(v128 a)
{
    v128 mask = _mm_cmpneq_ps(a, _mm_setzero_ps());
    v128 sign = _mm_and_ps(a, vi_seti_all(VI_SIGN_MASK));
    v128 one  = _mm_and_ps(mask, vi_seti_all(VI_ONE_ASINT));
    return _mm_or_ps(one, sign);
}

VI_INLINE v128 vi_max(v128 a, v128 b)
{
    return _mm_max_ps(a, b);
}

VI_INLINE v128 vi_min(v128 a, v128 b)
{
    return _mm_min_ps(a, b);
}

VI_INLINE v128 vi_lerp(v128 a, v128 b, float t)
{
    v128 r0, r1;

    r0 = _mm_sub_ps(b, a);
    r1 = vi_set_all(t);

    return vi_mad(r1, r0, a);
}

VI_INLINE v128 vi_clamp(v128 value, v128 lower, v128 upper)
{
    return _mm_max_ps(_mm_min_ps(value, upper), lower);
}

VI_INLINE v128 vi_sat(v128 value)
{
    return _mm_max_ps(_mm_min_ps(value, vi_seti_all(VI_ONE_ASINT)), _mm_setzero_ps());
}

VI_INLINE v128 vi_rcp(v128 a)
{
    return _mm_rcp_ps(a);
}

VI_INLINE v128 vi_rsqrt(v128 a)
{
    return _mm_rsqrt_ps(a);
}

VI_INLINE v128 vi_sqrt(v128 a)
{
    return _mm_sqrt_ps(a);
}

VI_INLINE v128 vi_cmp_gt(v128 a, v128 b)
{
    return _mm_cmpgt_ps(a, b);
}

VI_INLINE v128 vi_cmp_ge(v128 a, v128 b)
{
    return _mm_cmpge_ps(a, b);
}

VI_INLINE v128 vi_cmp_lt(v128 a, v128 b)
{
    return _mm_cmplt_ps(a, b);
}

VI_INLINE v128 vi_cmp_le(v128 a, v128 b)
{
    return _mm_cmple_ps(a, b);
}

VI_INLINE v128 vi_cmp_eq(v128 a, v128 b)
{
    return _mm_cmpeq_ps(a, b);
}

VI_INLINE v128 vi_cmp_ne(v128 a, v128 b)
{
    return _mm_cmpneq_ps(a, b);
}

VI_INLINE int vi_cmpx_gt(v128 a, v128 b)
{
    return _mm_comigt_ss(a, b);
}

VI_INLINE int vi_cmpx_ge(v128 a, v128 b)
{
    return _mm_comige_ss(a, b);
}

VI_INLINE int vi_cmpx_lt(v128 a, v128 b)
{
    return _mm_comilt_ss(a, b);
}

VI_INLINE int vi_cmpx_le(v128 a, v128 b)
{
    return _mm_comile_ss(a, b);
}

VI_INLINE int vi_cmpx_eq(v128 a, v128 b)
{
    return _mm_comieq_ss(a, b);
}

VI_INLINE int vi_cmpx_ne(v128 a, v128 b)
{
    return _mm_comineq_ss(a, b);
}

VI_INLINE v128 vi_xor(v128 a, v128 b)
{
    return _mm_xor_ps(a, b);
}

VI_INLINE v128 vi_or(v128 a, v128 b)
{
    return _mm_or_ps(a, b);
}

VI_INLINE v128 vi_and(v128 a, v128 b)
{
    return _mm_and_ps(a, b);
}

VI_INLINE v128 vi_andnot(v128 a, v128 b)
{
    return _mm_andnot_ps(b, a);
}

VI_INLINE bool vi_all(v128 a)
{
    return _mm_movemask_ps(a) == 0x0F;
}

VI_INLINE bool vi_any(v128 a)
{
    return _mm_movemask_ps(a) != 0x00;
}

VI_INLINE v128 vi_cvt_u8x4_to_v128(uint32_t ub4)
{
    __m128i tmp, zero;

    tmp  = _mm_cvtsi32_si128(ub4);
    zero = _mm_setzero_si128();
    tmp  = _mm_unpacklo_epi8(tmp, zero);
    tmp  = _mm_unpacklo_epi16(tmp, zero);

    v128 res = _mm_cvtepi32_ps(tmp);

    res = _mm_div_ps(res, _mm_set_ps1(255.0f));

    return res;
}

//VI_INLINE v128 vi_cvt_v128_to_u8x4(__int32 ub4)
//{
//}
