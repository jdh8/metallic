#include "atan2f.h"
#include "log2_dd.h"
#include <complex.h>
#include <math.h>

/* ln(2)/2 as a double-double: re = 1/2 * ln(x^2+y^2) = ln2/2 * log2(x^2+y^2). */
static const sum_t clogf_half_ln2_ = { 0.34657359027997264, 1.1595234069231498e-17 };

/* Correctly-rounded real part of clog: 1/2 * log(x^2 + y^2).
 *
 * x^2 and y^2 are exact doubles (float operands), so s = x^2 + y^2 is an exact
 * double-double; carrying its low word past the logarithm is what makes |z| ~ 1
 * correctly rounded.  log2(s) = log2(s.hi) + (s.lo/s.hi)/ln2, and the 1/ln2
 * cancels the ln2/2 prefactor on the correction term. */
static float clogf_re_(float x, float y)
{
    double a = fabs((double)x);
    double b = fabs((double)y);

    if (a == INFINITY || b == INFINITY)
        return INFINITY;

    sum_t s = dd_2sum_(a * a, b * b);

    if (s.hi == 0.0)
        return -INFINITY;

    if (isnan(s.hi))
        return NAN;

    sum_t re = dd_mul_dd_(log2_dd_(s.hi), clogf_half_ln2_);
    re = dd_add_(re, (sum_t){ 0.5 * (s.lo / s.hi), 0.0 });
    return round_dd_signed_(re);
}

float _Complex clogf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    return CMPLXF(clogf_re_(x, y), atan2f_(y, x));
}
