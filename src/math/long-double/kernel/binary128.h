#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_BINARY128_H
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_BINARY128_H

#include "../../../soft/integer/clzti2.h"
#include <float.h>
#include <stdint.h>

typedef unsigned __int128 u128;

#define U128(hi, lo) (((u128)(uint64_t)(hi) << 64) | (u128)(uint64_t)(lo))

#define F128_EXP_SHIFT 112
#define F128_BIAS 0x3fff
#define F128_SIGN_MASK ((u128)1 << 127)
#define F128_EXP_MASK ((u128)0x7fff << F128_EXP_SHIFT)
#define F128_MANTISSA_MASK (((u128)1 << F128_EXP_SHIFT) - 1)
#define F128_IMPLICIT_BIT ((u128)1 << F128_EXP_SHIFT)
#define F128_QUIET_BIT ((u128)1 << (F128_EXP_SHIFT - 1))

_Static_assert(sizeof(long double) == 16, "long double must be IEEE binary128");
_Static_assert(LDBL_MANT_DIG == 113, "long double must be IEEE binary128");

static inline u128 f128_bits_(long double x)
{
    union {
        long double value;
        u128 bits;
    } value = { .value = x };

    return value.bits;
}

static inline long double f128_from_bits_(u128 bits)
{
    union {
        long double value;
        u128 bits;
    } value = { .bits = bits };

    return value.value;
}

/* Split a nonzero finite magnitude into a 113-bit mantissa and unbiased
 * exponent.  Subnormals receive a virtual exponent below -16382. */
static inline void f128_split_(u128 bits, u128 *mantissa, int *exponent)
{
    int biased = bits >> F128_EXP_SHIFT;

    if (biased) {
        *mantissa = (bits & F128_MANTISSA_MASK) | F128_IMPLICIT_BIT;
        *exponent = biased - F128_BIAS;
        return;
    }

    unsigned shift = (unsigned)clzti2_(bits) - (127 - F128_EXP_SHIFT);
    *mantissa = bits << shift;
    *exponent = 1 - F128_BIAS - (int)shift;
}

#endif
