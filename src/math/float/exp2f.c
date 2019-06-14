#include "kernel/exp2f.h"
#include "../double/shift.h"
#include <math.h>
#include <float.h>

float exp2f(float x)
{
    if (x < -150)
        return 0;

    if (x > 128)
        return 128 * FLT_MAX;

    float n = rintf(x);
    double y = 1 + kernel_exp2f_(x - n);

    return shift_(y, n);
}
