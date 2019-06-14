#ifndef METALLIC_PREC_EXP2F_H
#define METALLIC_PREC_EXP2F_H

#include "kernel/exp2f.h"
#include "../double/shift.h"
#include <math.h>
#include <stdint.h>

static double exp2f_(double x)
{
    if (x < -1022)
        return 0;

    if (x >= 1024)
        return x * HUGE_VAL;

    double n = rint(x);
    double y = 1 + kernel_exp2f_(x - n);

    return shift_(y, n);
}

#endif
