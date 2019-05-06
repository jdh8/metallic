#include "expm1f.h"
#include <math.h>

static float _right(float x)
{
    if (x > 9.010913f)
        return 1;

    double y = _expm1f(2 * x);

    return y / (y + 2);
}

float tanhf(float x)
{
    return copysignf(_right(fabsf(x)), x);
}
