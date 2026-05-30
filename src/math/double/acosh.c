/* Correctly-rounded double acosh.
 *
 * acosh(x) = ln(x + sqrt(x^2 - 1)),  domain [1, ∞)
 *
 * For x == 1: acosh(1) = 0 (exact).
 * For 1 < x < 2: ln(x + sqrt((x-1)*(x+1))) = ln1p((x-1) + sqrt((x-1)*(2+(x-1))))
 *   computed as logtab_ln_dd_(arg_hi) + arg_lo/arg_hi with double-double arg.
 * For 2 <= x < 2^511: ln(x + sqrt(x^2-1)) via double-double with sqrt correction.
 * For x >= 2^511: x^2 overflows; acosh(x) ≈ ln(2x) = ln(x) + ln(2).
 *
 * Accurate path: asin_sqrt_dint_ from kernel/asin.h, then dint_ln_.
 */

#include "kernel/asin.h"    /* asin_sqrt_dint_, dint machinery */
#include "kernel/logtab.h"  /* logtab_ln_dd_ */
#include <math.h>

/* Fast-path error bound: relative to r.hi (2 ULP). */
#define ACOSH_ERR(r_hi) ((r_hi) * 0x1p-51)

static double acosh_accurate_(double x)
{
    dint_t xd = dint_from_f64_(x);

    if (x >= 0x1p511) {
        /* For very large x: acosh(x) ≈ ln(2x). */
        dint_t two_x = { 0, xd.ex + 1, xd.m };
        dint_t ln_r = dint_ln_(two_x);
        return dint_to_f64_(&ln_r);
    }

    if (x < 2.0) {
        dint_t neg_one = { 1, 0, (unsigned __int128)0x8000000000000000ULL << 64 };
        dint_t dm = dint_add_(&xd, &neg_one);     /* x - 1 */
        dint_t two = { 0, 1, (unsigned __int128)0x8000000000000000ULL << 64 };
        dint_t tp = dint_add_(&two, &dm);          /* 2 + (x-1) */
        dint_t prod = dint_mul_(&dm, &tp);         /* (x-1)*(2+(x-1)) = x^2-1 */
        dint_t sq = asin_sqrt_dint_(&prod);
        dint_t arg = dint_add_(&xd, &sq);
        dint_t ln_r = dint_ln_(arg);
        return dint_to_f64_(&ln_r);
    } else {
        dint_t x2 = dint_mul_(&xd, &xd);
        dint_t neg_one = { 1, 0, (unsigned __int128)0x8000000000000000ULL << 64 };
        dint_t x2m1 = dint_add_(&x2, &neg_one);
        dint_t sq = asin_sqrt_dint_(&x2m1);
        dint_t arg = dint_add_(&xd, &sq);
        dint_t ln_r = dint_ln_(arg);
        return dint_to_f64_(&ln_r);
    }
}

double acosh(double x)
{
    if (x != x)
        return x;
    if (x == INFINITY)
        return x;
    if (x < 1.0)
        return (x - x) / (x - x);  /* NaN, domain error */
    if (x == 1.0)
        return 0.0;

    exptab_sum_ r;

    if (x >= 0x1p511) {
        /* x^2 overflows; acosh(x) ≈ ln(2x) = ln(x) + ln(2). */
        r = logtab_ln_dd_(x);
        r = exptab_add_(r, (exptab_sum_){ logtab_ln2_hi_, logtab_ln2_lo_ });
    } else if (x < 2.0) {
        /* Stable for x near 1: arg = x + sqrt((x-1)*(x+1)) = x + sqrt(x^2-1).
         * Use (x-1) and 2+(x-1) to avoid cancellation. */
        double dm = x - 1.0;             /* exact for x in [1,2) */
        double sq = sqrt(dm * (2.0 + dm));
        /* arg = 1 + dm + sq = x + sqrt(x^2-1).
         * Form as double-double: t = 1 + dm, then t + sq with correction. */
        double t_hi = 1.0 + dm;         /* = x (exact) */
        double t_lo = (dm - (t_hi - 1.0));  /* fast2sum residual = 0 (exact) */
        double arg_hi = t_hi + sq;
        /* sqrt correction for better arg accuracy */
        double sq_lo = (dm * (2.0 + dm) - sq * sq) / (2.0 * sq);
        double arg_lo = t_lo + sq_lo + (t_hi - (arg_hi - sq));
        r = logtab_ln_dd_(arg_hi);
        r.lo += arg_lo / arg_hi;
    } else {
        /* General case: arg = x + sqrt(x^2 - 1). */
        double sq = sqrt(x * x - 1.0);
        double arg_hi = x + sq;
        double sq_lo = (x * x - 1.0 - sq * sq) / (2.0 * sq);
        double arg_lo = sq_lo + (x - (arg_hi - sq));
        r = logtab_ln_dd_(arg_hi);
        r.lo += arg_lo / arg_hi;
    }

    double err   = ACOSH_ERR(r.hi);
    double left  = r.hi + (r.lo - err);
    double right = r.hi + (r.lo + err);

    if (left != right)
        return acosh_accurate_(x);

    return left;
}
