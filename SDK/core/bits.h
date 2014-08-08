#pragma once

#include <stdint.h>
#include <assert.h>

/*
** Architecture-specific bit manipulation routines.
**
** Most modern processors provide instructions to count leading zeroes
** in a word, find the lowest and highest set bit, etc. These
** specific implementations will be used when available, falling back
** to a reasonably efficient generic implementation.
**
** bit_ffs/bit_fls returning value 0..31.
** ffs/fls return 1-32 by default, returning 0 for error.
*/

/*
** gcc 3.4 and above have builtin support, specialized for architecture.
** Some compilers masquerade as gcc; patchlevel test filters them out.
*/
#if defined (__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)) \
    && defined (__GNUC_PATCHLEVEL__)

inline int bit_ffs(unsigned int word)
{
    return __builtin_ffs(word) - 1;
}

inline int bit_fls(unsigned int word)
{
    const int bit = word ? 32 - __builtin_clz(word) : 0;
    return bit - 1;
}

#elif defined (_MSC_VER) && (_MSC_VER >= 1400) && (defined (_M_IX86) || defined (_M_X64))
/* Microsoft Visual C++ support on x86/X64 architectures. */

#include <intrin.h>

#pragma intrinsic(_BitScanReverse)
#pragma intrinsic(_BitScanForward)

inline int bit_fls(unsigned int word)
{
    unsigned long index;
    return _BitScanReverse(&index, word) ? index : -1;
}

inline int bit_ffs(unsigned int word)
{
    unsigned long index;
    return _BitScanForward(&index, word) ? index : -1;
}

#elif defined (_MSC_VER) && defined (_M_PPC)
/* Microsoft Visual C++ support on PowerPC architectures. */

#include <ppcintrinsics.h>

inline int bit_fls(unsigned int word)
{
    const int bit = 32 - _CountLeadingZeros(word);
    return bit - 1;
}

inline int bit_ffs(unsigned int word)
{
    const unsigned int reverse = word & (~word + 1);
    const int bit = 32 - _CountLeadingZeros(reverse);
    return bit - 1;
}

#elif defined (__ARMCC_VERSION)
/* RealView Compilation Tools for ARM */

inline int bit_ffs(unsigned int word)
{
    const unsigned int reverse = word & (~word + 1);
    const int bit = 32 - __clz(reverse);
    return bit - 1;
}

inline int bit_fls(unsigned int word)
{
    const int bit = word ? 32 - __clz(word) : 0;
    return bit - 1;
}

#elif defined (__ghs__)
/* Green Hills support for PowerPC */

#include <ppc_ghs.h>

inline int bit_ffs(unsigned int word)
{
    const unsigned int reverse = word & (~word + 1);
    const int bit = 32 - __CLZ32(reverse);
    return bit - 1;
}

inline int bit_fls(unsigned int word)
{
    const int bit = word ? 32 - __CLZ32(word) : 0;
    return bit - 1;
}

#else
/* Fall back to generic implementation. */

inline int bit_fls_generic(unsigned int word)
{
    int bit = 32;

    if (!word) bit -= 1;
    if (!(word & 0xffff0000)) { word <<= 16; bit -= 16; }
    if (!(word & 0xff000000)) { word <<= 8; bit -= 8; }
    if (!(word & 0xf0000000)) { word <<= 4; bit -= 4; }
    if (!(word & 0xc0000000)) { word <<= 2; bit -= 2; }
    if (!(word & 0x80000000)) { word <<= 1; bit -= 1; }

    return bit;
}

/* Implement ffs in terms of fls. */
inline int bit_ffs(unsigned int word)
{
    return bit_fls_generic(word & (~word + 1)) - 1;
}

inline int bit_fls(unsigned int word)
{
    return bit_fls_generic(word) - 1;
}

#endif

inline bool bit_is_pow2(uint32_t x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}

inline uint32_t bit_align_up(uint32_t size, uint32_t align)
{
    assert(bit_is_pow2(align));
    return (size + (align - 1)) & ~(align - 1);
}
