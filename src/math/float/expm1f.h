#ifndef METALLIC_EXPM1F_H
#define METALLIC_EXPM1F_H

#include "kernel/expf.h"
#include "../double/shift.h"
#include <math.h>

static double expm1f_(float x)
{
    /* Compare in double: a float threshold for `maximum` rounds up to
     * 0x1.62e43p+9, letting that input slip past into the shift_ path where the
     * exponent (n = 1024) overflows to NaN.  expf_ compares in double for the
     * same reason. */
    const double minimum = -37.42994775023704671;
    const double maximum = 709.78271289338399684;

    const float log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    if (x < minimum)
        return -1;

    if (x > maximum)
        return x * HUGE_VAL;

    float n = rintf(x * log2e) + 0;
    double y = kernel_expf_(x - n * ln2);

    if (n == 0)
        return y;

    return shift_(y + 1, n) - 1;
}
#endif
