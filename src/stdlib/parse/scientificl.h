#include "../../math/reinterpret.h"
#include "bigint.h"
#include "decimal.h"
#include "pow5l.h"
#include "u256.h"
#include <math.h>
#include <stdint.h>

static int clz_u128_l_(unsigned __int128 x)
{
    uint64_t hi = x >> 64;
    return hi ? __builtin_clzll(hi) : 64 + __builtin_clzll((uint64_t)x);
}

static int u256_lt_(u256_t a, u256_t b)
{
    return a.hi < b.hi || (a.hi == b.hi && a.lo < b.lo);
}

static u256_t u256_shr_1_(u256_t a)
{
    return (u256_t){ a.hi >> 1, (a.hi << 127) | (a.lo >> 1) };
}

static u256_t u256_sub_(u256_t a, u256_t b)
{
    unsigned __int128 borrow = a.lo < b.lo;
    return (u256_t){ a.hi - b.hi - borrow, a.lo - b.lo };
}

/* Divide a 512-bit dividend (n_hi:n_lo) by a 256-bit divisor d, returning
 * a 256-bit quotient and the 256-bit remainder.  Requires n_hi < d so the
 * quotient fits in 256 bits.  Bit-by-bit -- 256 iterations of a shift /
 * compare / subtract at u256 width, the same shape as the udiv256by128_
 * loop in parse/scientific.h. */
static u256_t udiv512by256_(u256_t n_hi, u256_t n_lo, u256_t d, u256_t r[static 1])
{
    u256_t q = {0, 0};
    u256_t rem = n_hi;

    for (int i = 255; i >= 0; --i) {
        int carry = (int)(rem.hi >> 127);
        unsigned bit = (i >= 128)
            ? (unsigned)((n_lo.hi >> (i - 128)) & 1)
            : (unsigned)((n_lo.lo >> i) & 1);
        rem.hi = (rem.hi << 1) | (rem.lo >> 127);
        rem.lo = (rem.lo << 1) | bit;
        q.hi = (q.hi << 1) | (q.lo >> 127);
        q.lo <<= 1;
        if (carry || !u256_lt_(rem, d)) {
            rem = u256_sub_(rem, d);
            q.lo |= 1;
        }
    }

    *r = rem;
    return q;
}

/* Convert a decimal_t to a correctly-rounded long double (binary128) via a
 * 256-bit fast path -- same structure as decimal_to_double_ widened to
 * u256.  Normalize the u128 mantissa to occupy the high half of a u256,
 * compose 5^|dec_exp| via pow5l_, multiply (positive exp) or 512/256-
 * divide (negative exp) to obtain a 256-bit candidate, then round-half-
 * to-even to 113 bits in a single step after the subnormal shift.
 * Halfway-ambiguous inputs (where digit truncation could flip the
 * rounding decision) are resolved by the bigint slow path. */
static long double decimal_to_long_double_(const decimal_t* d)
{
    if (!d->mant)
        return 0;

    if (d->dec_exp > 4933)
        return HUGE_VALL;

    /* The parser folds (total_digits - 38) into d->dec_exp on truncation, so
     * truncated inputs near halfway-from-zero can land with d->dec_exp
     * substantially below LDBL_MIN_10_EXP - DECIMAL_DIG.  Pad generously. */
    if (d->dec_exp < -5050)
        return 0;

    int leading = clz_u128_l_(d->mant);
    u256_t mant = { d->mant << leading, 0 };
    int binexp = -leading - 128 + d->dec_exp;
    _Bool sticky_lo = d->truncated;

    if (d->dec_exp >= 0) {
        pow5l_t p = pow5l_(d->dec_exp);
        u256_t product = umulti256_(mant, p.mant);
        int shift = !(product.hi >> 127);
        mant.hi = (product.hi << shift) | (shift ? product.lo >> 127 : 0);
        mant.lo = product.lo << shift;
        /* umulti256_ already dropped the low u256 of the u512 product; any
         * nonzero bit there would set sticky_lo, but at u256 precision the
         * loss is far below 1 ulp at the 113-bit target. */
        binexp += p.binexp + 256 - shift;
    } else {
        pow5l_t p = pow5l_(-d->dec_exp);
        u256_t dividend_hi, dividend_lo, rem;
        int k;

        if (u256_lt_(mant, p.mant)) {
            k = 256;
            dividend_hi = mant;
            dividend_lo = (u256_t){0, 0};
        } else {
            k = 255;
            dividend_hi = u256_shr_1_(mant);
            dividend_lo.hi = (mant.lo & 1) ? ((unsigned __int128)1 << 127) : 0;
            dividend_lo.lo = 0;
        }

        mant = udiv512by256_(dividend_hi, dividend_lo, p.mant, &rem);
        if (rem.hi || rem.lo)
            sticky_lo = 1;
        binexp -= p.binexp + k;
    }

    int biased = binexp + 255 + 0x3FFF;
    int shift = biased >= 1 ? 143 : 143 + 1 - biased;

    if (biased < 1)
        biased = 0;

    unsigned __int128 result_mant;
    int round_bit;
    _Bool sticky;

    if (shift >= 256) {
        result_mant = 0;
        round_bit = shift == 256 ? (int)(mant.hi >> 127) : 0;
        sticky = shift == 256
            ? (mant.hi & (((unsigned __int128)1 << 127) - 1)) != 0 || mant.lo != 0
            : (mant.hi != 0 || mant.lo != 0);
    } else {
        /* shift in [143, 256); split into the hi-half-only case (shift >= 128)
         * versus a hi+lo combine -- but our shift is always >= 143, so the
         * combine is unreachable.  The compiler can fold the >= 128 check. */
        int s = shift - 128;
        result_mant = mant.hi >> s;
        if (s == 0) {
            round_bit = (int)(mant.lo >> 127);
            sticky = (mant.lo & (((unsigned __int128)1 << 127) - 1)) != 0;
        } else {
            round_bit = (int)((mant.hi >> (s - 1)) & 1);
            sticky = (mant.hi & (((unsigned __int128)1 << (s - 1)) - 1)) != 0
                  || mant.lo != 0;
        }
    }
    sticky = sticky || sticky_lo;

    _Bool round_up;
    if (round_bit && d->truncated) {
        int cmp = bigint_cmp_halfway_(d, 2 * result_mant + 1, shift + binexp - 1);
        round_up = cmp > 0 || (cmp == 0 && (result_mant & 1));
    } else {
        round_up = round_bit && (sticky || (result_mant & 1));
    }

    if (round_up)
        ++result_mant;

    if (result_mant >> 113) {
        result_mant >>= 1;
        ++biased;
    } else if ((result_mant >> 112) && biased == 0) {
        biased = 1;
    }

    if (biased >= 0x7FFF)
        return HUGE_VALL;

    unsigned __int128 bits = ((unsigned __int128)biased << 112)
                           | (result_mant & (((unsigned __int128)1 << 112) - 1));
    return reinterpret(long double, bits);
}

static long double decimal_to_scalar_(const decimal_t* d)
{
    return decimal_to_long_double_(d);
}
