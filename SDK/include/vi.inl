template<unsigned int mask>
__forceinline v128 vi_swizzle(v128 a)
{
    return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(a), mask));
}

template<unsigned int mask>
__forceinline v128 vi_shuffle(v128 a, v128 b)
{
    v128 t1 = vi_swizzle<mask&0xFF>(a);
    v128 t2 = vi_swizzle<mask&0xFF>(b);
    return _mm_blend_ps(t1, t2, (mask>>8)&0xF);
}

template<unsigned int mask>
__forceinline v128 vi_select(v128 a, v128 b)
{
    return _mm_blend_ps(a, b, mask);
}

#if 1
__forceinline v128 vi_select(v128 a, v128 b, v128 mask)
{
    return _mm_blendv_ps(a, b, mask);
}
#else
// http://realtimecollisiondetection.net/blog/?p=90
__forceinline v128 vi_select(v128 a, v128 bá v128 mask)
{
    return vi_xor(vi_and(vi_xor(a, b), mask), a);
}
#endif

__forceinline v128 vi_set(float x, float y, float z, float w)
{
    return _mm_set_ps(w, z, y, x);
}

__forceinline v128 vi_set_f000(float x)
{
    return _mm_set_ss(x);
}

inline v128 vi_set_fff0 (float x)
{
    return vi_swizzle<VI_SWIZZLE_MASK(0, 0, 0, 3)>(vi_set_f000(x));
}

__forceinline v128 vi_set_ffff(float x)
{
    return _mm_set_ps1(x);
}

__forceinline v128 vi_set_iiii(int x)
{
    return _mm_castsi128_ps(_mm_set1_epi32(x));
}

__forceinline v128 vi_set_i000(int x)
{
    return _mm_castsi128_ps(_mm_cvtsi32_si128(x));
}

__forceinline v128 vi_set_0000()
{
    return _mm_setzero_ps();
}

__forceinline v128 vi_set_1000f()
{
    return vi_set_i000(FLT_1_0_ASINT);
}

__forceinline v128 vi_set_0100f()
{
    return vi_swizzle<VI_SWIZZLE_MASK(1, 0, 1, 1)>(vi_set_i000(FLT_1_0_ASINT));
}

__forceinline v128 vi_set_0010f()
{
    return vi_swizzle<VI_SWIZZLE_MASK(1, 1, 0, 1)>(vi_set_i000(FLT_1_0_ASINT));
}

__forceinline v128 vi_set_0001f()
{
    return vi_swizzle<VI_SWIZZLE_MASK(1, 1, 1, 0)>(vi_set_i000(FLT_1_0_ASINT));
}

__forceinline float vi_get_x(v128 v)
{
    return _mm_cvtss_f32(v);
}

__forceinline float vi_get_y(v128 v)
{
    return _mm_cvtss_f32(vi_swizzle<VI_SWIZZLE_YYYY>(v));
}

__forceinline float vi_get_z(v128 v)
{
    return _mm_cvtss_f32(vi_swizzle<VI_SWIZZLE_ZZZZ>(v));
}

__forceinline float vi_get_w(v128 v)
{
    return _mm_cvtss_f32(vi_swizzle<VI_SWIZZLE_WWWW>(v));
}

__forceinline v128 vi_load_x(const void* m32)
{
    return _mm_load_ss((const float*)m32);
}

__forceinline v128 vi_load_v2(const void* m64)
{
    return _mm_castpd_ps(_mm_load_sd((const double*)m64));
}

__forceinline v128 vi_load_v3(const void* m96)
{
    v128 r0, r1;

    r0 = _mm_castpd_ps(_mm_load_sd((const double*)m96));
    r1 = _mm_load_ss((const float*)m96 + 2);
    return _mm_movelh_ps(r0, r1);
}

__forceinline v128 vi_load_v4(const void* m128)
{
    return _mm_load_ps((const float*)m128);
}

__forceinline v128 vi_loadu_v4(const void* m128)
{
    return _mm_loadu_ps((const float*)m128);
}

__forceinline void vi_store_x(void* m32, v128 v)
{
    _mm_store_ss((float*)m32, v);
}

__forceinline void vi_store_v2(void* m64,  v128 v)
{
    _mm_store_sd((double*)m64, _mm_castps_pd(v));
}

__forceinline void vi_store_v3(void* m96,  v128 v)
{
    _mm_store_sd((double*)m96, _mm_castps_pd(v));
    _mm_store_ss((float*)m96 + 2, _mm_movehl_ps(v, v));
}

__forceinline void vi_store_v4(void* m128, v128 v)
{
    _mm_store_ps((float*)m128, v);
}

__forceinline void vi_storeu_v4(void* m128, v128 v)
{
    _mm_storeu_ps((float*)m128, v);
}

__forceinline v128 vi_dot2(v128 a, v128 b)
{
    v128 res   = _mm_mul_ps(a, b);
    v128 shuff = vi_swizzle<VI_SWIZZLE_MASK(VI_Y, VI_X, VI_W, VI_Z)>(res);
    return _mm_add_ps(res, shuff);
}

__forceinline v128 vi_dot3(v128 a, v128 b)
{
    v128 res = _mm_mul_ps(a, b);
    v128 shuff = vi_swizzle<VI_SWIZZLE_MASK(VI_Y, VI_X, VI_Z, VI_Z)>(res);
    v128 res2 = _mm_add_ps(res, shuff);
    shuff = vi_swizzle<VI_SWIZZLE_MASK(VI_Z, VI_Z, VI_Z, VI_Z)>(res);
    res = _mm_add_ps(res2, shuff);
    return vi_swizzle<VI_SWIZZLE_XXXX>(res);
}

#if 1
__forceinline v128 vi_dot4(v128 a, v128 b)
{
    return _mm_dp_ps(a, b, 0xFF);
}
#else
__forceinline v128 vi_dot4(v128 a, v128 b)
{
    v128 res   = _mm_mul_ps(a, b);
    v128 shuff = vi_swizzle<VI_SWIZZLE_MASK(VI_Y, VI_X, VI_W, VI_Z)>(res);
    res = _mm_add_ps(res, shuff);
    shuff = vi_swizzle<VI_SWIZZLE_MASK(VI_Z, VI_W, VI_X, VI_Y)>(res);
    return _mm_add_ps(res, shuff);
}
#endif

__forceinline v128 vi_cross3(v128 a, v128 b)
{
    v128 yzx1, zxy1;
    v128 yzx2, zxy2;
    v128 r;

    yzx1 = vi_swizzle<VI_SWIZZLE_MASK(1, 2, 0, 3)>(a);
    zxy1 = vi_swizzle<VI_SWIZZLE_MASK(2, 0, 1, 3)>(b);

    r = vi_mul(yzx1, zxy1);

    zxy2 = vi_swizzle<VI_SWIZZLE_MASK(1, 2, 0, 3)>(yzx1);
    yzx2 = vi_swizzle<VI_SWIZZLE_MASK(2, 0, 1, 3)>(zxy1);

    return vi_sub(r, vi_mul(yzx2, zxy2));
}

__forceinline v128 vi_mad(v128 a, v128 b, v128 c)
{
    return _mm_add_ps(_mm_mul_ps(a, b), c);
}

__forceinline v128 vi_mul(v128 a, v128 b)
{
    return _mm_mul_ps(a, b);
}

__forceinline v128 vi_div(v128 a, v128 b)
{
    return _mm_div_ps(a, b);
}

__forceinline v128 vi_add(v128 a, v128 b)
{
    return _mm_add_ps(a, b);
}

__forceinline v128 vi_sub(v128 a, v128 b)
{
    return _mm_sub_ps(a, b);
}

__forceinline v128 vi_neg(v128 a)
{
    return _mm_xor_ps(a, vi_set_iiii(FLT_SIGN));
}

__forceinline v128 vi_abs(v128 a)
{
    return _mm_and_ps(a, vi_set_iiii(0x7FFFFFFF));
}

__forceinline v128 vi_sign(v128 a)
{
    v128 mask = vi_cmp_ne(a, vi_set_0000());
    v128 sign = vi_and(a, vi_set_iiii(FLT_SIGN));
    v128 one  = vi_and(mask, vi_set_iiii(FLT_1_0_ASINT));
    return vi_or(one, sign);
}

__forceinline v128 vi_max(v128 a, v128 b)
{
    return _mm_max_ps(a, b);
}

__forceinline v128 vi_min(v128 a, v128 b)
{
    return _mm_min_ps(a, b);
}

__forceinline v128 vi_lerp(v128 a, v128 b, float t)
{
    v128 r0, r1;

    r0 = vi_sub(b, a);
    r1 = vi_set_ffff(t);

    return vi_mad(r1, r0, a);
}

__forceinline v128 vi_clamp(v128 value, v128 lower, v128 upper)
{
    return vi_max(vi_min(value, upper), lower);
}

__forceinline v128 vi_sat(v128 value)
{
    return vi_clamp(value, vi_set_0000(), vi_set_iiii(FLT_1_0_ASINT));
}

__forceinline v128 vi_rcp(v128 a)
{
    return _mm_rcp_ps(a);
}

__forceinline v128 vi_rsqrt(v128 a)
{
    return _mm_rsqrt_ps(a);
}

__forceinline v128 vi_sqrt(v128 a)
{
    return _mm_sqrt_ps(a);
}

__forceinline v128 vi_cmp_gt(v128 a, v128 b)
{
    return _mm_cmpgt_ps(a, b);
}

__forceinline v128 vi_cmp_ge(v128 a, v128 b)
{
    return _mm_cmpge_ps(a, b);
}

__forceinline v128 vi_cmp_lt(v128 a, v128 b)
{
    return _mm_cmplt_ps(a, b);
}

__forceinline v128 vi_cmp_le(v128 a, v128 b)
{
    return _mm_cmple_ps(a, b);
}

__forceinline v128 vi_cmp_eq(v128 a, v128 b)
{
    return _mm_cmpeq_ps(a, b);
}

__forceinline v128 vi_cmp_ne(v128 a, v128 b)
{
    return _mm_cmpneq_ps(a, b);
}

__forceinline int vi_cmpx_gt(v128 a, v128 b)
{
    return _mm_comigt_ss(a, b);
}

__forceinline int vi_cmpx_ge(v128 a, v128 b)
{
    return _mm_comige_ss(a, b);
}

__forceinline int vi_cmpx_lt(v128 a, v128 b)
{
    return _mm_comilt_ss(a, b);
}

__forceinline int vi_cmpx_le(v128 a, v128 b)
{
    return _mm_comile_ss(a, b);
}

__forceinline int vi_cmpx_eq(v128 a, v128 b)
{
    return _mm_comieq_ss(a, b);
}

__forceinline int vi_cmpx_ne(v128 a, v128 b)
{
    return _mm_comineq_ss(a, b);
}

__forceinline v128 vi_xor(v128 a, v128 b)
{
    return _mm_xor_ps(a, b);
}

__forceinline v128 vi_or(v128 a, v128 b)
{
    return _mm_or_ps(a, b);
}

__forceinline v128 vi_and(v128 a, v128 b)
{
    return _mm_and_ps(a, b);
}

__forceinline v128 vi_andnot(v128 a, v128 b)
{
    return _mm_andnot_ps(b, a);
}

__forceinline bool vi_all(v128 a)
{
    return _mm_movemask_ps(a) == 0x0F;
}

__forceinline bool vi_any(v128 a)
{
    return _mm_movemask_ps(a) != 0x00;
}

__forceinline v128 vi_cvt_ubyte4_to_vec4(__int32 ub4)
{
    __m128i tmp, zero;

    //tmp.m128i_u32[0] = ub4;
    tmp  = _mm_cvtsi32_si128(ub4);
    zero = _mm_setzero_si128();
    tmp  = _mm_unpacklo_epi8(tmp, zero);
    tmp  = _mm_unpacklo_epi16(tmp, zero);

    v128 res = _mm_cvtepi32_ps(tmp);

    res = _mm_div_ps(res, _mm_set_ps1(255.0f));

    return res;
}

//__forceinline v128 vi_cvt_vec4_to_ubyte4(__int32 ub4)
//{
//}
