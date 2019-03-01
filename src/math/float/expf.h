#ifndef METALLIC_PREC_EXPF_H
#define METALLIC_PREC_EXPF_H

#include "kernel/expf.h"
#include "../double/shift.h"
#include <math.h>

static double _expf(double x)
{
    const double minimum = -708.39641853226410622;
    const double maximum = 709.78271289338399684;

    const double log2e = 1.442695040888963407;
    const double ln2 = 0.6931471805599453094;

    if (x < minimum)
        return 0;

    if (x > maximum)
        return x * HUGE_VAL;

    double n = rint(x * log2e);
    double y = 1 + _kernel_expf(x - n * ln2);

    return _shift(y, n);
}

#endif
