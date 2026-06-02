#include "dd.h"
#include <complex.h>
#include <float.h>
#include <math.h>

/* Ziv gate: is (float)v safely the correctly-rounded result?  True when v is a
 * normal-range double whose discarded low 29 bits sit > 0x2000 from a float
 * rounding midpoint, so the few-ulp fast-path error cannot cross it.  Subnormal
 * and overflow outputs fail the gate and take the double-double path. */
static inline int csqrtf_ziv_(double v)
{
    double a = fabs(v);

    if (!(a >= (double)FLT_MIN && a < 0x1p127))
        return 0;

    uint64_t bits = reinterpret(uint64_t, v);
    int64_t dist = (int64_t)(bits & UINT64_C(0x1FFFFFFF)) - INT64_C(0x10000000);
    return (dist < 0 ? -dist : dist) > 0x2000;
}

/* Correctly-rounded principal square root of a finite z = x + iy.
 *
 * The larger output magnitude is always sqrt((|z| + |x|)/2) -- no cancellation
 * for either sign of x -- and the smaller follows from the exact identity
 * re*|im| = |y|/2 as |y|/(2*big).  x and y hold float values, so x*x and y*y
 * are exact doubles and x*x + y*y is an exact double-double. */
static float _Complex csqrtf_finite_(double x, double y)
{
    double ax = fabs(x);
    double ay = fabs(y);

    /* Fast path: plain double, no FMA. */
    double m = sqrt(x * x + y * y);
    double big = sqrt(0.5 * (ax + m));
    double small = 0.5 * ay / big;

    float rbig, rsmall;

    if (csqrtf_ziv_(big) && csqrtf_ziv_(small)) {
        rbig = (float)big;
        rsmall = (float)small;
    } else {
        sum_t s = dd_2sum_(x * x, y * y);
        sum_t mm = dd_sqrt_(s);
        sum_t bigsq = dd_add_(mm, (sum_t){ ax, 0.0 });
        bigsq.hi *= 0.5;
        bigsq.lo *= 0.5;
        sum_t b = dd_sqrt_(bigsq);
        sum_t t = dd_div_((sum_t){ 0.5 * ay, 0.0 }, b);
        rbig = round_dd_general_(b);
        rsmall = round_dd_general_(t);
    }

    if (signbit(x))
        return CMPLXF(rsmall, copysignf(rbig, y));

    return CMPLXF(rbig, copysignf(rsmall, y));
}

float _Complex csqrtf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    if (z == 0)
        return CMPLXF(0, y);

    if (isinf(y))
        return CMPLXF(INFINITY, y);

    if (x == INFINITY)
        return CMPLXF(x, 0 * y);

    if (x == -INFINITY)
        return CMPLXF(y - y, copysignf(x, y));

    if (isnan(x) || isnan(y))
        return CMPLXF(NAN, NAN);

    return csqrtf_finite_(x, y);
}
