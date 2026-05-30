/* Correctly-rounded double asinh.
 *
 * asinh(x) = ln(x + sqrt(x^2 + 1))   [odd function]
 *
 * For |x| < 2^-27: asinh(x) = x (exact, error < x^3/6 < 0.5 ulp).
 * For |x| <= 0.5:  use ln(1 + x + x^2/(1+sqrt(1+x^2))) to avoid cancellation.
 * For 0.5 < |x| < 2^511: asinh(x) = sign(x) * ln(|x| + sqrt(|x|^2+1)).
 *   - Double-double: form arg as a double-double, call logtab_ln_dd_ + correction.
 * For |x| >= 2^511: x^2 overflows; asinh(x) ≈ sign(x)*(ln(2|x|)) = sign(x)*(ln(|x|)+ln2).
 *
 * Fast path uses relative ERR (as in asin.c) to ensure Ziv test triggers correctly.
 * Accurate path: asin_sqrt_dint_ from kernel/asin.h, then dint_ln_.
 */

#include "kernel/asin.h"    /* asin_sqrt_dint_, dint machinery */
#include "kernel/logtab.h"  /* logtab_ln_dd_ */
#include <math.h>

/* Fast-path error bound: relative to r.hi (2 ULP). */
#define ASINH_ERR(r_hi) ((r_hi) * 0x1p-51)

static double asinh_accurate_(double x)
{
    double ax = fabs(x);
    dint_t axd = dint_from_f64_(ax);

    if (ax >= 0x1p511) {
        /* For very large x: asinh(x) ≈ ln(2x) = ln(x) + ln(2).
         * In dint: form 2*x, apply dint_ln_. */
        dint_t two_x = { 0, axd.ex + 1, axd.m };
        dint_t ln_r = dint_ln_(two_x);
        return copysign(dint_to_f64_(&ln_r), x);
    }

    /* General: ln(|x| + sqrt(x^2+1)). */
    dint_t x2 = dint_mul_(&axd, &axd);
    dint_t one = { 0, 0, (unsigned __int128)0x8000000000000000ULL << 64 };
    dint_t x2p1 = dint_add_(&x2, &one);
    dint_t sq = asin_sqrt_dint_(&x2p1);
    dint_t arg = dint_add_(&axd, &sq);
    dint_t ln_r = dint_ln_(arg);

    return copysign(dint_to_f64_(&ln_r), x);
}

double asinh(double x)
{
    if (x != x)
        return x;
    if (x == 0 || x == INFINITY || x == -INFINITY)
        return x;

    double ax = fabs(x);

    /* |x| < 2^-27: asinh(x) = x (error < x^3/6 < 0.5 ulp(x)). */
    if (ax < 0x1p-27)
        return x;

    exptab_sum_ r;

    if (ax >= 0x1p511) {
        /* x^2 overflows; asinh(x) ≈ ln(2x) = ln(x) + ln(2).
         * Compute logtab_ln_dd_(ax) and add ln2 as a double-double. */
        r = logtab_ln_dd_(ax);
        r = exptab_add_(r, (exptab_sum_){ logtab_ln2_hi_, logtab_ln2_lo_ });
    } else if (ax <= 0.5) {
        /* Stable for small x: arg = 1 + x + x^2/(1+sqrt(1+x^2)).
         * The result is close to 1, so we use logtab_ln_dd_ directly.
         * Correction for the double-double argument: we compute arg_hi = 1+ax+s/(1+sq),
         * arg_lo captures rounding in the argument. */
        double s   = ax * ax;
        double sq  = sqrt(1.0 + s);
        double frac = s / (1.0 + sq);
        /* arg = 1 + ax + frac, form carefully */
        double t_hi = 1.0 + ax;
        double t_lo = (ax - (t_hi - 1.0));  /* fast2sum residual */
        double arg_hi = t_hi + frac;
        double arg_lo = t_lo + (frac - (arg_hi - t_hi));  /* two-sum residual */
        r = logtab_ln_dd_(arg_hi);
        r.lo += arg_lo / arg_hi;
    } else {
        /* General: ln(ax + sqrt(ax^2+1)).
         * Form the argument as a double-double to avoid losing precision. */
        double s  = ax * ax;
        double sq = sqrt(s + 1.0);
        /* arg = ax + sq; correct the rounding in sqrt. */
        double arg_hi = ax + sq;
        /* sqrt correction: sq_true ≈ sq + (s+1 - sq^2)/(2*sq) */
        double sq_lo = (s + 1.0 - sq * sq) / (2.0 * sq);
        /* arg = ax + sq + sq_lo; arg_hi = ax + sq; arg_lo = sq_lo + (ax + sq - arg_hi) */
        double arg_lo = sq_lo + (ax - (arg_hi - sq));
        r = logtab_ln_dd_(arg_hi);
        r.lo += arg_lo / arg_hi;
    }

    double err   = ASINH_ERR(r.hi);
    double left  = r.hi + (r.lo - err);
    double right = r.hi + (r.lo + err);

    if (left != right)
        return asinh_accurate_(x);

    return copysign(left, x);
}
