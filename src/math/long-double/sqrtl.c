#include "kernel/binary128.h"
#include "kernel/cbrt_tables.h"
#include "kernel/roots.h"
#include "kernel/uint.h"
#include <stdint.h>

/* Correctly rounded binary128 roots imported from metallic-rs commit
 * 76a9d7f0bce0e2a1dd271b106801325beb866dc3. */

#define ROOT_GATE_ 32

static u128 abs_diff_(u128 a, u128 b)
{
    return a < b ? b - a : a - b;
}

static int div_floor_(int value, int divisor)
{
    int quotient = value / divisor;
    return quotient - (value % divisor < 0);
}

static int mod_floor_(int value, int divisor)
{
    int remainder = value % divisor;
    return remainder < 0 ? remainder + divisor : remainder;
}

static u128 sqrt_fixed_(u128 mantissa, int octave)
{
    return sqrt_wide_(mantissa << (14 + octave));
}

static u128 rsqrt_fixed_(u128 mantissa, int octave)
{
    uint64_t r = rsqrt64_(mantissa, octave);
    u128 z = mantissa << (14 + octave);
    u128 hp = ((u128)1 << 124) - mhi_approx_((u128)r * r, z);
    u128 linear = (u128)r * (hp >> 64) + ((u128)r * (uint64_t)hp >> 64);
    u128 hs = (uint64_t)(hp >> 28);
    uint64_t q3 = (uint64_t)((hs * hs) >> 68) * 3;
    u128 quadratic = ((u128)q3 * r) >> 66;
    return ((u128)r << 61) + linear + quadratic;
}

static u128 cbrt_fixed_(u128 mantissa, int remainder)
{
    uint64_t m64 = mantissa >> 49;
    unsigned i = (m64 >> 57) & 63;
    int64_t d = (int64_t)(m64 & (((uint64_t)1 << 57) - 1))
        - ((int64_t)1 << 56);
    int64_t slope = ((__int128)d * (__int128)CBRT_SEED_SLOPE_[i]) >> 65;
    uint64_t dd = ((u128)((__int128)d * (__int128)d)) >> 63;
    int64_t curve = (u128)dd * CBRT_SEED_CURVE_[i] >> 66;
    uint64_t r0 = CBRT_SEED_VALUE_[i] - (uint64_t)slope + (uint64_t)curve;

    uint64_t r2 = mul_hi_64_(r0, r0);
    uint64_t r3 = mul_hi_64_(r2, r0);
    uint64_t t = ((uint64_t)1 << 62) - mul_hi_64_(m64, r3);
    uint64_t u = mul_hi_64_(r0, t);
    uint64_t r = (uint64_t)(((u128)u * CBRT_NEWTON_[remainder]) >> 61) - 64;

    u128 z = mantissa << (13 + remainder);
    u128 sx = mhi_approx_((u128)r * r, z);
    u128 hp = ((u128)1 << 122)
        - ((u128)r * (sx >> 64) + ((u128)r * (uint64_t)sx >> 64));
    const u128 two_thirds = 2 * (~(u128)0 / 3) + 1;
    /* The quadratic term is ~2^17 units, so 64 bits of 5/9 leave its
     * truncation far below one unit. */
    const uint64_t five_ninths = UINT64_C(0x8e38e38e38e38e38);
    u128 s23 = mhi_approx_(sx, two_thirds);
    u128 high;
    u128 low;
    wmul_(hp, s23, &high, &low);
    u128 linear = (high << 6) | (low >> 122);
    u128 hs = (uint64_t)(hp >> 26);
    uint64_t q59 = mul_hi_64_((uint64_t)((hs * hs) >> 69), five_ninths);
    u128 quadratic = ((u128)q59 * (uint64_t)(sx >> 61)) >> 62;
    return sx + linear + quadratic;
}

static u384_t mul3_(u128 a, u128 b, u128 c)
{
    u128 ab_high;
    u128 ab_low;
    u128 carry;
    u128 low;
    u128 high;
    u128 middle;
    wmul_(a, b, &ab_high, &ab_low);
    wmul_(ab_low, c, &carry, &low);
    wmul_(ab_high, c, &high, &middle);
    u128 sum = middle + carry;
    high += sum < middle;
    return U384(low, sum, high);
}

/* Gate hit: the candidate is within ROOT_GATE_ of its block midpoint
 * M = (2m + 1) / 2 ulp, and the fast path is within ROOT_GATE_ of the exact
 * root, so the result is one of the two neighbors of M.  The sign of an
 * exact integer comparison of the input against M squared (or cubed) picks
 * it; ties are impossible because 2m + 1 is odd while the input side always
 * carries a nonzero power of two. */
static __attribute__((cold, noinline)) u128 correct_sqrt_(u128 mantissa,
    int octave, u128 candidate)
{
    u128 m = candidate >> 13;
    u128 modd = 2 * m + 1;
    u128 high;
    u128 low;
    wmul_(modd, modd, &high, &low);
    u128 in_high = mantissa >> (14 - octave);
    u128 in_low = mantissa << (114 + octave);
    return m + (in_high > high || (in_high == high && in_low > low));
}

static __attribute__((cold, noinline)) u128 correct_rsqrt_(u128 mantissa,
    int octave, u128 candidate)
{
    u128 m = candidate >> 11;
    u128 modd = 2 * m + 1;
    int side = u384_cmp_(mul3_(mantissa, modd, modd),
        U384(0, 0, (u128)1 << (84 - octave)));
    return m + (side < 0);
}

static __attribute__((cold, noinline)) u128 correct_cbrt_(u128 mantissa,
    int remainder, u128 candidate)
{
    u128 m = candidate >> 11;
    u128 modd = 2 * m + 1;
    int side = u384_cmp_(u384_shl_(U384(mantissa, 0, 0), 227 + remainder),
        mul3_(modd, modd, modd));
    return m + (side > 0);
}

long double sqrtl(long double x)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude > F128_EXP_MASK)
        return f128_from_bits_(bits | F128_QUIET_BIT);
    if (!magnitude)
        return x;
    if (bits & F128_SIGN_MASK)
        return f128_from_bits_(F128_EXP_MASK | F128_QUIET_BIT);
    if (magnitude == F128_EXP_MASK)
        return x;

    u128 mantissa;
    int exponent;
    f128_split_(magnitude, &mantissa, &exponent);
    int octave = mod_floor_(exponent, 2);
    int scale = div_floor_(exponent, 2);
    u128 candidate = sqrt_fixed_(mantissa, octave);
    u128 m = abs_diff_(candidate & 8191, 4096) <= ROOT_GATE_
        ? correct_sqrt_(mantissa, octave, candidate)
        : (candidate + 4096) >> 13;
    return f128_from_bits_(((u128)(F128_BIAS - 1 + scale) << F128_EXP_SHIFT)
        + m);
}

long double rsqrtl(long double x)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (magnitude > F128_EXP_MASK)
        return f128_from_bits_(bits | F128_QUIET_BIT);
    if (!magnitude)
        return f128_from_bits_((bits & F128_SIGN_MASK) | F128_EXP_MASK);
    if (bits & F128_SIGN_MASK)
        return f128_from_bits_(F128_EXP_MASK | F128_QUIET_BIT);
    if (magnitude == F128_EXP_MASK)
        return f128_from_bits_(0);

    u128 mantissa;
    int exponent;
    f128_split_(magnitude, &mantissa, &exponent);
    int octave = mod_floor_(exponent, 2);
    int scale = -div_floor_(exponent, 2);
    u128 candidate = rsqrt_fixed_(mantissa, octave);
    u128 m = abs_diff_(candidate & 2047, 1024) <= ROOT_GATE_
        ? correct_rsqrt_(mantissa, octave, candidate)
        : (candidate + 1024) >> 11;
    return f128_from_bits_(((u128)(F128_BIAS - 2 + scale) << F128_EXP_SHIFT)
        + m);
}

long double cbrtl(long double x)
{
    u128 bits = f128_bits_(x);
    u128 magnitude = bits & ~F128_SIGN_MASK;
    if (!magnitude || magnitude >= F128_EXP_MASK)
        return magnitude > F128_EXP_MASK
            ? f128_from_bits_(bits | F128_QUIET_BIT) : x;

    u128 mantissa;
    int exponent;
    f128_split_(magnitude, &mantissa, &exponent);
    int remainder = mod_floor_(exponent, 3);
    int scale = div_floor_(exponent, 3);
    u128 candidate = cbrt_fixed_(mantissa, remainder);
    u128 m = abs_diff_(candidate & 2047, 1024) <= ROOT_GATE_
        ? correct_cbrt_(mantissa, remainder, candidate)
        : (candidate + 1024) >> 11;
    u128 scaled = ((u128)(F128_BIAS - 1) << F128_EXP_SHIFT) + m;
    u128 step = (u128)(scale < 0 ? -scale : scale) << F128_EXP_SHIFT;
    scaled = scale < 0 ? scaled - step : scaled + step;
    return f128_from_bits_((bits & F128_SIGN_MASK) | scaled);
}
