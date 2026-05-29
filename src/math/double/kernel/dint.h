/* Accurate-path 128-bit fixed-point logarithm (Ziv second step).
 *
 * Ported from metallic-rs f64/dint.rs (itself a port of Tom Hubrecht's
 * dint64_t arithmetic in CORE-MATH binary64/log/dint.h).  A dint_t represents
 *   value = (-1)^sgn * (m / 2^127) * 2^ex,
 * with the 128-bit significand m normalized so bit 127 is set (m in
 * [2^127, 2^128)) for nonzero values.  CORE-MATH stores m as two uint64 words
 * {hi, lo}; here they are packed as m = (hi << 64) | lo.
 *
 * This is exact integer arithmetic carrying ~127 significant bits, so there are
 * no FMA concerns: every operation below is plain integer add/sub/mul.  It is
 * the rare accurate fallback behind the double-double fast path in logtab.h.
 *
 * Constants (INVERSE_2, LOG_INV_2, P_2, LOG2, M_ONE, ZERO) were converted
 * mechanically from f64/dint_consts.rs (regex script over the Rust `Dint { sgn,
 * ex, m }` literals); a few were cross-checked by eye against CORE-MATH dint.h.
 * LOG2E_dint / LOG10E_dint (log2(e), log10(e) to 128 bits) were computed here
 * and verified through the oracle. */
#ifndef METALLIC_KERNEL_DINT_H
#define METALLIC_KERNEL_DINT_H

#include "../../reinterpret.h"
#include <math.h>
#include <stdint.h>

typedef struct {
    _Bool sgn;
    int64_t ex;
    unsigned __int128 m;
} dint_t;

/* Count leading zeros of a 128-bit value; returns 128 for zero (matches Rust
 * u128::leading_zeros). */
static inline int dint_clz_(unsigned __int128 x)
{
    uint64_t hi = (uint64_t)(x >> 64);
    uint64_t lo = (uint64_t)x;

    if (hi)
        return __builtin_clzll(hi);
    if (lo)
        return 64 + __builtin_clzll(lo);
    return 128;
}

/* Compare magnitudes: -1, 0, 1 for a < b, a == b, a > b (by ex then m). */
static inline int dint_cmp_magnitude_(const dint_t *a, const dint_t *b)
{
    if (a->ex != b->ex)
        return a->ex < b->ex ? -1 : 1;
    if (a->m != b->m)
        return a->m < b->m ? -1 : 1;
    return 0;
}

/* Round-to-nearest sum (port of add_dint). */
static inline dint_t dint_add_(const dint_t *self, const dint_t *other)
{
    static const dint_t ZERO = { 0, 0, 0 };

    if (self->m == 0)
        return *other;
    if (other->m == 0)
        return *self;

    const dint_t *a, *b;
    int cmp = dint_cmp_magnitude_(self, other);

    if (cmp == 0) {
        if (self->sgn != other->sgn)
            return ZERO;
        /* Equal magnitudes, same sign: result is 2*a. */
        return (dint_t){ self->sgn, self->ex + 1, self->m };
    }
    if (cmp < 0) {
        a = other;
        b = self;
    } else {
        a = self;
        b = other;
    }

    /* From now on |a| > |b|. */
    unsigned __int128 big = a->m;
    unsigned __int128 small = b->m;
    int64_t m_ex = a->ex;

    if (a->ex > b->ex) {
        int64_t sh = a->ex - b->ex;
        /* Round to nearest before discarding the shifted-out bits. */
        if (sh <= 128)
            small += 1 & (unsigned __int128)(small >> (sh - 1));
        small = sh < 128 ? small >> sh : 0;
    }

    _Bool sgn = a->sgn;
    unsigned __int128 c;

    if (a->sgn != b->sgn) {
        /* Different signs: C = A - B (no borrow since |A| > |B|). */
        c = big - small;
    } else {
        c = big + small;
        if (c < big) {
            /* Carry out of bit 127: shift right one, rounding, restore bit 127. */
            c += c & 1;
            c = ((unsigned __int128)1 << 127) | (c >> 1);
            m_ex += 1;
        }
    }

    /* Renormalize so bit 127 is set. */
    int shift = dint_clz_(c);
    c <<= shift;

    return (dint_t){ sgn, m_ex - shift, c };
}

/* Round-to-nearest product with ~126-bit accuracy (port of mul_dint). */
static inline dint_t dint_mul_(const dint_t *self, const dint_t *other)
{
    uint64_t a_hi = (uint64_t)(self->m >> 64);
    uint64_t a_lo = (uint64_t)self->m;
    uint64_t b_hi = (uint64_t)(other->m >> 64);
    uint64_t b_lo = (uint64_t)other->m;

    /* t = a_hi * b_hi (top 128 bits of the product so far). */
    unsigned __int128 t = (unsigned __int128)a_hi * b_hi;

    unsigned __int128 m1 = (unsigned __int128)a_hi * b_lo;
    unsigned __int128 m2 = (unsigned __int128)a_lo * b_hi;

    /* m = m1 + m2; carry folds into t's high word. */
    unsigned __int128 m = m1 + m2;
    if (m < m1)
        t += (unsigned __int128)1 << 64;
    t += (uint64_t)(m >> 64);

    /* Ensure the leading bit (bit 127 of t) is set. */
    int ex = (t >> 127) == 0;
    if (ex)
        t <<= 1;

    /* Round bit: bit 63 of the low word of m. */
    t += (uint64_t)m >> 63;

    return (dint_t){ self->sgn ^ other->sgn, self->ex + other->ex - ex + 1, t };
}

/* Product with a signed integer (port of mul_dint_2). */
static inline dint_t dint_mul_int_(const dint_t *self, int64_t b)
{
    static const dint_t ZERO = { 0, 0, 0 };

    if (b == 0)
        return ZERO;

    uint64_t c = b < 0 ? -(uint64_t)b : (uint64_t)b;
    _Bool sgn = b < 0 ? !self->sgn : self->sgn;

    uint64_t a_hi = (uint64_t)(self->m >> 64);
    uint64_t a_lo = (uint64_t)self->m;

    /* t = a_hi * c, then left-justify it. */
    unsigned __int128 t = (unsigned __int128)a_hi * c;
    uint64_t leading = (uint64_t)(t >> 64);
    int sh = leading ? __builtin_clzll(leading) : 64;
    t <<= sh;

    /* l = (a_lo * c) shifted to align, keeping one rounding bit. */
    unsigned __int128 l = (((unsigned __int128)a_lo * c) << (sh - 1)) >> 63;

    unsigned __int128 sum = l + t;
    if (sum < l) {
        sum += sum & 1;
        sum = ((unsigned __int128)1 << 127) | (sum >> 1);
        sh -= 1;
    }

    return (dint_t){ sgn, self->ex + 64 - sh, sum };
}

/* Convert a finite positive f64 to a dint_t (port of dint_fromd, including the
 * subnormal preparation cr_log does before calling it). */
static inline dint_t dint_from_f64_(double b)
{
    int64_t bias = 0;

    if (fabs(b) < 0x1p-1022) {
        b *= 0x1p52;
        bias = 52;
    }

    uint64_t bits = reinterpret(uint64_t, b);
    int64_t biased = (int64_t)((bits >> 52) & 0x7ff);
    /* Significand with the implicit leading bit restored. */
    uint64_t hi = (bits & (~0ULL >> 12)) + (1ULL << 52);
    int64_t ex = biased - 0x3ff - bias;

    /* Left-justify hi (a 53-bit value) into the top word. */
    int t = __builtin_clzll(hi);
    return (dint_t){
        b < 0.0,
        ex - (t > 11 ? t - 12 : 0),
        (unsigned __int128)(hi << t) << 64,
    };
}

/* Convert to f64 with correct rounding (port of dint_tod).  Assumes the result
 * is in the normal range, which always holds for ln of finite positive x. */
static inline double dint_to_f64_(const dint_t *self)
{
    uint64_t hi = (uint64_t)(self->m >> 64);

    /* Upper 53 bits of hi as a mantissa in [1, 2). */
    double r = reinterpret(double, (hi >> 11) | (0x3ffULL << 52));

    double rd = 0.0;
    /* Round bit (bit 10 of hi): add 2^-53. */
    if ((hi >> 10) & 1)
        rd += 0x1p-53;
    /* Sticky bits: add 2^-54. */
    if ((hi & 0x3ff) != 0 || (uint64_t)self->m != 0)
        rd += 0x1p-54;

    if (self->sgn) {
        r = -r;
        r -= rd;
    } else {
        r += rd;
    }

    /* Scale by 2^ex.  For ln, -1023 < ex < 1023, so this is one exact step. */
    double e = reinterpret(double, ((uint64_t)((self->ex + 1023) & 0x7ff)) << 52);
    return r * e;
}

/* === Constants, converted from metallic-rs f64/dint_consts.rs ===
 * value = (-1)^sgn * (m / 2^127) * 2^ex; { sgn, ex, m }. */

/* m * INVERSE_2[i-128] ~ 1: reduces a [1/sqrt2, sqrt2) mantissa toward 1. */
static const dint_t dint_inverse_2_[240] = {
    { 0, 1, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xfe03f80fe03f80ffULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xfc0fc0fc0fc0fc10ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xfa232cf252138ac0ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xf83e0f83e0f83e10ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xf6603d980f6603daULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xf4898d5f85bb3951ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xf2b9d6480f2b9d65ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xf0f0f0f0f0f0f0f1ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xef2eb71fc4345239ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xed7303b5cc0ed731ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xebbdb2a5c1619c8cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xea0ea0ea0ea0ea0fULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xe865ac7b7603a197ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xe6c2b4481cd8568aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xe525982af70c880fULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xe38e38e38e38e38fULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xe1fc780e1fc780e2ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xe070381c0e070382ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xdee95c4ca037ba58ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xdd67c8a60dd67c8bULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xdbeb61eed19c5958ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xda740da740da740eULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xd901b2036406c80eULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xd79435e50d79435fULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xd62b80d62b80d62cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xd4c77b03531dec0eULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xd3680d3680d3680eULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xd20d20d20d20d20eULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xd0b69fcbd2580d0cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xcf6474a8819ec8eaULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xce168a7725080ce2ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xcccccccccccccccdULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xcb8727c065c393e1ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xca4587e6b74f032aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xc907da4e871146adULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xc7ce0c7ce0c7ce0dULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xc6980c6980c6980dULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xc565c87b5f9d4d1cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xc4372f855d824ca6ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xc30c30c30c30c30dULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xc1e4bbd595f6e948ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xc0c0c0c0c0c0c0c1ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xbfa02fe80bfa02ffULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xbe82fa0be82fa0bfULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xbd69104707661aa3ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xbc52640bc52640bdULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xbb3ee721a54d880cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xba2e8ba2e8ba2e8cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb92143fa36f5e02fULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb81702e05c0b8171ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb70fbb5a19be3659ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb60b60b60b60b60cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb509e68a9b948220ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb40b40b40b40b40cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb30f63528917c80cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb21642c8590b2165ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb11fd3b80b11fd3cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xb02c0b02c0b02c0cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xaf3addc680af3adeULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xae4c415c9882b932ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xad602b580ad602b6ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xac7691840ac76919ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xab8f69e28359cd12ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xaaaaaaaaaaaaaaabULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa9c84a47a07f5638ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa8e83f5717c0a8e9ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa80a80a80a80a80bULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa72f05397829cbc2ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa655c4392d7b73a8ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa57eb50295fad40bULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa4a9cf1d96833752ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa3d70a3d70a3d70bULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa3065e3fae7cd0e1ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa237c32b16cfd773ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa16b312ea8fc377dULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0xa0a0a0a0a0a0a0a1ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9fd809fd809fd80aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9f1165e7254813e3ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9e4cad23dd5f3a21ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9d89d89d89d89d8aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9cc8e160c3fb19b9ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9c09c09c09c09c0aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9b4c6f9ef03a3caaULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9a90e7d95bc609aaULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x99d722dabde58f07ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x991f1a515885fb38ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9868c809868c8099ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x97b425ed097b425fULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x97012e025c04b80aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x964fda6c0964fda7ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x95a02568095a0257ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x94f2094f2094f20aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x9445809445809446ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x939a85c40939a85dULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x92f113840497889dULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x924924924924924aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x91a2b3c4d5e6f80aULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x90fdbc090fdbc091ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x905a38633e06c43bULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8fb823ee08fb823fULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8f1779d9fdc3a219ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8e78356d1408e784ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8dda520237694809ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8d3dcb08d3dcb08eULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8ca29c046514e024ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8c08c08c08c08c09ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8b70344a139bc75bULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8ad8f2fba9386823ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8a42f8705669db47ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x89ae4089ae4089afULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x891ac73ae9819b51ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8888888888888889ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x87f78087f78087f8ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8767ab5f34e47ef2ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x86d905447a34acc7ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x864b8a7de6d1d609ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x85bf37612cee3c9bULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8534085340853409ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x84a9f9c8084a9f9dULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8421084210842109ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x839930523fbe3368ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x83126e978d4fdf3cULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x828cbfbeb9a020a4ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8208208208208209ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x81848da8faf0d278ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8102040810204082ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xff00ff00ff00ff02ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xfe03f80fe03f80ffULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xfd08e5500fd08e56ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xfc0fc0fc0fc0fc11ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xfb18856506ddaba7ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xfa232cf252138ac1ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf92fb2211855a866ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf83e0f83e0f83e11ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf74e3fc22c700f76ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf6603d980f6603dbULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf57403d5d00f5741ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf4898d5f85bb3951ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf3a0d52cba872337ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf2b9d6480f2b9d66ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf1d48bcee0d399fbULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf0f0f0f0f0f0f0f2ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xf00f00f00f00f010ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xef2eb71fc4345239ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xee500ee500ee5010ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xed7303b5cc0ed731ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xec979118f3fc4da3ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xebbdb2a5c1619c8dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xeae56403ab959010ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xea0ea0ea0ea0ea10ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe939651fe2d8d35dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe865ac7b7603a198ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe79372e225fe30daULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe6c2b4481cd8568aULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe5f36cb00e5f36ccULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe525982af70c880fULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe45932d7dc52100fULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe38e38e38e38e38fULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe2c4a6886a4c2e11ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe1fc780e1fc780e3ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe135a9c97500e137ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xe070381c0e070383ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xdfac1f74346c5760ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xdee95c4ca037ba58ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xde27eb2c41f3d9d2ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xdd67c8a60dd67c8bULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xdca8f158c7f91ab9ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xdbeb61eed19c5959ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xdb2f171df770291aULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xda740da740da740fULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd9ba4256c0366e92ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd901b2036406c80fULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd84a598ec9151f44ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd79435e50d79435fULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd6df43fca482f00eULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd62b80d62b80d62dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd578e97c3f5fe552ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd4c77b03531dec0eULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd4173289870ac52fULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd3680d3680d3680eULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd2ba083b445250acULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd20d20d20d20d20eULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd161543e28e50275ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd0b69fcbd2580d0cULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xd00d00d00d00d00eULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xcf6474a8819ec8eaULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xcebcf8bb5b4169ccULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xce168a7725080ce2ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xcd712752a886d243ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xccccccccccccccceULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xcc29786c7607f9a0ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xcb8727c065c393e1ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xcae5d85f1bbd6c96ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xca4587e6b74f032aULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc9a633fcd967300eULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc907da4e871146aeULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc86a78900c86a78aULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc7ce0c7ce0c7ce0dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc73293d789b9f839ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc6980c6980c6980dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc5fe740317f9d00dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc565c87b5f9d4d1dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc4ce07b00c4ce07cULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc4372f855d824ca7ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc3a13de60495c774ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc30c30c30c30c30dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc2780613c0309e03ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc1e4bbd595f6e948ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc152500c152500c2ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc0c0c0c0c0c0c0c2ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xc0300c0300c0300dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbfa02fe80bfa0300ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbf112a8ad278e8deULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbe82fa0be82fa0c0ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbdf59c91700bdf5bULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbd69104707661aa4ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbcdd535db1cc5b7cULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbc52640bc52640bdULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbbc8408cd63069a2ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbb3ee721a54d880dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xbab656100bab6562ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xba2e8ba2e8ba2e8dULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb9a7862a0ff46589ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb92143fa36f5e02fULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb89bc36ce3e0453bULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb81702e05c0b8171ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb79300b79300b794ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb70fbb5a19be365aULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb68d31340e4307d9ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb60b60b60b60b60cULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb58a485518d1e7e5ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb509e68a9b948220ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb48a39d44685fe98ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb40b40b40b40b40cULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb38cf9b00b38cf9cULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb30f63528917c80cULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0xb2927c29da5519d0ULL << 64) | 0x0000000000000000ULL },
};

/* LOG_INV_2[i-128] = log(INVERSE_2[i-128]) ~ -log(m). */
static const dint_t dint_log_inv_2_[240] = {
    { 1, -1, ((unsigned __int128)0xb17217f7d1cf79abULL << 64) | 0xc9e3b39803f2f6afULL },
    { 1, -1, ((unsigned __int128)0xaf74155120c9011dULL << 64) | 0x046d235ee63073dcULL },
    { 1, -1, ((unsigned __int128)0xad7a02e1b24efd32ULL << 64) | 0x160864fd949b4bd3ULL },
    { 1, -1, ((unsigned __int128)0xab83d135dc633301ULL << 64) | 0xffe6607ba902ef3bULL },
    { 1, -1, ((unsigned __int128)0xa991713433c2b999ULL << 64) | 0x0ba4aea614d05700ULL },
    { 1, -1, ((unsigned __int128)0xa7a2d41ad270c9d7ULL << 64) | 0xcd362382a7688479ULL },
    { 1, -1, ((unsigned __int128)0xa5b7eb7cb860fb89ULL << 64) | 0x7b6a62a0dec6e072ULL },
    { 1, -1, ((unsigned __int128)0xa3d0a93f45169a4bULL << 64) | 0x09594fab088c0d64ULL },
    { 1, -1, ((unsigned __int128)0xa1ecff97c91e267bULL << 64) | 0x1b7efae08e597e16ULL },
    { 1, -1, ((unsigned __int128)0xa00ce1092e5498c4ULL << 64) | 0x69879c5a30cd1241ULL },
    { 1, -1, ((unsigned __int128)0x9e304061b5fda91aULL << 64) | 0x04603d87b6df81acULL },
    { 1, -1, ((unsigned __int128)0x9c5710b8cbb73a42ULL << 64) | 0xaa554b2dd4619e63ULL },
    { 1, -1, ((unsigned __int128)0x9a81456cec642e10ULL << 64) | 0x4d49f9aaea3cb5e0ULL },
    { 1, -1, ((unsigned __int128)0x98aed221a03458b6ULL << 64) | 0x732f89321647b358ULL },
    { 1, -1, ((unsigned __int128)0x96dfaabd86fa1647ULL << 64) | 0xd61188fbc94e2f14ULL },
    { 1, -1, ((unsigned __int128)0x9513c36876083696ULL << 64) | 0xb5cbc416a2418011ULL },
    { 1, -1, ((unsigned __int128)0x934b1089a6dc93c2ULL << 64) | 0xbf5bb3b60554e151ULL },
    { 1, -1, ((unsigned __int128)0x918586c5f5e4bf01ULL << 64) | 0x9f92199ed1a4bab0ULL },
    { 1, -1, ((unsigned __int128)0x8fc31afe30b2c6deULL << 64) | 0xe300bf167e95da66ULL },
    { 1, -1, ((unsigned __int128)0x8e03c24d7300395aULL << 64) | 0xcddae1ccce247837ULL },
    { 1, -1, ((unsigned __int128)0x8c47720791e53314ULL << 64) | 0x762ad19415fe25a5ULL },
    { 1, -1, ((unsigned __int128)0x8a8e1fb794b09134ULL << 64) | 0x9eb628dba173c82dULL },
    { 1, -1, ((unsigned __int128)0x88d7c11e3ad53cdcULL << 64) | 0x8a3111a707b6de2cULL },
    { 1, -1, ((unsigned __int128)0x87244c308e670a66ULL << 64) | 0x85e005d06dbfa8f7ULL },
    { 1, -1, ((unsigned __int128)0x8573b71682a7d21bULL << 64) | 0xb21f9f89c1ab80b2ULL },
    { 1, -1, ((unsigned __int128)0x83c5f8299e2b4091ULL << 64) | 0xb8f6fafe8fbb68b8ULL },
    { 1, -1, ((unsigned __int128)0x821b05f3b01d6774ULL << 64) | 0xdb0d58c3f7e2ea1eULL },
    { 1, -1, ((unsigned __int128)0x8072d72d903d588cULL << 64) | 0x7dd1b09c70c40109ULL },
    { 1, -2, ((unsigned __int128)0xfd9ac57bd2442180ULL << 64) | 0xaf05924d258c14c4ULL },
    { 1, -2, ((unsigned __int128)0xfa553f7018c966f4ULL << 64) | 0x2780a545a1b54dceULL },
    { 1, -2, ((unsigned __int128)0xf7150ab5a09f27f6ULL << 64) | 0x0a470250d40ebe8eULL },
    { 1, -2, ((unsigned __int128)0xf3da161eed6b9ab1ULL << 64) | 0x248d42f78d3e65d2ULL },
    { 1, -2, ((unsigned __int128)0xf0a450d139366ca7ULL << 64) | 0x7c66eb6408ff6432ULL },
    { 1, -2, ((unsigned __int128)0xed73aa4264b0adebULL << 64) | 0x5391cf4b33e42996ULL },
    { 1, -2, ((unsigned __int128)0xea481236f7d35bb2ULL << 64) | 0x39a767a80d6d97e6ULL },
    { 1, -2, ((unsigned __int128)0xe72178c0323a1a0fULL << 64) | 0xcc4e1653e71d9973ULL },
    { 1, -2, ((unsigned __int128)0xe3ffce3a2aa64923ULL << 64) | 0x8eadb651b49ac539ULL },
    { 1, -2, ((unsigned __int128)0xe0e30349fd1cec82ULL << 64) | 0x03e8e1802aba24d5ULL },
    { 1, -2, ((unsigned __int128)0xddcb08dc0717d85cULL << 64) | 0x940a666c87842842ULL },
    { 1, -2, ((unsigned __int128)0xdab7d02231484a93ULL << 64) | 0xbec20cca6efe2ac4ULL },
    { 1, -2, ((unsigned __int128)0xd7a94a92466e833cULL << 64) | 0xcd88bba7d0cee8dfULL },
    { 1, -2, ((unsigned __int128)0xd49f69e456cf1b7bULL << 64) | 0x7f53bd2e406e66e6ULL },
    { 1, -2, ((unsigned __int128)0xd19a201127d3c646ULL << 64) | 0x279d79f51dcc7301ULL },
    { 1, -2, ((unsigned __int128)0xce995f50af69d863ULL << 64) | 0x432f3f4f861ad6a8ULL },
    { 1, -2, ((unsigned __int128)0xcb9d1a189ab56e77ULL << 64) | 0x7d7e9307c70c0667ULL },
    { 1, -2, ((unsigned __int128)0xc8a5431adfb44ca6ULL << 64) | 0x048ce7c1a75e341aULL },
    { 1, -2, ((unsigned __int128)0xc5b1cd44596fa51fULL << 64) | 0xf218fb8f9f9ef27fULL },
    { 1, -2, ((unsigned __int128)0xc2c2abbb6e5fd570ULL << 64) | 0x03337789d592e296ULL },
    { 1, -2, ((unsigned __int128)0xbfd7d1dec0a8df70ULL << 64) | 0x37eda996244bccafULL },
    { 1, -2, ((unsigned __int128)0xbcf13343e7d9ec7fULL << 64) | 0x2afd17781bb3afeaULL },
    { 1, -2, ((unsigned __int128)0xba0ec3b633dd8b0bULL << 64) | 0x91dc60b2b059a609ULL },
    { 1, -2, ((unsigned __int128)0xb730773578cb90b3ULL << 64) | 0xaa1116c3466beb6cULL },
    { 1, -2, ((unsigned __int128)0xb45641f4e350a0d4ULL << 64) | 0xe756eba00bc33976ULL },
    { 1, -2, ((unsigned __int128)0xb1801859d56249deULL << 64) | 0x98ce51fff99479cbULL },
    { 1, -2, ((unsigned __int128)0xaeadeefacaf97d37ULL << 64) | 0x9dd6e688ebb13b01ULL },
    { 1, -2, ((unsigned __int128)0xabdfba9e468fd6f9ULL << 64) | 0x472ea07749ce6bd1ULL },
    { 1, -2, ((unsigned __int128)0xa9157039c51ebe72ULL << 64) | 0xe164c759686a2207ULL },
    { 1, -2, ((unsigned __int128)0xa64f04f0b961df78ULL << 64) | 0x54f5275c2d15c21eULL },
    { 1, -2, ((unsigned __int128)0xa38c6e138e20d834ULL << 64) | 0xd698298adddd7f30ULL },
    { 1, -2, ((unsigned __int128)0xa0cda11eaf46390eULL << 64) | 0x632438273918db7dULL },
    { 1, -2, ((unsigned __int128)0x9e1293b9998c1dadULL << 64) | 0x3b035eae273a855cULL },
    { 1, -2, ((unsigned __int128)0x9b5b3bb5f088b768ULL << 64) | 0x5078bbe3d392be24ULL },
    { 1, -2, ((unsigned __int128)0x98a78f0e9ae71d87ULL << 64) | 0x64dec34784707838ULL },
    { 1, -2, ((unsigned __int128)0x95f783e6e49a9cfcULL << 64) | 0x025004f3ef063312ULL },
    { 1, -2, ((unsigned __int128)0x934b1089a6dc93c2ULL << 64) | 0xdf5bb3b60554e151ULL },
    { 1, -2, ((unsigned __int128)0x90a22b6875c6a1f8ULL << 64) | 0x8e91aeba609c8876ULL },
    { 1, -2, ((unsigned __int128)0x8dfccb1ad35ca6efULL << 64) | 0x9947bdb6ddcaf59aULL },
    { 1, -2, ((unsigned __int128)0x8b5ae65d67db9acfULL << 64) | 0x7ba5168126a58b99ULL },
    { 1, -2, ((unsigned __int128)0x88bc74113f23def3ULL << 64) | 0xbc5a0fe396f40f1cULL },
    { 1, -2, ((unsigned __int128)0x86216b3b0b17188cULL << 64) | 0x363ceae88f720f1dULL },
    { 1, -2, ((unsigned __int128)0x8389c3026ac3139dULL << 64) | 0x6adda9d2270fa1f3ULL },
    { 1, -2, ((unsigned __int128)0x80f572b1363487bcULL << 64) | 0xedbd0b5b3479d5f2ULL },
    { 1, -3, ((unsigned __int128)0xfcc8e3659d9bcbf1ULL << 64) | 0x8a0cdf301431b60bULL },
    { 1, -3, ((unsigned __int128)0xf7ad6f26e7ff2efcULL << 64) | 0x9cd2238f75f969adULL },
    { 1, -3, ((unsigned __int128)0xf29877ff38809097ULL << 64) | 0x2b020fa1820c948dULL },
    { 1, -3, ((unsigned __int128)0xed89ed86a44a01abULL << 64) | 0x09d49f96cb88317aULL },
    { 1, -3, ((unsigned __int128)0xe881bf932af3dac3ULL << 64) | 0x2524848e3443e03fULL },
    { 1, -3, ((unsigned __int128)0xe37fde37807b84e3ULL << 64) | 0x5e9a750b6b68781cULL },
    { 1, -3, ((unsigned __int128)0xde8439c1dec5687cULL << 64) | 0x9d57da945b5d0aa6ULL },
    { 1, -3, ((unsigned __int128)0xd98ec2bade71e53eULL << 64) | 0xd0a98f2ad65bee96ULL },
    { 1, -3, ((unsigned __int128)0xd49f69e456cf1b7aULL << 64) | 0x5f53bd2e406e66e7ULL },
    { 1, -3, ((unsigned __int128)0xcfb6203844b3209bULL << 64) | 0x18cb02f33f79c16bULL },
    { 1, -3, ((unsigned __int128)0xcad2d6e7b80bf915ULL << 64) | 0xcc507fb7a3d0bf69ULL },
    { 1, -3, ((unsigned __int128)0xc5f57f59c7f46156ULL << 64) | 0x9a8b6997a402bf30ULL },
    { 1, -3, ((unsigned __int128)0xc11e0b2a8d1e0de1ULL << 64) | 0xda631e830fd308feULL },
    { 1, -3, ((unsigned __int128)0xbc4c6c2a226399f6ULL << 64) | 0x276ebcfb2016a433ULL },
    { 1, -3, ((unsigned __int128)0xb780945bab55dceaULL << 64) | 0xb4c7bc3d32750fd9ULL },
    { 1, -3, ((unsigned __int128)0xb2ba75f46099cf8fULL << 64) | 0x243c2e77904afa76ULL },
    { 1, -3, ((unsigned __int128)0xadfa035aa1ed8fddULL << 64) | 0x549767e410316d2bULL },
    { 1, -3, ((unsigned __int128)0xa93f2f250dac67d5ULL << 64) | 0x9ad2fb8d48054addULL },
    { 1, -3, ((unsigned __int128)0xa489ec199dab06f4ULL << 64) | 0x59fb6cf0ecb411b7ULL },
    { 1, -3, ((unsigned __int128)0x9fda2d2cc9465c52ULL << 64) | 0x6b2b9565f5355180ULL },
    { 1, -3, ((unsigned __int128)0x9b2fe580ac80b182ULL << 64) | 0x011a5b944aca8705ULL },
    { 1, -3, ((unsigned __int128)0x968b08643409ceb9ULL << 64) | 0xd5c0da506a088482ULL },
    { 1, -3, ((unsigned __int128)0x91eb89524e100d28ULL << 64) | 0xbfd3df5c52d67e77ULL },
    { 1, -3, ((unsigned __int128)0x8d515bf11fb94f22ULL << 64) | 0xa0713268840cbcbbULL },
    { 1, -3, ((unsigned __int128)0x88bc74113f23def7ULL << 64) | 0x9c5a0fe396f40f19ULL },
    { 1, -3, ((unsigned __int128)0x842cc5acf1d0344bULL << 64) | 0x6fecdfa819b96092ULL },
    { 1, -4, ((unsigned __int128)0xff4489cedeab2ca6ULL << 64) | 0xe17bd40d8d9291ecULL },
    { 1, -4, ((unsigned __int128)0xf639cc185088fe62ULL << 64) | 0x5066e87f2c0f733dULL },
    { 1, -4, ((unsigned __int128)0xed393b1c22351281ULL << 64) | 0xff4e2e660317d55fULL },
    { 1, -4, ((unsigned __int128)0xe442c00de2591b4cULL << 64) | 0xe96ab34ce0bccd10ULL },
    { 1, -4, ((unsigned __int128)0xdb56446d6ad8df09ULL << 64) | 0x28112e35a60e636fULL },
    { 1, -4, ((unsigned __int128)0xd273b2058de1bd4bULL << 64) | 0x36bbf837b4d320c6ULL },
    { 1, -4, ((unsigned __int128)0xc99af2eaca4c457bULL << 64) | 0xeaf51f66692844b2ULL },
    { 1, -4, ((unsigned __int128)0xc0cbf17a071f80e9ULL << 64) | 0x396ffdf76a147cc2ULL },
    { 1, -4, ((unsigned __int128)0xb8069857560707a7ULL << 64) | 0x0a677b4c8bec22e0ULL },
    { 1, -4, ((unsigned __int128)0xaf4ad26cbc8e5befULL << 64) | 0x9e8b8b88a14ff0c9ULL },
    { 1, -4, ((unsigned __int128)0xa6988ae903f562f1ULL << 64) | 0x7e858f08597b3a68ULL },
    { 1, -4, ((unsigned __int128)0x9defad3e8f732186ULL << 64) | 0x476d3b5b45f6ca02ULL },
    { 1, -4, ((unsigned __int128)0x9550252238bd2468ULL << 64) | 0x658e5a0b811c596dULL },
    { 1, -4, ((unsigned __int128)0x8cb9de8a32ab3694ULL << 64) | 0x97c9859530a4514cULL },
    { 1, -4, ((unsigned __int128)0x842cc5acf1d0344cULL << 64) | 0x1fecdfa819b96094ULL },
    { 1, -5, ((unsigned __int128)0xf7518e0035c3dd92ULL << 64) | 0x606d89093278a931ULL },
    { 1, -5, ((unsigned __int128)0xe65b9e6eed965c4fULL << 64) | 0x609f5fe2058d5ff2ULL },
    { 1, -5, ((unsigned __int128)0xd5779687d887e0eeULL << 64) | 0x49dda17056e45ebbULL },
    { 1, -5, ((unsigned __int128)0xc4a550a4fd9a19bbULL << 64) | 0x3e97660a23cc5402ULL },
    { 1, -5, ((unsigned __int128)0xb3e4a796a5dac213ULL << 64) | 0x07cca0bcc06c2f8eULL },
    { 1, -5, ((unsigned __int128)0xa33576a16f1f4c79ULL << 64) | 0x121016bd904dc95aULL },
    { 1, -5, ((unsigned __int128)0x9297997c68c1f4e6ULL << 64) | 0x610db3d4dd423bc9ULL },
    { 1, -5, ((unsigned __int128)0x820aec4f3a222397ULL << 64) | 0xb9e3aea6c444eef6ULL },
    { 1, -6, ((unsigned __int128)0xe31e9760a5578c6dULL << 64) | 0xf9eb2f284f31c35aULL },
    { 1, -6, ((unsigned __int128)0xc24929464655f482ULL << 64) | 0xda5f3cc0b3251da6ULL },
    { 1, -6, ((unsigned __int128)0xa195492cc0660519ULL << 64) | 0x4a18dff7cdb4ae33ULL },
    { 1, -6, ((unsigned __int128)0x8102b2c49ac23a86ULL << 64) | 0x91d082dce3ddcd08ULL },
    { 1, -7, ((unsigned __int128)0xc122451c45155150ULL << 64) | 0xb16137f09a002b0eULL },
    { 1, -7, ((unsigned __int128)0x8080abac46f389c4ULL << 64) | 0x662d417ced0079c9ULL },
    { 0, 127, ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, 127, ((unsigned __int128)0x0000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, -9, ((unsigned __int128)0xff805515885e014eULL << 64) | 0x435ab4da6a5bb50fULL },
    { 0, -8, ((unsigned __int128)0xff015358833c4762ULL << 64) | 0xbb481c8ee1416999ULL },
    { 0, -7, ((unsigned __int128)0xbee23afc0853b6a8ULL << 64) | 0xa89782c20df350c2ULL },
    { 0, -7, ((unsigned __int128)0xfe054587e01f1e2bULL << 64) | 0xf6d3a69bd5eab72fULL },
    { 0, -6, ((unsigned __int128)0x9e75221a352ba751ULL << 64) | 0x452b7ea62f2198eaULL },
    { 0, -6, ((unsigned __int128)0xbdc8d83ead88d518ULL << 64) | 0x7faa638b5e00ee90ULL },
    { 0, -6, ((unsigned __int128)0xdcfe013d7c8cbfc5ULL << 64) | 0x632dbac46f30d009ULL },
    { 0, -6, ((unsigned __int128)0xfc14d873c1980236ULL << 64) | 0xc7e09e3de453f5fcULL },
    { 0, -5, ((unsigned __int128)0x8d86cc491ecbfe03ULL << 64) | 0xf1776453b7e82558ULL },
    { 0, -5, ((unsigned __int128)0x9cf43dcff5eafd2fULL << 64) | 0x2ad90155c8a7236aULL },
    { 0, -5, ((unsigned __int128)0xac52dd7e4726a456ULL << 64) | 0xa47a963a91bb3018ULL },
    { 0, -5, ((unsigned __int128)0xbba2c7b196e7e224ULL << 64) | 0xe7950f7252c163cfULL },
    { 0, -5, ((unsigned __int128)0xcae41876471f5bdeULL << 64) | 0x91d00a417e330f8eULL },
    { 0, -5, ((unsigned __int128)0xda16eb88cb8df5fbULL << 64) | 0x28a63ecfb66e94c0ULL },
    { 0, -5, ((unsigned __int128)0xe93b5c56d85a9083ULL << 64) | 0xce2992bfea38e76bULL },
    { 0, -5, ((unsigned __int128)0xf85186008b1532f9ULL << 64) | 0xe64b8b7759978998ULL },
    { 0, -4, ((unsigned __int128)0x83acc1acc7238978ULL << 64) | 0x5a5333c45b7f442eULL },
    { 0, -4, ((unsigned __int128)0x8b29b7751bd7073bULL << 64) | 0x02e0b9ee992f2372ULL },
    { 0, -4, ((unsigned __int128)0x929fb17850a0b7beULL << 64) | 0x5b4d3807660516a4ULL },
    { 0, -4, ((unsigned __int128)0x9a0ebcb0de8e848eULL << 64) | 0x2c1bb082689ba814ULL },
    { 0, -4, ((unsigned __int128)0xa176e5f5323781d2ULL << 64) | 0xdcf935996c92e8d4ULL },
    { 0, -4, ((unsigned __int128)0xa8d839f830c1fb40ULL << 64) | 0x4c7343517c8ac264ULL },
    { 0, -4, ((unsigned __int128)0xb032c549ba861d83ULL << 64) | 0x774e27bc92ce3373ULL },
    { 0, -4, ((unsigned __int128)0xb78694572b5a5cd3ULL << 64) | 0x24cdcf68cdb2067cULL },
    { 0, -4, ((unsigned __int128)0xbed3b36bd8966419ULL << 64) | 0x7c0644d7d9ed08b4ULL },
    { 0, -4, ((unsigned __int128)0xc61a2eb18cd907a1ULL << 64) | 0xe5a1532f6d5a1ac1ULL },
    { 0, -4, ((unsigned __int128)0xcd5a1231019d66d7ULL << 64) | 0x761e3e7b171e44b2ULL },
    { 0, -4, ((unsigned __int128)0xd49369d256ab1b1fULL << 64) | 0x9e9154e1d5263cdaULL },
    { 0, -4, ((unsigned __int128)0xdbc6415d876d0839ULL << 64) | 0x3e33c0c9f8824f54ULL },
    { 0, -4, ((unsigned __int128)0xe2f2a47ade3a18a8ULL << 64) | 0xa0bf7c0b0d8bb4efULL },
    { 0, -4, ((unsigned __int128)0xea189eb3659aeaebULL << 64) | 0x93b2a3b21f448259ULL },
    { 0, -4, ((unsigned __int128)0xf1383b7157972f48ULL << 64) | 0x543fff0ff4f0aaf1ULL },
    { 0, -4, ((unsigned __int128)0xf85186008b153302ULL << 64) | 0x5e4b8b7759978993ULL },
    { 0, -4, ((unsigned __int128)0xff64898edf55d548ULL << 64) | 0x428ccfc99271dffaULL },
    { 0, -3, ((unsigned __int128)0x8338a89652cb714aULL << 64) | 0xb247eb86498c2ce7ULL },
    { 0, -3, ((unsigned __int128)0x86bbf3e68472cb2fULL << 64) | 0x0b8bd20615747126ULL },
    { 0, -3, ((unsigned __int128)0x8a3c2c233a156341ULL << 64) | 0x9027c74fe0e6f64fULL },
    { 0, -3, ((unsigned __int128)0x8db956a97b3d0143ULL << 64) | 0xf023472cd739f9e1ULL },
    { 0, -3, ((unsigned __int128)0x913378c852d65be6ULL << 64) | 0x977e3013d10f7525ULL },
    { 0, -3, ((unsigned __int128)0x94aa97c0ffa91a5dULL << 64) | 0x4ee3880fb7d34429ULL },
    { 0, -3, ((unsigned __int128)0x981eb8c723fe97f2ULL << 64) | 0x1f1c134fb702d433ULL },
    { 0, -3, ((unsigned __int128)0x9b8fe100f47ba1d8ULL << 64) | 0x04b62af189fcba0dULL },
    { 0, -3, ((unsigned __int128)0x9efe158766314e4fULL << 64) | 0x4d71827efe892fc8ULL },
    { 0, -3, ((unsigned __int128)0xa2695b665be8f338ULL << 64) | 0x4eca87c3f0f06211ULL },
    { 0, -3, ((unsigned __int128)0xa5d1b79cd2af2acaULL << 64) | 0x8837986ceabfbed6ULL },
    { 0, -3, ((unsigned __int128)0xa9372f1d0da1bd10ULL << 64) | 0x580eb71e58cd36e5ULL },
    { 0, -3, ((unsigned __int128)0xac99c6ccc1042e94ULL << 64) | 0x3dd557528315838dULL },
    { 0, -3, ((unsigned __int128)0xaff983853c9e9e40ULL << 64) | 0x5f105039091dd7f5ULL },
    { 0, -3, ((unsigned __int128)0xb3566a13956a86f4ULL << 64) | 0x471b1e1574d9fd55ULL },
    { 0, -3, ((unsigned __int128)0xb6b07f38ce90e463ULL << 64) | 0x7bb2e265d0de37e1ULL },
    { 0, -3, ((unsigned __int128)0xba07c7aa01bd2648ULL << 64) | 0x43f9d57b324bd05fULL },
    { 0, -3, ((unsigned __int128)0xbd5c481086c848dbULL << 64) | 0xbb596b5030403242ULL },
    { 0, -3, ((unsigned __int128)0xc0ae050a1abf56adULL << 64) | 0x2f7f8c5fa9c50d76ULL },
    { 0, -3, ((unsigned __int128)0xc3fd03290648847dULL << 64) | 0x30480bee4cbbd698ULL },
    { 0, -3, ((unsigned __int128)0xc74946f4436a054eULL << 64) | 0xf4f5cb531201c0d3ULL },
    { 0, -3, ((unsigned __int128)0xca92d4e7a2b5a3adULL << 64) | 0xc983a9c5c4b3b135ULL },
    { 0, -3, ((unsigned __int128)0xcdd9b173efdc1aaaULL << 64) | 0x8863e007c184a1e7ULL },
    { 0, -3, ((unsigned __int128)0xd11de0ff15ab18c6ULL << 64) | 0xd88d83d4cc613f21ULL },
    { 0, -3, ((unsigned __int128)0xd45f67e44178c612ULL << 64) | 0x5486e73c615158b4ULL },
    { 0, -3, ((unsigned __int128)0xd79e4a7405ff96c3ULL << 64) | 0x1300c9be67ae5da0ULL },
    { 0, -3, ((unsigned __int128)0xdada8cf47dad236dULL << 64) | 0xdffb833c3409ee7eULL },
    { 0, -3, ((unsigned __int128)0xde1433a16c66b14cULL << 64) | 0xde744870f54f0f18ULL },
    { 0, -3, ((unsigned __int128)0xe14b42ac60c60512ULL << 64) | 0x4e38eb8092a01f06ULL },
    { 0, -3, ((unsigned __int128)0xe47fbe3cd4d10d5bULL << 64) | 0x2ec0f797fdcd125cULL },
    { 0, -3, ((unsigned __int128)0xe7b1aa704e2ee240ULL << 64) | 0xb40faab6d2ad0841ULL },
    { 0, -3, ((unsigned __int128)0xeae10b5a7ddc8ad8ULL << 64) | 0x806b2fc9a8038790ULL },
    { 0, -3, ((unsigned __int128)0xee0de5055f63eb01ULL << 64) | 0x90a33316df83ba5aULL },
    { 0, -3, ((unsigned __int128)0xf1383b7157972f4aULL << 64) | 0xb43fff0ff4f0aaf1ULL },
    { 0, -3, ((unsigned __int128)0xf460129552d2ff41ULL << 64) | 0xe62e3201bb2bbdceULL },
    { 0, -3, ((unsigned __int128)0xf7856e5ee2c9b28aULL << 64) | 0x76f2a1b84190a7dcULL },
    { 0, -3, ((unsigned __int128)0xfaa852b25bd9b833ULL << 64) | 0xa6dbfa03186e0666ULL },
    { 0, -3, ((unsigned __int128)0xfdc8c36af1f15468ULL << 64) | 0x0a3361bca696504aULL },
    { 0, -2, ((unsigned __int128)0x8073622d6a80e631ULL << 64) | 0xe897009015316073ULL },
    { 0, -2, ((unsigned __int128)0x82012ca5a68206d5ULL << 64) | 0x8fde85afdd2bc88aULL },
    { 0, -2, ((unsigned __int128)0x838dc2fe6ac868e7ULL << 64) | 0x1a3fcbdef40100cbULL },
    { 0, -2, ((unsigned __int128)0x851927139c871af8ULL << 64) | 0x67bd00c38061c51fULL },
    { 0, -2, ((unsigned __int128)0x86a35abcd5ba5901ULL << 64) | 0x5481c3cbd925ccd2ULL },
    { 0, -2, ((unsigned __int128)0x882c5fcd7256a8c1ULL << 64) | 0x39055a6598e7c29eULL },
    { 0, -2, ((unsigned __int128)0x89b438149d4582f5ULL << 64) | 0x34531dba493eb5a6ULL },
    { 0, -2, ((unsigned __int128)0x8b3ae55d5d30701aULL << 64) | 0xc63eab8837170480ULL },
    { 0, -2, ((unsigned __int128)0x8cc0696ea11b7b36ULL << 64) | 0x94361c9a28d38a6aULL },
    { 0, -2, ((unsigned __int128)0x8e44c60b4ccfd7dcULL << 64) | 0x1473aa01c7778679ULL },
    { 0, -2, ((unsigned __int128)0x8fc7fcf24517946aULL << 64) | 0x380cbe769f2c6793ULL },
    { 0, -2, ((unsigned __int128)0x914a0fde7bcb2d0eULL << 64) | 0xc429ed3aea197a60ULL },
    { 0, -2, ((unsigned __int128)0x92cb0086fbb1cf75ULL << 64) | 0xa29d47c50b1182d0ULL },
    { 0, -2, ((unsigned __int128)0x944ad09ef4351af1ULL << 64) | 0xa49827e081cb16baULL },
    { 0, -2, ((unsigned __int128)0x95c981d5c4e924eaULL << 64) | 0x45404f5aa577d6b4ULL },
    { 0, -2, ((unsigned __int128)0x974715d708e984ddULL << 64) | 0x6648d42840d9e6fbULL },
    { 0, -2, ((unsigned __int128)0x98c38e4aa20c27d2ULL << 64) | 0x846767ec990d7333ULL },
    { 0, -2, ((unsigned __int128)0x9a3eecd4c3eaa6aeULL << 64) | 0xdb3a7f6e6087b947ULL },
    { 0, -2, ((unsigned __int128)0x9bb93315fec2d790ULL << 64) | 0x7f589fba0865790fULL },
    { 0, -2, ((unsigned __int128)0x9d3262ab4a2f4e37ULL << 64) | 0xa1ae6ba06846fae0ULL },
    { 0, -2, ((unsigned __int128)0x9eaa7d2e0fb87c35ULL << 64) | 0xff472bc6ce648a7dULL },
    { 0, -2, ((unsigned __int128)0xa0218434353f1de4ULL << 64) | 0xd493efa632530accULL },
    { 0, -2, ((unsigned __int128)0xa197795027409daaULL << 64) | 0x1dd1d4a6df960357ULL },
    { 0, -2, ((unsigned __int128)0xa30c5e10e2f613e4ULL << 64) | 0x9bd9bd99e39a20b3ULL },
    { 0, -2, ((unsigned __int128)0xa4803402004e865cULL << 64) | 0x31cbe0e8824116cdULL },
    { 0, -2, ((unsigned __int128)0xa5f2fcabbbc506d8ULL << 64) | 0x68ca4fb7ec323d74ULL },
    { 0, -2, ((unsigned __int128)0xa764b99300134d79ULL << 64) | 0x0d04d10474301862ULL },
    { 0, -2, ((unsigned __int128)0xa8d56c396fc1684cULL << 64) | 0x01eb067d578c4756ULL },
    { 0, -2, ((unsigned __int128)0xaa45161d6e93167bULL << 64) | 0x9b081cf72249f5b2ULL },
    { 0, -2, ((unsigned __int128)0xabb3b8ba2ad362a1ULL << 64) | 0x1db6506cc17a01f5ULL },
    { 0, -2, ((unsigned __int128)0xad215587a67f0cdfULL << 64) | 0xe890422cb86b7cb1ULL },
    { 0, -2, ((unsigned __int128)0xae8dedfac04e5282ULL << 64) | 0xac707b8ffc22b3e8ULL },
    { 0, -2, ((unsigned __int128)0xaff983853c9e9e3fULL << 64) | 0xc5105039091dd7f8ULL },
    { 0, -2, ((unsigned __int128)0xb1641795ce3ca978ULL << 64) | 0xfaf915300e517393ULL },
    { 0, -2, ((unsigned __int128)0xb2cdab981f0f940bULL << 64) | 0xc857c77dc1df600fULL },
    { 0, -2, ((unsigned __int128)0xb43640f4d8a5761fULL << 64) | 0xf5f080a71c34b25dULL },
    { 0, -2, ((unsigned __int128)0xb59dd911aca1ec48ULL << 64) | 0x1d2664cf09a0c1bfULL },
    { 0, -2, ((unsigned __int128)0xb70475515d0f1c5eULL << 64) | 0x4c98c6b8be17818dULL },
    { 0, -2, ((unsigned __int128)0xb86a1713c491aeaaULL << 64) | 0xd37ee2872a6f1cd6ULL },
};

/* P_2: deg-12 minimax of log(1+z)/... evaluated by Horner (low degree last). */
static const dint_t dint_p_2_[13] = {
    { 0, -4, ((unsigned __int128)0x99df88a0430813caULL << 64) | 0xa1cffb6e966a70f6ULL },
    { 1, -4, ((unsigned __int128)0xaaa02d43f696c3e4ULL << 64) | 0x4dbe754667b6bc48ULL },
    { 0, -4, ((unsigned __int128)0xba2e7a1eaf856174ULL << 64) | 0x70e5c5a5ebbe0226ULL },
    { 1, -4, ((unsigned __int128)0xccccccb9ec017492ULL << 64) | 0xf934e28d924e76d4ULL },
    { 0, -4, ((unsigned __int128)0xe38e38e3807cfa4bULL << 64) | 0xc976e6cbd22e203fULL },
    { 1, -4, ((unsigned __int128)0xfffffffffff924ccULL << 64) | 0x05b308e39fa7dfb5ULL },
    { 0, -3, ((unsigned __int128)0x924924924924911dULL << 64) | 0x862bc3d33abb3649ULL },
    { 1, -3, ((unsigned __int128)0xaaaaaaaaaaaaaaaaULL << 64) | 0x6637fd4b19743eecULL },
    { 0, -3, ((unsigned __int128)0xccccccccccccccccULL << 64) | 0xccc2ca18b08fe343ULL },
    { 1, -3, ((unsigned __int128)0xffffffffffffffffULL << 64) | 0xffffff2245823ae0ULL },
    { 0, -2, ((unsigned __int128)0xaaaaaaaaaaaaaaaaULL << 64) | 0xaaaaaaaaa5c48b54ULL },
    { 1, -2, ((unsigned __int128)0xffffffffffffffffULL << 64) | 0xffffffffffffebd8ULL },
    { 0, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
};

static const dint_t dint_log2_ = { 0, -1, ((unsigned __int128)0xb17217f7d1cf79abULL << 64) | 0xc9e3b39803f2f6afULL };
static const dint_t dint_m_one_ = { 1, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL };

/* log2(e) = 1/ln(2) and log10(e) = 1/ln(10) to full 128-bit precision, for the
 * log2/log10 accurate paths (CORE-MATH's LOG2_INV is a differently-scaled
 * constant, so these were computed and verified independently). */
static const dint_t dint_log2e_ = { 0, 0, ((unsigned __int128)0xb8aa3b295c17f0bbULL << 64) | 0xbe87fed0691d3e89ULL };
static const dint_t dint_log10e_ = { 0, -2, ((unsigned __int128)0xde5bd8a937287195ULL << 64) | 0x355baaafad33dc32ULL };

/* Degree-12 Horner evaluation of the accurate-path polynomial (port of p_2). */
static inline dint_t dint_p_(const dint_t *z)
{
    dint_t r = dint_p_2_[0];

    for (int k = 1; k < 13; ++k) {
        r = dint_mul_(z, &r);
        r = dint_add_(&dint_p_2_[k], &r);
    }
    return dint_mul_(z, &r);
}

/* Accurate ln(x) for a finite positive x (port of log_2 + dint_fromd). */
static inline dint_t dint_ln_(dint_t x)
{
    int64_t e = x.ex;
    int i = (int)(x.m >> (64 + 55)); /* x->hi >> 55 */

    /* sqrt2 split: if the leading word exceeds sqrt2 (scaled), bump exponent. */
    if ((uint64_t)(x.m >> 64) > 0xb504f333f9de6484ULL) {
        e += 1;
        i >>= 1;
    }

    /* x reduced to [1/sqrt2, sqrt2) by adjusting only the exponent. */
    dint_t xr = { x.sgn, x.ex - e, x.m };

    dint_t z = dint_mul_(&xr, &dint_inverse_2_[i - 128]);
    z = dint_add_(&dint_m_one_, &z);

    dint_t r = dint_mul_int_(&dint_log2_, e);
    dint_t p = dint_p_(&z);
    p = dint_add_(&dint_log_inv_2_[i - 128], &p);
    return dint_add_(&p, &r);
}

#endif
