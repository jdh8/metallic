#include "kernel/binary128.h"
#include "kernel/cbrt_tables.h"
#include "kernel/roots.h"
#include "kernel/rsqrt_tables.h"
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

static uint64_t rsqrt64_(u128 mantissa, int octave)
{
    uint64_t m64 = mantissa >> 49;
    unsigned i = (m64 >> 57) & 63;
    int64_t d = (int64_t)(m64 & (((uint64_t)1 << 57) - 1))
        - ((int64_t)1 << 56);
    int64_t slope = ((__int128)d * (__int128)RSQRT_SEED_SLOPE_[i]) >> 65;
    uint64_t dd = ((u128)((__int128)d * (__int128)d)) >> 63;
    int64_t curve = (u128)dd * RSQRT_SEED_CURVE_[i] >> 65;
    uint64_t r0 = RSQRT_SEED_VALUE_[i] - (uint64_t)slope + (uint64_t)curve;

    uint64_t r2 = mul_hi_64_(r0, r0);
    uint64_t t = ((uint64_t)3 << 61) - mul_hi_64_(m64, r2);
    uint64_t u = mul_hi_64_(r0, t);
    return (uint64_t)(((u128)u * RSQRT_NEWTON_[octave]) >> 61) - 64;
}

u128 sqrt_wide_(u128 z)
{
    int octave = z >> 127;
    uint64_t r = rsqrt64_(z >> (14 + octave), octave);
    u128 hp = ((u128)1 << 124) - mhi_approx_((u128)r * r, z);
    u128 s0 = (u128)r * (z >> 64) + ((u128)r * (uint64_t)z >> 64);
    u128 linear = mhi_approx_(s0, hp << 3);
    u128 hs = (uint64_t)(hp >> 28);
    uint64_t s64 = s0 >> 62;
    u128 quadratic = (((hs * hs) >> 68) * 3 * s64) >> 65;
    return s0 + linear + quadratic;
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
    u128 quadratic = (((hs * hs) >> 68) * 3 * r) >> 66;
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
    const u128 five_ninths = ((u128)5 << 64) / 9;
    u128 s23 = mhi_approx_(sx, two_thirds);
    u128 high;
    u128 low;
    wmul_(hp, s23, &high, &low);
    u128 linear = (high << 6) | (low >> 122);
    u128 hs = (uint64_t)(hp >> 26);
    u128 quadratic = (((((hs * hs) >> 69) * five_ninths) >> 64)
        * (sx >> 60)) >> 63;
    return sx + linear + quadratic;
}

static void parts_(u128 magnitude, u128 *mantissa, int *exponent)
{
    *mantissa = (magnitude & F128_MANTISSA_MASK) | F128_IMPLICIT_BIT;
    *exponent = (int)(magnitude >> F128_EXP_SHIFT) - F128_BIAS;
}

static void midpoints_(u128 mantissa, u128 *lower, u128 *upper)
{
    u128 center = mantissa << 2;
    *lower = center - (mantissa == F128_IMPLICIT_BIT ? 1 : 2);
    *upper = center + 2;
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

static long double correct_sqrt_(u128 mantissa, int exponent, long double candidate)
{
    for (;;) {
        u128 bits = f128_bits_(candidate);
        u128 m;
        u128 lower;
        u128 upper;
        int e;
        parts_(bits, &m, &e);
        midpoints_(m, &lower, &upper);
        u384_t input = u384_shl_(U384(mantissa, 0, 0),
            116 + exponent - 2 * e);
        int odd = bits & 1;
        u128 high;
        u128 low;

        wmul_(lower, lower, &high, &low);
        int side = u384_cmp_(input, U384(low, high, 0));
        if (side < 0 || (side == 0 && odd)) {
            candidate = f128_from_bits_(bits - 1);
            continue;
        }

        wmul_(upper, upper, &high, &low);
        side = u384_cmp_(input, U384(low, high, 0));
        if (side > 0 || (side == 0 && odd)) {
            candidate = f128_from_bits_(bits + 1);
            continue;
        }
        return candidate;
    }
}

static long double correct_rsqrt_(u128 mantissa, int exponent,
    long double candidate)
{
    for (;;) {
        u128 bits = f128_bits_(candidate);
        u128 m;
        u128 lower;
        u128 upper;
        int e;
        parts_(bits, &m, &e);
        midpoints_(m, &lower, &upper);
        u384_t one = u384_shl_(U384(1, 0, 0), 340 - exponent - 2 * e);
        int odd = bits & 1;

        int side = u384_cmp_(mul3_(mantissa, lower, lower), one);
        if (side > 0 || (side == 0 && odd)) {
            candidate = f128_from_bits_(bits - 1);
            continue;
        }
        side = u384_cmp_(mul3_(mantissa, upper, upper), one);
        if (side < 0 || (side == 0 && odd)) {
            candidate = f128_from_bits_(bits + 1);
            continue;
        }
        return candidate;
    }
}

static long double correct_cbrt_(u128 mantissa, int remainder,
    long double candidate)
{
    for (;;) {
        u128 bits = f128_bits_(candidate);
        u128 m;
        u128 lower;
        u128 upper;
        int e;
        parts_(bits, &m, &e);
        midpoints_(m, &lower, &upper);
        u384_t input = u384_shl_(U384(mantissa, 0, 0),
            remainder - 3 * e + 230);
        int odd = bits & 1;

        int side = u384_cmp_(mul3_(lower, lower, lower), input);
        if (side > 0 || (side == 0 && odd)) {
            candidate = f128_from_bits_(bits - 1);
            continue;
        }
        side = u384_cmp_(mul3_(upper, upper, upper), input);
        if (side < 0 || (side == 0 && odd)) {
            candidate = f128_from_bits_(bits + 1);
            continue;
        }
        return candidate;
    }
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
    u128 rounded_bits = (u128)(F128_BIAS - 1 + scale) << F128_EXP_SHIFT;
    rounded_bits += (candidate + 4096) >> 13;
    long double rounded = f128_from_bits_(rounded_bits);
    return abs_diff_(candidate & 8191, 4096) <= ROOT_GATE_
        ? correct_sqrt_(mantissa, exponent, rounded) : rounded;
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
    u128 rounded_bits = (u128)(F128_BIAS - 2 + scale) << F128_EXP_SHIFT;
    rounded_bits += (candidate + 1024) >> 11;
    long double rounded = f128_from_bits_(rounded_bits);
    return abs_diff_(candidate & 2047, 1024) <= ROOT_GATE_
        ? correct_rsqrt_(mantissa, exponent, rounded) : rounded;
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
    u128 rounded_bits = (u128)(F128_BIAS - 1) << F128_EXP_SHIFT;
    rounded_bits += (candidate + 1024) >> 11;
    long double rounded = f128_from_bits_(rounded_bits);
    long double result = abs_diff_(candidate & 2047, 1024) <= ROOT_GATE_
        ? correct_cbrt_(mantissa, remainder, rounded) : rounded;
    u128 scaled = f128_bits_(result);
    u128 step = (u128)(scale < 0 ? -scale : scale) << F128_EXP_SHIFT;
    scaled = scale < 0 ? scaled - step : scaled + step;
    return f128_from_bits_((bits & F128_SIGN_MASK) | scaled);
}
