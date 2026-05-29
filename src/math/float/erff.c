#include "kernel/erff.h"
#include <math.h>

float erff(float x)
{
    float r = fabsf(x);

    return r < 0.4375f ? kernel_erf_(x) : copysignf(1 - kernel_erfc_(r), x);
}
