#include "kernel/exptab.h"
#include "kernel/expm1tab.h"
#include "kernel/exp2lvl.h"
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

/* U(x) = (eˣ − 1 − x)/x² = ∑ xᵏ/(k+2)!, k = 0..5, for the plain-double fast
 * leg of the small branch (ported from metallic-rs EXPM1_U_COEFFS).  Degree 5
 * over |x| < ln2/256: the x⁶/8! truncation is ≈ 2⁻⁶⁵ relative to U. */
static const double expm1_u_[6] = {
    0.5,
    0.16666666666666666,
    0.041666666666666664,
    0.008333333333333333,
    0.001388888888888889,
    0.0001984126984126984,
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

    /* expm1(x) - x = x²/2·(1 + x/3 + …) > 0 for both signs: away from zero
     * for x > 0 (coarse side, half-gap 2^(e-53) ≥ x²/2 for binade e ≤ -54)
     * and toward zero for x < 0, whose binding corner x = -2^e exact has
     * half-gap 2^(e-54) > 2^(2e-1) = x²/2 for e ≤ -54.  No tie arises, so
     * expm1(x) rounds to x.  Strict bound: at x = -2^-53 the x³/6 term would
     * be needed to break the tie.  Nearest expm1_wc_ entries (≈±2^-52) lie
     * outside.  This also skips the degree-9 double-double Horner below. */
    if (fabs(x) < 0x1p-53)
        return x;

    double scaled = rint(x * exptab_n_over_ln2_);
    int64_t n = (int64_t)scaled;

    if (n == 0) {
        /* Result-anchored plain-double fast leg: expm1(x) = x + c with the
         * correction c = x²·U(x) > 0, so the leg's error rides the tiny c
         * rather than the result.  Ziv gate eps = x²·2⁻⁴⁹ ≈ c·2⁻⁴⁸: the
         * degree-5 truncation (≈ 2⁻⁶⁵·c), the plain Horner (≈ 2⁻⁵²·c), and
         * the x·x / x²·u / c∓eps roundings (≈ 3·2⁻⁵³·c) total ≤ ~2⁻⁵⁰·⁵·c,
         * a ≥ 5× margin.  When x + (c−eps) and x + (c+eps) round identically,
         * that common double is the correct rounding of x + (expm1(x) − x);
         * otherwise fall through to the double-double Horner below. */
        double xx = x * x;
        double u = ((((expm1_u_[5] * x + expm1_u_[4]) * x + expm1_u_[3]) * x
                  + expm1_u_[2]) * x + expm1_u_[1]) * x + expm1_u_[0];
        double c = xx * u;
        double eps = xx * 0x1p-49;
        double left = x + (c - eps);
        double right = x + (c + eps);

        if (left == right)
            return left;

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

    /* Lean two-level fast leg with the −1 taken in mantissa space (ported
     * from metallic-rs expm1): exp(x) = (th + fl)·2^q on the 4096-point grid,
     * so expm1(x) = ((th − 2^−q) + fl)·2^q with off = −2^−q built by bit
     * pattern.  The three-way window mirrors CORE-MATH's cr_expm1: off leads
     * the Fast2Sum for q < 53 (dominant or exact against th — the sum is even
     * error-free for 0 ≤ q ≤ 52, both operands on the 2^−52 grid), trails it
     * for 53 ≤ q < 75 (sub-ulp but still gate-relevant), and is dropped for
     * q ≥ 75 (2^−q ≪ the gate; its bit pattern degenerates at q = 1024).
     * n ≠ 0 keeps q in [−54, 1024], so `off` is finite whenever used and
     * shift_(left, q) stays normal-exact.  The fold is ~2^−64 absolute on
     * th + fl and low = fl + s.lo adds one rounding ≤ 2^−66; the subtraction-
     * preserving absolute 2^−62 gate still bounds the leg. */
    {
        double s4 = rint(x * exp2lvl_n_over_ln2_);
        double a = x - s4 * exp2lvl_ln2_over_n_hi_;
        double dx = a - s4 * exp2lvl_ln2_over_n_lo_;
        int64_t q2;
        exptab_sum_ f = exp2lvl_fold_((int64_t)s4, dx, &q2);

        double off = reinterpret(double, (2048 + 1023 - q2) << 52);
        exptab_sum_ s = q2 < 53 ? exptab_fast2sum_(off, f.hi)
                      : q2 < 75 ? exptab_fast2sum_(f.hi, off)
                      : (exptab_sum_){ f.hi, 0 };

        double low = f.lo + s.lo;
        double left = s.hi + (low - EXP2LVL_EPS_);
        double right = s.hi + (low + EXP2LVL_EPS_);

        if (left == right)
            return shift_(left, q2);
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
