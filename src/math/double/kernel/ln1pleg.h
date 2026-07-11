#ifndef METALLIC_KERNEL_LN1PLEG_H
#define METALLIC_KERNEL_LN1PLEG_H

/* Shared legs of the log1p base-2/10 lifts (metallic-rs f64_/log.rs and
 * f64_/dint.rs): the small and mid-band result-anchored series as raw pairs
 * for the callers' Ziv gates, and the 128-bit ln(1+x) series that replaces
 * CORE-MATH's log2p1/log10p1 exception tables outright.  Working term-wise
 * in x (never forming 1 + x) sidesteps the 128-bit cancellation below
 * |x| ~ 2^-60. */

#include "exptab.h"
#include "dint.h"

/* (ln(1+x) - x)/x^2 = sum (-1)^(k+1) x^k/(k+2), low degree first; degree 7
 * over |x| < 2^-8. */
static const double ln1pleg_q_[8] = {
    -0.5,
    0.3333333333333333,
    -0.25,
    0.2,
    -0.16666666666666666,
    0.14285714285714285,
    -0.125,
    0.1111111111111111,
};

/* (ln(1+x) - x + x^2/2)/x^3 = sum (-1)^k x^k/(k+3), low degree first;
 * degree 13 over |x| < 2^-4 (truncation x^17/19 <= 2^-72 after the x^3
 * scale). */
static const double ln1pleg_f_wide_[14] = {
    0.3333333333333333,
    -0.25,
    0.2,
    -0.16666666666666666,
    0.14285714285714285,
    -0.125,
    0.1111111111111111,
    -0.1,
    0.09090909090909091,
    -0.08333333333333333,
    0.07692307692307693,
    -0.07142857142857142,
    0.06666666666666667,
    -0.0625,
};

/* The small-|x| leg (2^-54 <~ |x| < 2^-8): the exact x lead plus the
 * x^2*Q(x) correction, as the raw pair for an x^2-scaled gate. */
static inline exptab_sum_ ln1pleg_small_(double x)
{
    double q = ln1pleg_q_[7];

    for (int k = 6; k >= 0; --k)
        q = q * x + ln1pleg_q_[k];

    double tail = (x * x) * q;
    double hi = x + tail;

    return (exptab_sum_){ hi, x - hi + tail };
}

/* The mid-|x| leg (2^-8 <= |x| < 2^-4): the exact x - x^2/2 lead (Dekker
 * product; the residual joins the low word) plus the x^3*f(x) correction,
 * as the raw pair for an |x|^3-scaled gate. */
static inline exptab_sum_ ln1pleg_wide_(double x)
{
    exptab_sum_ p = exptab_prod_(-0.5 * x, x);
    double hi = x + p.hi;
    double resid = ((x - hi) + p.hi) + p.lo;
    double f = ln1pleg_f_wide_[13];

    for (int k = 12; k >= 0; --k)
        f = f * x + ln1pleg_f_wide_[k];

    return (exptab_sum_){ hi, (x * x) * x * f + resid };
}

/* 1/(j+1) as dints, j = 0..25, for the deep series. */
static const dint_t ln1pleg_recip_[26] = {
    { 0, 0, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, -1, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, -2, ((unsigned __int128)0xaaaaaaaaaaaaaaaaULL << 64) | 0xaaaaaaaaaaaaaaabULL },
    { 0, -2, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, -3, ((unsigned __int128)0xccccccccccccccccULL << 64) | 0xcccccccccccccccdULL },
    { 0, -3, ((unsigned __int128)0xaaaaaaaaaaaaaaaaULL << 64) | 0xaaaaaaaaaaaaaaabULL },
    { 0, -3, ((unsigned __int128)0x9249249249249249ULL << 64) | 0x2492492492492492ULL },
    { 0, -3, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, -4, ((unsigned __int128)0xe38e38e38e38e38eULL << 64) | 0x38e38e38e38e38e4ULL },
    { 0, -4, ((unsigned __int128)0xccccccccccccccccULL << 64) | 0xcccccccccccccccdULL },
    { 0, -4, ((unsigned __int128)0xba2e8ba2e8ba2e8bULL << 64) | 0xa2e8ba2e8ba2e8baULL },
    { 0, -4, ((unsigned __int128)0xaaaaaaaaaaaaaaaaULL << 64) | 0xaaaaaaaaaaaaaaabULL },
    { 0, -4, ((unsigned __int128)0x9d89d89d89d89d89ULL << 64) | 0xd89d89d89d89d89eULL },
    { 0, -4, ((unsigned __int128)0x9249249249249249ULL << 64) | 0x2492492492492492ULL },
    { 0, -4, ((unsigned __int128)0x8888888888888888ULL << 64) | 0x8888888888888889ULL },
    { 0, -4, ((unsigned __int128)0x8000000000000000ULL << 64) | 0x0000000000000000ULL },
    { 0, -5, ((unsigned __int128)0xf0f0f0f0f0f0f0f0ULL << 64) | 0xf0f0f0f0f0f0f0f1ULL },
    { 0, -5, ((unsigned __int128)0xe38e38e38e38e38eULL << 64) | 0x38e38e38e38e38e4ULL },
    { 0, -5, ((unsigned __int128)0xd79435e50d79435eULL << 64) | 0x50d79435e50d7943ULL },
    { 0, -5, ((unsigned __int128)0xccccccccccccccccULL << 64) | 0xcccccccccccccccdULL },
    { 0, -5, ((unsigned __int128)0xc30c30c30c30c30cULL << 64) | 0x30c30c30c30c30c3ULL },
    { 0, -5, ((unsigned __int128)0xba2e8ba2e8ba2e8bULL << 64) | 0xa2e8ba2e8ba2e8baULL },
    { 0, -5, ((unsigned __int128)0xb21642c8590b2164ULL << 64) | 0x2c8590b21642c859ULL },
    { 0, -5, ((unsigned __int128)0xaaaaaaaaaaaaaaaaULL << 64) | 0xaaaaaaaaaaaaaaabULL },
    { 0, -5, ((unsigned __int128)0xa3d70a3d70a3d70aULL << 64) | 0x3d70a3d70a3d70a4ULL },
    { 0, -5, ((unsigned __int128)0x9d89d89d89d89d89ULL << 64) | 0xd89d89d89d89d89eULL },
};

/* 128-bit ln(1+x) for |x| < 2^-4: x * sum (-x)^j/(j+1) by Horner. */
static inline dint_t ln1pleg_series_(double x)
{
    dint_t t = dint_from_f64_(x);
    dint_t u = { !t.sgn, t.ex, t.m };
    dint_t acc = ln1pleg_recip_[25];

    for (int k = 24; k >= 0; --k) {
        acc = dint_mul_(&acc, &u);
        acc = dint_add_(&acc, &ln1pleg_recip_[k]);
    }

    return dint_mul_(&t, &acc);
}

#endif
