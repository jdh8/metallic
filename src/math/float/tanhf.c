#include "expm1f.h"
#include <math.h>

static float right_(float x)
{
    if (x > 9.010913f)
        return 1;

    double y = expm1f_(2 * x);

    return y / (y + 2);
}

float tanhf(float x)
{
    return copysignf(right_(fabsf(x)), x);
}
