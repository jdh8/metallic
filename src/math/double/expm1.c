#include "kernel/exptab.h"
#include "kernel/expm1tab.h"
#include <math.h>
#include <stdint.h>

/* Coefficients for S(x) = (exp(x) − 1) / x = ∑ xᵏ/(k+1)!, k=0..9,
 * stored as double-double pairs for Horner evaluation. */
static const double expm1_s_[10][2] = {
    { 1.0, 0.0 },
    { 0.5, 0.0 },
    { 0.16666666666666666, 9.25185853854297e-18 },
    { 0.041666666666666664, 2.3129646346357427e-18 },
    { 0.008333333333333333, 1.1564823173178714e-19 },
    { 0.001388888888888889, -5.300543954373577e-20 },
    { 0.0001984126984126984, 1.7209558293420705e-22 },
    { 2.48015873015873e-05, 2.1511947866775882e-23 },
    { 2.7557319223985893e-06, -1.858393274046472e-22 },
    { 2.755731922398589e-07, 2.3767714622250297e-23 },
};

/* Exact half-ULP for the rounding decision at `fast`.
 * When fast is exactly ±2^k (mantissa bits all zero) and the true value e
 * is above fast, the rounding boundary sits in the NEXT binade (smaller ULP),
 * so we halve the threshold. */
static double expm1_half_ulp_(double fast, double e)
{
    int64_t bits = reinterpret(int64_t, fabs(fast));
    int binade = (int)((bits >> 52) & 0x7FF) - 1023;
    /* At a binade boundary with true value above fast, the next representable
     * is in binade-1 with half the ULP step. */
    if ((bits & 0x000FFFFFFFFFFFFFLL) == 0 && e > 0)
        --binade;
    return shift_(0x1p-53, binade);
}

double expm1(double x)
{
    if (x != x)
        return x;

    if (x == 0)
        return x;   /* preserve sign of zero: expm1(±0) = ±0 */

    if (x >= 709.782712893384)
        return INFINITY;

    /* exp(x) < 2^-54 here, so exp(x) − 1 rounds to exactly −1.  Strict <
     * so the boundary input -0x1.2b708872320e1p+5 (where exp(x) > 2^-54
     * and expm1 rounds to -(1-2^-53)) is not prematurely returned as -1. */
    if (x < -37.42994775023704)
        return -1.0;

    double scaled = rint(x * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;

    if (n == 0) {
        /* |x| < ln2/(2N) ≈ 0.0027: evaluate expm1(x) = x · S(x) directly. */
        exptab_sum_ s = { expm1_s_[9][0], expm1_s_[9][1] };

        for (int i = 8; i >= 0; --i)
            s = exptab_muladd_(s, x, expm1_s_[i][0], expm1_s_[i][1]);

        exptab_sum_ r = exptab_prod_(s.hi, x);
        double res_lo = s.lo * x + r.lo;
        double fast = r.hi + res_lo;
        double e = (r.hi - fast) + res_lo;

        /* Exact half-ULP check.  Accuracy ≈ (|expm1(x)| + 1) * 2^-99,
         * since the kernel error is bounded by |exp(x)| * 2^-100. */
        if (fabs(e) + (fabs(fast) + 1.0) * 0x1p-99 >= expm1_half_ulp_(fast, e)) {
            double wc = expm1_wc_lookup_(x);
            if (wc) return wc;
        }
        return fast;
    }

    int j = (int)(n & (EXPTAB_N - 1));
    int64_t q = n >> 7;

    /* Three-word reduction: ~2^-100 accuracy. */
    double a = x - scaled * exptab_ln2_over_n_hi_;
    exptab_sum_ blo = exptab_prod_(scaled, exptab_ln2_over_n_lo_);
    exptab_sum_ r = exptab_twosum_(a, -blo.hi);
    r = exptab_fast2sum_(r.hi, r.lo - blo.lo - scaled * exptab_ln2_over_n_lo2_);

    /* exp(x) = 2^q · mantissa; form 2^q · mantissa − 1 as double-double. */
    exptab_sum_ mantissa = exptab_mantissa_(j, &q, r);
    double exp_hi = shift_(mantissa.hi, q);
    /* shift_(0, q) corrupts the zero bit pattern; guard explicitly. */
    double exp_lo = mantissa.lo ? shift_(mantissa.lo, q) : 0.0;

    exptab_sum_ result = exptab_twosum_(exp_hi, -1.0);
    double res_lo = result.lo + exp_lo;
    double fast = result.hi + res_lo;
    double e = (result.hi - fast) + res_lo;

    /* Exact half-ULP check.  Accuracy ≈ (|expm1(x)| + 1) * 2^-99 = |exp(x)| * 2^-99. */
    if (fabs(e) + (fabs(fast) + 1.0) * 0x1p-99 >= expm1_half_ulp_(fast, e)) {
        double wc = expm1_wc_lookup_(x);
        if (wc) return wc;
    }
    return fast;
}
