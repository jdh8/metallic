#ifndef METALLIC_KERNEL_EXP2LVLACC_H
#define METALLIC_KERNEL_EXP2LVLACC_H

/* Accurate side of the two-level 4096-grid exponential (metallic-rs
 * f64_/exp.rs: exp_poly_dd, exp_two_level_mantissa_of, exp_two_level_finish;
 * itself the shape of CORE-MATH's as_exp_accurate): the same grid tables as
 * the lean fold in exp2lvl.h, but with the reduced argument dx carried as a
 * double-double and exp(dx) evaluated by a degree-7 double-double Horner.
 * Reaches ~2^-105 relative; each caller resolves its own handful of
 * sub-2^-105 near-ties with a per-function hard-case database certified
 * against exactly this pipeline.
 *
 * This pipeline runs true fma() -- a software routine on WASM, affordable on
 * this ~2^-9-of-calls cold path -- so every operation is bit-identical to
 * the metallic-rs pipeline the databases were certified against.  Replacing
 * the dd-multiply cross terms with plain mul+add was tried and adds
 * ~2^-106-class noise, comparable to the pipeline's own budget: it flipped
 * 5 of the 65146 exp10.wc worst cases. */

#include "exp2lvl.h"
#include "../shift.h"
#include <math.h>

/* dd x dd multiply with true-fma compensation, metallic-rs's Mul verbatim. */
static inline exptab_sum_ exp2lvlacc_mul_(exptab_sum_ a, exptab_sum_ b)
{
    double hi = a.hi * b.hi;
    double lo = fma(a.hi, b.hi, -hi);

    lo = fma(a.hi, b.lo, lo);
    lo = fma(a.lo, b.hi, lo);
    return (exptab_sum_){ hi, lo };
}

/* ln(2)/4096 split into three words for the accurate base-e reduction:
 * l2h has 24 trailing zero significand bits, so t*l2h is exact for
 * |t| < 2^23; the three words sum to ln(2)/4096 to ~2^-156 and
 * ln2/4096 = l2h - l2l - l2ll (l2l, l2ll to be *added* to the residual). */
static const double exp2lvlacc_l2h_ = 0.00016922538588914904;
static const double exp2lvlacc_l2l_ = 1.0256140314162804e-14;
static const double exp2lvlacc_l2ll_ = 3.2042720746546034e-31;

/* (exp(z) - 1)/z = sum z^k/(k+1)! as double-doubles, low degree first. */
static const double exp2lvlacc_c_[7][2] = {
    { 0x1.0000000000000p+0, 0x0.0p+0 },
    { 0x1.0000000000000p-1, 0x1.712f72ecec2cfp-99 },
    { 0x1.5555555555555p-3, 0x1.5555555554d07p-57 },
    { 0x1.5555555555555p-5, 0x1.55194d28275dap-59 },
    { 0x1.1111111111111p-7, 0x1.12faa0e1c0f7bp-63 },
    { 0x1.6c16c16da6973p-10, -0x1.4ba45ab25d2a3p-64 },
    { 0x1.a01a019eb7f31p-13, -0x1.9091d845ecd36p-67 },
};

/* Degree-7 double-double Horner of exp2lvlacc_c_: P(z) = (exp(z) - 1)/z for
 * z = dx a double-double, |dx| <= ln2/8192.  Mirrors CORE-MATH's opolydd --
 * each step multiplies the accumulator by dx then folds in the next
 * coefficient with a renormalizing Fast2Sum on the high words, keeping the
 * low word meaningful at ~2^-107. */
static inline exptab_sum_ exp2lvlacc_poly_(exptab_sum_ dx)
{
    exptab_sum_ acc = { exp2lvlacc_c_[6][0], exp2lvlacc_c_[6][1] };

    for (int k = 5; k >= 0; --k) {
        acc = exp2lvlacc_mul_(dx, acc);
        exptab_sum_ s = exptab_fast2sum_(exp2lvlacc_c_[k][0], acc.hi);
        acc = (exptab_sum_){ s.hi, acc.lo + (s.lo + exp2lvlacc_c_[k][1]) };
    }

    return acc;
}

/* The un-normalized accurate mantissa 2^(j/4096)*exp(dx) and exponent
 * ie = jt >> 12, with dx a double-double, |dx| <= ln2/8192. */
static inline exptab_sum_ exp2lvlacc_mantissa_(int64_t jt, exptab_sum_ dx, int64_t *ie)
{
    int j = (int)(jt & 4095);
    int i0 = j >> 6, i1 = j & 63;
    *ie = jt >> 12;

    exptab_sum_ em1 = exp2lvlacc_mul_(dx, exp2lvlacc_poly_(dx));
    exptab_sum_ tab = exp2lvlacc_mul_(
        (exptab_sum_){ exp2lvl_t0_[i0][0], exp2lvl_t0_[i0][1] },
        (exptab_sum_){ exp2lvl_t1_[i1][0], exp2lvl_t1_[i1][1] });

    if (tab.hi == 1.0 && tab.lo == 0.0) {
        /* x near a multiple of ln2 (table = 1): the mantissa is
         * 1 + (exp(dx) - 1), a catastrophic near-1 sum.  Build it with an
         * extra Fast2Sum so the low word keeps the full residual, then
         * round-to-odd nudge it off any exact power-of-two tie (toward the
         * discarded residual's sign) so the final hi + lo rounds correctly
         * -- CORE-MATH's th == 1 branch. */
        exptab_sum_ s = exptab_fast2sum_(1.0, em1.hi);
        exptab_sum_ pair = exptab_fast2sum_(s.lo, em1.lo);
        double fl = pair.hi, e2 = pair.lo;
        uint64_t fb = reinterpret(uint64_t, fl);

        if ((fb & (~0ULL >> 12)) == 0 && e2 != 0.0) {
            int64_t step = (fb ^ reinterpret(uint64_t, e2)) >> 63 ? -1 : 1;
            fl = reinterpret(double, fb + (uint64_t)step);
        }

        return (exptab_sum_){ s.hi, fl };
    }

    /* table + (exp(dx) - 1)*table, Fast2Sum on the highs (table dominates) */
    exptab_sum_ prod = exp2lvlacc_mul_(em1, tab);
    exptab_sum_ s = exptab_fast2sum_(tab.hi, prod.hi);
    return (exptab_sum_){ s.hi, s.lo + (tab.lo + prod.lo) };
}

/* Round mantissa * 2^n to the nearest double, safe across the subnormal
 * range; the mantissa pair must sit in [1, 2) (port of metallic-rs
 * round_general64). */
static inline double exp2lvlacc_round_(exptab_sum_ m, int64_t n)
{
    if (n >= -1022)
        return shift_(m.hi + m.lo, n); /* exact 2^n scaling, one rounding */

    /* Subnormal result: rounding the pair and then scaling would round
     * twice.  Round the double-double on the 2^-1074 grid instead: scale
     * both words (exact; a zero low word turns into a <= 2^-972 phantom
     * that the rounding discards), round the high word, then correct with
     * the exact residual. */
    int64_t shift = n + 1074;
    double hi = shift_(m.hi, shift);
    double lo = shift_(m.lo, shift);
    double n0 = rint(hi);
    double nn = n0 + rint((hi - n0) + lo);

    return nn * 0x1p-1074;
}

/* Shared reconstruction tail of the accurate paths: 2^(jt/4096)*exp(dx),
 * correctly rounded (subnormal-safe) up to the caller's hard-case database. */
static inline double exp2lvlacc_finish_(int64_t jt, exptab_sum_ dx)
{
    int64_t ie;
    exptab_sum_ m = exp2lvlacc_mantissa_(jt, dx, &ie);

    /* Normalize the mantissa into [1, 2), folding its exponent into ie. */
    if (m.hi < 1.0) {
        m.hi *= 2;
        m.lo *= 2;
        --ie;
    } else if (m.hi >= 2.0) {
        m.hi *= 0.5;
        m.lo *= 0.5;
        ++ie;
    }

    return exp2lvlacc_round_(m, ie);
}

#endif
