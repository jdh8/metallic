#include "normalizef.h"
#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <math.h>

static float _finite(int32_t i)
{
    const double ln2 = 0.69314718055994530942;

    int32_t exponent = (i - 0x3F3504F4) >> 23;
    double x = reinterpret(float, i - (exponent << 23));

    return 2 * _kernel_atanhf((x - 1) / (x + 1)) + exponent * ln2;
}

float logf(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -INFINITY : NAN;

    if (i < 0x7F800000)
        return _finite(_normalizef(i));

    return x;
}
