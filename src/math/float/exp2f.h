#ifndef METALLIC_PREC_EXP2F_H
#define METALLIC_PREC_EXP2F_H

#include "kernel/exp2f.h"
#include "../double/shift.h"
#include <math.h>
#include <stdint.h>

static double _exp2f(double x)
{
    if (x < -1022)
        return 0;

    if (x >= 1024)
        return x * HUGE_VAL;

    double n = rint(x);
    double y = 1 + _kernel_exp2f(x - n);

    return _shift(y, n);
}

#endif
