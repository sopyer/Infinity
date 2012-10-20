#pragma once

#include <xmmintrin.h>
#include <tmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

typedef __m128	v128;

#define VI_EPS7 0.0000001f

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
inline v128 vi_swizzle(v128 a);
//select components from 2 source vectors
template<unsigned int mask>
inline v128 vi_shuffle(v128 a, v128 b);
inline v128 vi_select (v128 mask, v128 a, v128 b);

//load/store functions
inline v128 vi_set         (float x, float y, float z, float w);
inline v128 vi_set_xxxx    (float x);
inline v128 vi_set_xxxx_i32(int x);
inline v128 vi_set_x000_i32(int x);
inline v128 vi_load        (void* m128);
inline v128 vi_loadu       (void* m128);
inline v128 vi_load_zero   ();
inline v128 vi_load_one    ();
inline void vi_store       (void* m128, v128 val);
inline void vi_storeu      (void* m128, v128 val);
inline void vi_storex      (void* m32,  v128 val);

//math functions
inline v128 vi_dot4  (v128 a, v128 b);
inline v128 vi_dot3  (v128 a, v128 b);
inline v128 vi_cross3(v128 a, v128 b);
inline v128 vi_mad   (v128 a, v128 b, v128 c);
inline v128 vi_mul   (v128 a, v128 b);
inline v128 vi_div   (v128 a, v128 b);
inline v128 vi_add   (v128 a, v128 b);
inline v128 vi_sub   (v128 a, v128 b);
inline v128 vi_rcp   (v128 a);
inline v128 vi_rsqrt (v128 a);
inline v128 vi_sqrt  (v128 a);
inline v128 vi_neg   (v128 a);
inline v128 vi_abs   (v128 a);
inline v128 vi_sign  (v128 a);
inline v128 vi_max   (v128 a, v128 b);
inline v128 vi_min   (v128 a, v128 b);
inline v128 vi_lerp  (v128 a, v128 b, float t);
inline v128 vi_clamp (v128 value, v128 lower, v128 upper);
inline v128 vi_sat   (v128 value);

//vector compare functions
inline v128 vi_cmp_gt(v128 a, v128 b);
inline v128 vi_cmp_ge(v128 a, v128 b);
inline v128 vi_cmp_lt(v128 a, v128 b);
inline v128 vi_cmp_le(v128 a, v128 b);
inline v128 vi_cmp_eq(v128 a, v128 b);
inline v128 vi_cmp_ne(v128 a, v128 b);

//scalar compare functions
inline int vi_cmpx_gt(v128 a, v128 b);
inline int vi_cmpx_ge(v128 a, v128 b);
inline int vi_cmpx_lt(v128 a, v128 b);
inline int vi_cmpx_le(v128 a, v128 b);
inline int vi_cmpx_eq(v128 a, v128 b);
inline int vi_cmpx_ne(v128 a, v128 b);

//logical op functions
inline v128 vi_xor   (v128 a, v128 b);
inline v128 vi_or    (v128 a, v128 b);
inline v128 vi_and   (v128 a, v128 b);
inline v128 vi_andnot(v128 a, v128 b);

//logical result functions
inline bool vi_all(v128 a);
inline bool vi_any(v128 a);

//color operations
inline v128 vi_cvt_ubyte4_to_vec4(__int32 ub4);
inline v128 vi_cvt_vec4_to_ubyte4(__int32 ub4);

#include "vi.inl"