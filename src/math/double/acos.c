#include "kernel/asin.h"
#include <math.h>

/* Fast-path relative error bound: same derivation as asin.c.  The intermediate
 * argument may be up to ~1 ulp off, giving ~2 ulp fast-path error.  Using a
 * relative bound of 2 ulp of r.hi ensures the Ziv test catches all fast-path
 * errors regardless of result magnitude. */
#define ACOS_ERR(r_hi) ((r_hi) * 0x1p-51)

static double acos_accurate_(double ax, int negative)
{
    dint_t r;

    if (negative) {
        /* acos(-ax) = pi - acos(ax) = pi - (pi/2 - asin(ax)) = pi/2 + asin(ax) */
        dint_t a = asin_dint_(ax);
        r = dint_add_(&atantab_pi_2_dint_, &a);
    } else {
        /* acos(ax) = pi/2 - asin(ax) */
        dint_t a = asin_dint_(ax);
        dint_t neg_a = { !a.sgn, a.ex, a.m };
        r = dint_add_(&atantab_pi_2_dint_, &neg_a);
    }

    return dint_to_f64_(&r);
}

double acos(double x)
{
    if (x != x)
        return x;

    double ax = fabs(x);

    /* Domain: [-1, 1].  |x| > 1 is a domain error returning NaN. */
    if (ax > 1.0)
        return (x - x) / (x - x);  /* NaN */

    /* acos(1) = +0 */
    if (x == 1.0)
        return 0.0;

    /* acos(-1) = pi */
    if (x == -1.0)
        return atantab_pi_2_hi_ + atantab_pi_2_hi_;  /* 2 * pi/2_hi is exact: pi_hi */

    /* |x| < 2^-27: acos(x) ≈ pi/2 - x + O(x^3).
     * The O(x^3) term is < (2^-27)^3/6 < 2^-82 < 0.5 ulp of the result.
     * Compute pi/2 - x as a double-double (including pi/2_lo) so the result
     * is correctly rounded for all |x| in [0, 2^-27). */
    if (ax < 0x1p-27) {
        double r_hi = atantab_pi_2_hi_ - x;
        double r_lo = (atantab_pi_2_hi_ - r_hi) - x + atantab_pi_2_lo_;
        return r_hi + r_lo;
    }

    /* General case: |x| in [2^-27, 1). */
    int negative = (x < 0.0);
    exptab_sum_ r;

    if (!negative) {
        /* acos(x) = pi/2 - asin(x), computed in double-double. */
        exptab_sum_ a = asin_dd_(ax);
        double r_hi = atantab_pi_2_hi_ - a.hi;
        double r_lo = (atantab_pi_2_hi_ - r_hi) - a.hi;  /* residual */
        r_lo += atantab_pi_2_lo_ - a.lo;
        r = (exptab_sum_){ r_hi, r_lo };
    } else {
        /* acos(-ax) = pi/2 + asin(ax), computed in double-double. */
        exptab_sum_ a = asin_dd_(ax);
        exptab_sum_ pi_2 = { atantab_pi_2_hi_, atantab_pi_2_lo_ };
        r = exptab_add_(pi_2, a);
    }

    double err   = ACOS_ERR(r.hi);
    double left  = r.hi + (r.lo - err);
    double right = r.hi + (r.lo + err);

    if (left != right)
        return acos_accurate_(ax, negative);

    return left;
}
