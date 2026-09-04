#ifndef METALLIC_MATH_LONG_DOUBLE_KERNEL_ROOTS_H_
#define METALLIC_MATH_LONG_DOUBLE_KERNEL_ROOTS_H_

#include "rsqrt_tables.h"
#include "uint.h"
#include <stdint.h>

static inline u128 rsqrt_step_(u128 w, u128 q)
{
    const u128 unit = (u128)1 << 124;
    u128 residual = mhi_(w, mhi_(q, q));
    return residual >= unit
        ? q - (mhi_(q, residual - unit) << 3)
        : q + (mhi_(q, unit - residual) << 3);
}

static inline uint64_t rsqrt64_(u128 mantissa, int octave)
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

static inline u128 sqrt_wide_(u128 z)
{
    int octave = z >> 127;
    uint64_t r = rsqrt64_(z >> (14 + octave), octave);
    u128 s0 = (u128)r * (z >> 64) + ((u128)r * (uint64_t)z >> 64);
    /* r * s0 >> 64 undershoots r^2 z / 2^128 by < 1.5 units, tighter than
     * the 3 of mhi_approx_(r^2, z), and reuses the s0 product. */
    u128 hp = ((u128)1 << 124)
        - ((u128)r * (s0 >> 64) + ((u128)r * (uint64_t)s0 >> 64));
    u128 linear = mhi_approx_(s0, hp << 3);
    u128 hs = (uint64_t)(hp >> 28);
    uint64_t s64 = s0 >> 62;
    uint64_t q3 = (uint64_t)((hs * hs) >> 68) * 3;
    u128 quadratic = ((u128)q3 * s64) >> 65;
    return s0 + linear + quadratic;
}

#endif
