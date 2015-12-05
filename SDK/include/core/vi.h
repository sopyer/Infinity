#pragma once

#include <stdint.h>
#include <xmmintrin.h>
#include <tmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

typedef __m128  v128;

enum {
	VI_X = 0x00,
	VI_Y = 0x01,
	VI_Z = 0x02,
	VI_W = 0x03,

	VI_AX = 0x00,
	VI_AY = 0x01,
	VI_AZ = 0x02,
	VI_AW = 0x03,

	VI_BX = 0x04,
	VI_BY = 0x05,
	VI_BZ = 0x06,
	VI_BW = 0x07,

	VI_ONE_ASINT   = 0x3F800000, // 1.0f
	VI_ZERO_ASINT  = 0x00000000, // 0.0f
	VI_EPS7_AS_INT = 0x34000000, // 1.192092896e-7f
	VI_INF_AS_INT  = 0x7F800000, // INF
	VI_QNAN_AS_INT = 0x7FC00000, // QNAN
	VI_SIGN_MASK   = 0x80000000,
	VI_CMP_TRUE    = 0xFFFFFFFF,
	VI_CMP_FALSE   = 0x00000000,
};



// Component rearrange functions
template<int c0, int c1, int c2, int c3>
v128 vi_swizzle(v128 a);
// Rearrange components from 2 source vectors
template<int c0, int c1, int c2, int c3>
v128 vi_shuffle(v128 a, v128 b);
// Select components from 2 source vectors
v128 vi_select (v128 a, v128 b, v128 mask);



// Load/store functions
v128 vi_set     (float x, float y, float z, float w);
v128 vi_set_x   (float x);
v128 vi_set_all (float x);
v128 vi_seti_x  (int x);
v128 vi_seti_all(int x);
v128 vi_set_zero();

float vi_get_x(v128 v);

v128 vi_load_x   (const void* m32);
v128 vi_load_v2  (const void* m64);
v128 vi_load_v3  (const void* m96);
v128 vi_load_v4  (const void* m128);
v128 vi_loadu_v4 (const void* m128);

void vi_store_x  (void* m32,  v128 v);
void vi_store_v2 (void* m64,  v128 v);
void vi_store_v3 (void* m96,  v128 v);
void vi_store_v4 (void* m128, v128 v);
void vi_storeu_v4(void* m128, v128 v);



//math functions
float vi_dot4  (v128 a, v128 b);
float vi_dot3  (v128 a, v128 b);
float vi_dot2  (v128 a, v128 b);
v128  vi_cross3(v128 a, v128 b);
v128  vi_mad   (v128 a, v128 b, v128 c);
v128  vi_mul   (v128 a, v128 b);
v128  vi_div   (v128 a, v128 b);
v128  vi_add   (v128 a, v128 b);
v128  vi_sub   (v128 a, v128 b);
v128  vi_rcp   (v128 a);
v128  vi_rsqrt (v128 a);
v128  vi_sqrt  (v128 a);
v128  vi_neg   (v128 a);
v128  vi_abs   (v128 a);
v128  vi_sign  (v128 a);
v128  vi_max   (v128 a, v128 b);
v128  vi_min   (v128 a, v128 b);
v128  vi_lerp  (v128 a, v128 b, float t);
v128  vi_clamp (v128 value, v128 lower, v128 upper);
v128  vi_sat   (v128 value);



//vector compare functions
v128 vi_cmp_gt(v128 a, v128 b);
v128 vi_cmp_ge(v128 a, v128 b);
v128 vi_cmp_lt(v128 a, v128 b);
v128 vi_cmp_le(v128 a, v128 b);
v128 vi_cmp_eq(v128 a, v128 b);
v128 vi_cmp_ne(v128 a, v128 b);



//scalar compare functions
int vi_cmpx_gt(v128 a, v128 b);
int vi_cmpx_ge(v128 a, v128 b);
int vi_cmpx_lt(v128 a, v128 b);
int vi_cmpx_le(v128 a, v128 b);
int vi_cmpx_eq(v128 a, v128 b);
int vi_cmpx_ne(v128 a, v128 b);



//logical op functions
v128 vi_xor   (v128 a, v128 b);
v128 vi_or    (v128 a, v128 b);
v128 vi_and   (v128 a, v128 b);
v128 vi_andnot(v128 a, v128 b);



//logical result functions
bool vi_all(v128 a);
bool vi_any(v128 a);



//color operations
v128 vi_cvt_u8x4_to_v128(uint32_t ub4);
v128 vi_cvt_v128_to_u8x4(uint32_t ub4);

#include <core/vi_sse.h>
