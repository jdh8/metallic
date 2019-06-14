#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double finite_(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, (1 + x) / (1 - x));
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double y = reinterpret(double, i - (exponent << 52));

    if (exponent)
        return kernel_atanhf_((y - 1) / (y + 1)) + ln2 / 2 * exponent;
    else
        return kernel_atanhf_(x);
}

float atanhf(float x)
{
    float absx = fabsf(x);

    if (absx == 1)
        return copysignf(INFINITY, x);

    if (absx < 1)
        return finite_(x);

    return NAN;
}
