#include "../../math/reinterpret.h"
#include "../../soft/integer/clzti2.h"
#include "../../soft/integer/uldivmoddi5.h"
#include "bigint.h"
#include "decimal.h"
#include "pow5.h"
#include <math.h>
#include <stdint.h>

/* One digit of Knuth Algorithm D with 64-bit limbs: divide (u:u0), a
 * 192-bit value with u < d1:d0, by the divisor d1:d0.  The divisor comes
 * from pow5_ and is top-bit-normalized, so no normalization shift is
 * needed and the initial 128/64 estimate is off by at most 2.  With a
 * two-limb divisor the correction test is exact, so no add-back step. */
static uint64_t udiv192by128_(
    unsigned __int128 u, uint64_t u0, uint64_t d1, uint64_t d0,
    unsigned __int128 rem[static 1])
{
    uint64_t u1 = u >> 64;
    uint64_t qhat;
    unsigned __int128 rhat;

    if (u1 == d1) {
        /* uldivmoddi5_ would overflow; u < d caps the true digit at 2^64 - 1. */
        qhat = UINT64_MAX;
        rhat = (uint64_t)u + (unsigned __int128)d1;
    } else {
        uint64_t r64;
        qhat = uldivmoddi5_(u1, u, d1, &r64);
        rhat = r64;
    }

    while (!(rhat >> 64) && (unsigned __int128)qhat * d0 > (rhat << 64 | u0)) {
        --qhat;
        rhat += d1;
    }

    /* True remainder rhat:u0 - qhat * d0 is in [0, d), so computing it
     * mod 2^128 is exact even when rhat has outgrown 64 bits. */
    *rem = (rhat << 64 | u0) - (unsigned __int128)qhat * d0;
    return qhat;
}

/* Divide a 256-bit dividend (n_hi:n_lo) by a 128-bit divisor d, returning a
 * 128-bit quotient and the 128-bit remainder.  Requires n_hi < d so that
 * the quotient fits in 128 bits, and a top-bit-set d (pow5_ mantissas are
 * normalized). */
static unsigned __int128 udiv256by128_(
    unsigned __int128 n_hi, unsigned __int128 n_lo,
    unsigned __int128 d, unsigned __int128 r[static 1])
{
    unsigned __int128 rem;
    uint64_t q1 = udiv192by128_(n_hi, n_lo >> 64, d >> 64, d, &rem);
    uint64_t q0 = udiv192by128_(rem, n_lo, d >> 64, d, &rem);

    *r = rem;
    return (unsigned __int128)q1 << 64 | q0;
}

/* Convert a decimal_t to a correctly-rounded double using a 128-bit fast
 * path: normalize the mantissa to 128 bits, compose 5^|dec_exp| via pow5_,
 * then multiply (positive exp) or 256/128-divide (negative exp) to obtain
 * a 128-bit candidate.  Round-half-to-even is applied in a single step
 * after the subnormal shift to avoid double-rounding.  Halfway-ambiguous
 * inputs (where digit truncation could flip the rounding decision) are
 * resolved by a bigint slow path in a later commit. */
/* 10^k is exact in double up to k = 22 (5^22 < 2^53). */
static const double pow10_[23] = {
    1e0,  1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,  1e10, 1e11,
    1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22,
};

static double decimal_to_double_(const decimal_t* d)
{
    if (!d->mant)
        return 0;

    /* Clinger fast path: a mantissa below 2^53 converts to double exactly,
     * and 10^k above is exact, so one correctly-rounded multiply or divide
     * of exact operands is the correctly-rounded result -- bit-identical
     * to the 128-bit path below, at a fraction of the cost. */
    if (!d->truncated && !(d->mant >> 53)) {
        double m = (double)(uint64_t)d->mant;

        if ((unsigned)d->dec_exp <= 22)
            return m * pow10_[d->dec_exp];

        if (d->dec_exp >= -22)
            return m / pow10_[-d->dec_exp];
    }

    if (d->dec_exp > 309)
        return HUGE_VAL;

    if (d->dec_exp < -380)
        return 0;

    int leading = clzti2_(d->mant);
    unsigned __int128 mant = d->mant << leading;
    int binexp = -leading + d->dec_exp;
    _Bool sticky_lo = d->truncated;

    if (d->dec_exp >= 0) {
        pow5_t p = pow5_(d->dec_exp);
        unsigned __int128 hi;
        unsigned __int128 lo = umulti4_(mant, p.mant, &hi);
        int shift = !(hi >> 127);
        mant = (hi << shift) | (shift ? lo >> 127 : 0);
        if (shift ? (lo & (((unsigned __int128)1 << 127) - 1)) != 0 : lo != 0)
            sticky_lo = 1;
        binexp += p.binexp + 128 - shift;
    } else {
        pow5_t p = pow5_(-d->dec_exp);
        unsigned __int128 rem;
        int k = mant < p.mant ? 128 : 127;
        if (k == 128)
            mant = udiv256by128_(mant, 0, p.mant, &rem);
        else
            mant = udiv256by128_(mant >> 1, mant << 127, p.mant, &rem);
        if (rem)
            sticky_lo = 1;
        binexp -= p.binexp + k;
    }

    int biased = binexp + 127 + 1023;
    int shift = biased >= 1 ? 75 : 75 + 1 - biased;

    if (biased < 1)
        biased = 0;

    uint64_t result_mant;
    int round_bit;
    _Bool sticky;

    if (shift >= 128) {
        result_mant = 0;
        round_bit = shift == 128 ? (int)(mant >> 127) : 0;
        sticky = shift == 128
            ? (mant & (((unsigned __int128)1 << 127) - 1)) != 0
            : mant != 0;
    } else {
        result_mant = (uint64_t)(mant >> shift);
        round_bit = (int)((mant >> (shift - 1)) & 1);
        sticky = (mant & (((unsigned __int128)1 << (shift - 1)) - 1)) != 0;
    }
    sticky = sticky || sticky_lo;

    _Bool round_up;
    if (round_bit && d->truncated) {
        /* Halfway-ambiguous: bigint settles it by comparing the exact
         * decimal D against the halfway H = (2 result_mant + 1) * 2^(shift +
         * binexp - 1). */
        int cmp = bigint_cmp_halfway_(d, 2 * result_mant + 1, shift + binexp - 1);
        round_up = cmp > 0 || (cmp == 0 && (result_mant & 1));
    } else {
        round_up = round_bit && (sticky || (result_mant & 1));
    }

    if (round_up)
        ++result_mant;

    if (result_mant >> 53) {
        result_mant >>= 1;
        ++biased;
    } else if ((result_mant >> 52) && biased == 0) {
        biased = 1;
    }

    if (biased >= 2047)
        return HUGE_VAL;

    uint64_t bits = ((uint64_t)biased << 52) | (result_mant & ((UINT64_C(1) << 52) - 1));
    return reinterpret(double, bits);
}
