#ifndef METALLIC_STDLIB_PARSE_U256_H_
#define METALLIC_STDLIB_PARSE_U256_H_

#include "../../soft/integer/umulti4.h"
#include <stdint.h>

typedef struct {
    unsigned __int128 hi;
    unsigned __int128 lo;
} u256_t;

/* Compute the top u256 of a u256 * u256 product.  The low 256 bits are
 * truncated -- callers that need them must save them before composing.
 * For pow5l_, ~1 ulp of truncation error per multiply at u256 precision
 * is far below the binary128 target ulp (2^-113), so the loss is safe. */
static u256_t umulti256_(u256_t a, u256_t b)
{
    unsigned __int128 hh_hi, hl_hi, lh_hi, ll_hi;
    unsigned __int128 hh_lo = umulti4_(a.hi, b.hi, &hh_hi);
    unsigned __int128 hl_lo = umulti4_(a.hi, b.lo, &hl_hi);
    unsigned __int128 lh_lo = umulti4_(a.lo, b.hi, &lh_hi);
    (void)umulti4_(a.lo, b.lo, &ll_hi);

    /* bits 128-255: ll_hi + hl_lo + lh_lo, with carry out into bits 256.. */
    unsigned __int128 mid = ll_hi + hl_lo;
    unsigned mid_carry = mid < ll_hi;
    mid += lh_lo;
    mid_carry += mid < lh_lo;

    /* bits 256-383 (lo of top u256): hh_lo + hl_hi + lh_hi + mid_carry */
    unsigned __int128 lo = hh_lo + mid_carry;
    unsigned lo_carry = lo < hh_lo;
    lo += hl_hi;
    lo_carry += lo < hl_hi;
    lo += lh_hi;
    lo_carry += lo < lh_hi;

    /* bits 384-511 (hi of top u256): hh_hi + lo_carry */
    return (u256_t){ hh_hi + lo_carry, lo };
}

static u256_t u256_shl_(u256_t a, unsigned n)
{
    if (!n)
        return a;
    return (u256_t){ (a.hi << n) | (a.lo >> (128 - n)), a.lo << n };
}

#endif
