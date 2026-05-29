#include "kernel/asin.h"
#include <math.h>

/* Fast-path error bound: the intermediate argument y = x/sqrt(1-x^2) is
 * computed from correctly-rounded operations, so it can differ from the
 * exact x/sqrt(1-x^2) by up to ~1 ulp.  This propagates to ~1 ulp of
 * fast-path error (since d(atan)/dy = 1/(1+y^2) ≤ 1).  Using a relative
 * bound of 2 ulp of r.hi (= r.hi * 2^-51) ensures the Ziv test catches
 * any fast-path error up to 2 ulp regardless of the result magnitude. */
#define ASIN_ERR(r_hi) ((r_hi) * 0x1p-51)

static double asin_accurate_(double x)
{
    dint_t r = asin_dint_(fabs(x));
    return copysign(dint_to_f64_(&r), x);
}

double asin(double x)
{
    if (x != x)
        return x;

    double ax = fabs(x);

    /* Domain: [-1, 1].  |x| > 1 is a domain error returning NaN. */
    if (ax > 1.0)
        return (x - x) / (x - x);  /* NaN */

    /* asin(+-1) = +-pi/2 (exact double-double pi/2 hi word) */
    if (ax == 1.0)
        return copysign(atantab_pi_2_hi_ + atantab_pi_2_lo_, x);

    /* asin(+-0) = +-0; also handles the case |x| < 2^-27 where
     * |asin(x) - x| < x^3/6 < (2^-27)^3/6 < 2^-82 < 0.5 ulp(x). */
    if (ax < 0x1p-27)
        return x;

    exptab_sum_ r = asin_dd_(ax);
    double err   = ASIN_ERR(r.hi);
    double left  = r.hi + (r.lo - err);
    double right = r.hi + (r.lo + err);

    if (left != right)
        return asin_accurate_(x);

    return copysign(left, x);
}
