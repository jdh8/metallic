#include "kernel/atantab.h"
#include <math.h>

/* Fast-path absolute error bound.  The double-double reduction accumulates
 * error from the plain-double polynomial (corr = h*h²*(c0+…)), the dd-quotient
 * h computation, and catastrophic cancellation when adding small lo to hi.
 * Empirical scanning across all exponents gives a worst-case absolute error
 * of ~2^-59.5; 2^-58 covers it with margin and keeps the Ziv fallback rare. */
#define ATAN_ERR 0x1p-58

static double atan_accurate_(double x)
{
    dint_t r = atantab_atan_dint_(fabs(x));
    double a = dint_to_f64_(&r);

    return copysign(a, x);
}

double atan(double x)
{
    if (x != x)
        return x;

    double ax = fabs(x);

    /* atan(+-0) = +-0; atan(+-inf) = +-pi/2. */
    if (ax == 0)
        return x;
    if (ax == INFINITY)
        return copysign(atantab_pi_2_hi_, x);

    /* For |x| < 2^-27, |atan(x) - x| < x^3/3 < (2^-27)^3/3 = 2^-81/3 < 2^-82.
     * For x in [2^-28, 2^-27), ulp(x) = 2^-80, so the error is < 2^-82 < 0.5 ulp. */
    if (ax < 0x1p-27)
        return x;

    exptab_sum_ r = atantab_atan_dd_(ax);
    double left = r.hi + (r.lo - ATAN_ERR);
    double right = r.hi + (r.lo + ATAN_ERR);

    if (left != right)
        return atan_accurate_(x);

    return copysign(left, x);
}
