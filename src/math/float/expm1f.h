#ifndef METALLIC_EXPM1F_H
#define METALLIC_EXPM1F_H

#include "kernel/expf.h"
#include "../double/shift.h"
#include <math.h>

static double _expm1f(float x)
{
    const float minimum = -37.42994775023704671;
    const float maximum = 709.78271289338399684;

    const float log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    if (x < minimum)
        return -1;

    if (x > maximum)
        return x * HUGE_VAL;

    float n = rintf(x * log2e) + 0;
    double y = _kernel_expf(x - n * ln2);

    if (n == 0)
        return y;

    return _shift(y + 1, n) - 1;
}
#endif
