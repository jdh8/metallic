#include "../../math/double/shift.h"
#include "../../math/reinterpret.h"
#include "bigint.h"
#include "decimal.h"
#include "pow5.h"
#include <math.h>
#include <stdint.h>

static int clz_u128_(unsigned __int128 x)
{
    uint64_t hi = x >> 64;
    return hi ? __builtin_clzll(hi) : 64 + __builtin_clzll((uint64_t)x);
}

/* Divide a 256-bit dividend (n_hi:n_lo) by a 128-bit divisor d, returning a
 * 128-bit quotient and the 128-bit remainder.  Requires n_hi < d so that
 * the quotient fits in 128 bits.  Bit-by-bit long division -- ~128
 * iterations, same shape as the software path of udivmodti4_. */
static unsigned __int128 udiv256by128_(
    unsigned __int128 n_hi, unsigned __int128 n_lo,
    unsigned __int128 d, unsigned __int128 r[static 1])
{
    unsigned __int128 q = 0;
    unsigned __int128 rem = n_hi;

    for (int i = 127; i >= 0; --i) {
        int carry = (int)(rem >> 127);
        rem = (rem << 1) | ((n_lo >> i) & 1);
        q <<= 1;
        if (carry || rem >= d) {
            rem -= d;
            q |= 1;
        }
    }

    *r = rem;
    return q;
}

/* Convert a decimal_t to a correctly-rounded double using a 128-bit fast
 * path: normalize the mantissa to 128 bits, compose 5^|dec_exp| via pow5_,
 * then multiply (positive exp) or 256/128-divide (negative exp) to obtain
 * a 128-bit candidate.  Round-half-to-even is applied in a single step
 * after the subnormal shift to avoid double-rounding.  Halfway-ambiguous
 * inputs (where digit truncation could flip the rounding decision) are
 * resolved by a bigint slow path in a later commit. */
static double decimal_to_double_(const decimal_t* d)
{
    if (!d->mant)
        return 0;

    if (d->dec_exp > 309)
        return HUGE_VAL;

    if (d->dec_exp < -380)
        return 0;

    int leading = clz_u128_(d->mant);
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
