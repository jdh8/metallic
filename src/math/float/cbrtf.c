#include "normalizef.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static float kernel_(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i == 0 || i >= 0x7F800000)
        return x;

    i = 0x2A512CE3 + normalizef_(i) / 3;

    double y = reinterpret(float, i);

    y *= 0.5 + 1.5 * x / (2 * y * (y * y) + x);
    y += 0.33333333333333333333 * (x / (y * y) - y);

    return y;
}

float cbrtf(float x)
{
    return copysignf(kernel_(fabsf(x)), x);
}
