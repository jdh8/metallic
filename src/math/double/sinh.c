#include "kernel/sinhtab.h"
#include <math.h>
#include <stdint.h>

double sinh(double x)
{
    double s = fabs(x);

    /* ln(2·DBL_MAX) ≈ 0x1.633ce8fb9f87dp+9: above this (or NaN) propagate via multiply */
    if (!(s <= 0x1.633ce8fb9f87dp+9))
        return x * HUGE_VAL;

    /* For |x| <= 2^-26, sinh(x) rounds to exactly x (cubic term x³/6 < 0.5 ulp) */
    if (s < 1.4901161193847656e-8)
        return x;

    int64_t q;
    exptab_sum_ mantissa = sinhtab_mantissa_(s, 0, &q);

    /* sinh(x) = 2^(q-1) · (m.hi + m.lo), with the sign of x */
    return copysign(shift_(mantissa.hi + mantissa.lo, q - 1), x);
}
