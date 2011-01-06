#pragma once

#include <xmmintrin.h>
#include <tmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

typedef __m128	vec4;
typedef __m128i	vec4i;
typedef __m128d	vec4d;

extern __m128 _vi_c_one;
extern __m128 _vi_c_zero;

#define VI_X	0x00
#define VI_Y	0x01
#define VI_Z	0x02
#define VI_W	0x03

#define VI_A_X	0x00
#define VI_A_Y	0x01
#define VI_A_Z	0x02
#define VI_A_W	0x03

#ifdef USE_SSE4
#	define VI_B_X	0x10
#	define VI_B_Y	0x11
#	define VI_B_Z	0x12
#	define VI_B_W	0x13
#	define VI_COMPONENT(c, p) (((c&0x10)<<(p+4))|((c&0x3)<<(p*2)))
#else
#	define VI_B_X	0x20
#	define VI_B_Y	0x21
#	define VI_B_Z	0x22
#	define VI_B_W	0x23
#	define VI_COMPONENT(c, p) (((c&0x30)<<(2*p+4))|((c&0x3)<<(p*2)))
#endif

#define VI_SHUFFLE_MASK(xmask, ymask, zmask, wmask) (VI_COMPONENT(wmask, 3)|VI_COMPONENT(zmask, 2)|VI_COMPONENT(ymask, 1)|VI_COMPONENT(xmask, 0))
#define VI_SWIZZLE_MASK(x, y, z, w) _MM_SHUFFLE(w, z, y, x)
#define VI_SWIZZLE_XXXX VI_SWIZZLE_MASK(0, 0, 0, 0)
#define VI_SWIZZLE_YYYY VI_SWIZZLE_MASK(1, 1, 1, 1)
#define VI_SWIZZLE_ZZZZ VI_SWIZZLE_MASK(2, 2, 2, 2)
#define VI_SWIZZLE_WWWW VI_SWIZZLE_MASK(3, 3, 3, 3)

//Component rearrange functions
template<unsigned int mask>
inline vec4 vi_swizzle(vec4 a);
//select components from 2 source vectors
template<unsigned int mask>
inline vec4 vi_shuffle(vec4 a, vec4 b);
inline vec4 vi_select(vec4 mask, vec4 a, vec4 b);

//load functions
inline vec4 vi_set(float x, float y, float z, float w);
inline vec4 vi_set_all(float x);
inline vec4 vi_load(float* v4);
inline vec4 vi_loadu(float* v4);
inline vec4 vi_load_zero();
inline vec4 vi_load_one();

//math functions
inline vec4 vi_dot4(vec4 a, vec4 b);
inline vec4 vi_dot3(vec4 a, vec4 b);
inline vec4 vi_mad(vec4 a, vec4 b, vec4 c);
inline vec4 vi_mul(vec4 a, vec4 b);
inline vec4 vi_add(vec4 a, vec4 b);
inline vec4 vi_sub(vec4 a, vec4 b);
inline vec4 vi_rcp(vec4 a);
inline vec4 vi_rsqrt(vec4 a);
inline vec4 vi_sqrt(vec4 a);
inline vec4 vi_neg(vec4 a);
inline vec4 vi_abs(vec4 a);
inline vec4 vi_sign(vec4 a);
inline vec4 vi_max(vec4 a, vec4 b);
inline vec4 vi_min(vec4 a, vec4 b);
inline vec4 vi_clamp(vec4 value, vec4 lower, vec4 upper);
inline vec4 vi_sat(vec4 value);

//compare functions
inline vec4 vi_cmp_gt(vec4 a, vec4 b);
inline vec4 vi_cmp_ge(vec4 a, vec4 b);
inline vec4 vi_cmp_lt(vec4 a, vec4 b);
inline vec4 vi_cmp_le(vec4 a, vec4 b);
inline vec4 vi_cmp_eq(vec4 a, vec4 b);
inline vec4 vi_cmp_ne(vec4 a, vec4 b);

//logical op functions
inline vec4 vi_xor(vec4 a, vec4 b);
inline vec4 vi_or(vec4 a, vec4 b);
inline vec4 vi_and(vec4 a, vec4 b);
inline vec4 vi_andnot(vec4 a, vec4 b);

//logical result functions
inline bool vi_all(vec4 a);
inline bool vi_any(vec4 a);

#include "vi.inl"