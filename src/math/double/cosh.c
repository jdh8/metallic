#include "kernel/sinhtab.h"
#include <math.h>
#include <stdint.h>

double cosh(double x)
{
    x = fabs(x);

    /* ln(2·DBL_MAX) ≈ 0x1.633ce8fb9f87dp+9: above this (or NaN) propagate via multiply */
    if (!(x <= 0x1.633ce8fb9f87dp+9))
        return x * HUGE_VAL;

    int64_t q;
    exptab_sum_ mantissa = sinhtab_mantissa_(x, 1, &q);

    /* cosh(x) = 2^(q-1) · (m.hi + m.lo); result is always positive */
    return shift_(mantissa.hi + mantissa.lo, q - 1);
}
