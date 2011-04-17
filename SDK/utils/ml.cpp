#include "ml.h"

namespace ml
{
	void transpose(vec4* transposed/*[4]*/, vec4* mat/*[4]*/)
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

#		define MUL_M4_V4(r, m, v)                        \
		{                                                \
			vec4 xxxx = vi_swizzle<VI_SWIZZLE_XXXX>(v);  \
			vec4 yyyy = vi_swizzle<VI_SWIZZLE_YYYY>(v);  \
			vec4 zzzz = vi_swizzle<VI_SWIZZLE_ZZZZ>(v);  \
			vec4 wwww = vi_swizzle<VI_SWIZZLE_WWWW>(v);  \
			r = vi_mul(xxxx, m[0]);                      \
			r = vi_mad(yyyy, m[1], r);                   \
			r = vi_mad(zzzz, m[2], r);                   \
			r = vi_mad(wwww, m[3], r);                   \
		}

		void mul(vec4* r, vec4* m, vec4 v)
		{
			vec4 res;
			MUL_M4_V4(res, m, v);
			*r = res;
		}

		void mul(vec4* r, vec4* a, vec4* b)
		{
			vec4 res;
			MUL_M4_V4(res, a, b[0]);
			r[0] = res;
			MUL_M4_V4(res, a, b[1]);
			r[1] = res;
			MUL_M4_V4(res, a, b[2]);
			r[2] = res;
			MUL_M4_V4(res, a, b[3]);
			r[3] = res;
		}

#undef MUL_M4_V4
}