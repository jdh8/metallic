#include "expm1f.h"
#include <math.h>

float sinhf(float x)
{
    float r = fabsf(x);
    double y = _expm1f(r);

    return copysign(y, x) * (0.5 + 0.5 / (y + 1));
}
