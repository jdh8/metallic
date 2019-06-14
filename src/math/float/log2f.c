#include "normalizef.h"
#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <math.h>

static float finite_(int32_t i)
{
    const double log2e = 1.44269504088896340736;

    int32_t exponent = (i - 0x3F3504F4) >> 23;
    double x = reinterpret(float, i - (exponent << 23));

    return 2 * log2e * kernel_atanhf_((x - 1) / (x + 1)) + exponent;
}

float log2f(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -INFINITY : NAN;

    if (i < 0x7F800000)
        return finite_(normalizef_(i));

    return x;
}
