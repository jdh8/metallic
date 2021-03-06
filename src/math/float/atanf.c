#include "kernel/atanf.h"
#include <math.h>

float atanf(float x)
{
    const double pi_2 = 1.57079632679489661923;

    return fabsf(x) > 1 ? copysign(pi_2, x) - kernel_atanf_(1.0 / x) : kernel_atanf_(x);
}
