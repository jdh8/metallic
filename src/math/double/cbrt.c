#include "../reinterpret.h"
#include "normalize.h"
#include <math.h>
#include <stdint.h>

static double _kernel(double x)
{
    int64_t i = reinterpret(int64_t, x);

    if (i == 0 || i >= 0x7FF0000000000000)
        return x;

    i = 0x2A9F7AF196E8E6E8 + _normalize(i) / 3;

    double y = reinterpret(double, i);

    y *= 0.5 + 1.5 * x / (2 * y * (y * y) + x);
    y += 0.33333333333333333333 * (x / (y * y) - y);
    y += 0.33333333333333333333 * (x / (y * y) - y);

    return y;
}

double cbrt(double x)
{
    return copysign(_kernel(fabs(x)), x);
}
