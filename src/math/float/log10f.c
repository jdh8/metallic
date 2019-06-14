#include "normalizef.h"
#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <math.h>

static float finite_(int32_t i)
{
    const double log10_e = 0.43429448190325182765;
    const double log10_2 = 0.30102999566398119521;

    int32_t exponent = (i - 0x3F3504F4) >> 23;
    double x = reinterpret(float, i - (exponent << 23));

    return 2 * log10_e * kernel_atanhf_((x - 1) / (x + 1)) + exponent * log10_2;
}

float log10f(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -INFINITY : NAN;

    if (i < 0x7F800000)
        return finite_(normalizef_(i));

    return x;
}
