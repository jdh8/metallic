#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _finite(double c)
{
    const double ln2 = 0.6931471805599453094;

    double s = sqrt(c * c - 1);
    int64_t i = reinterpret(int64_t, c + s);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double y = reinterpret(double, i - (exponent << 52));

    return 2 * _kernel_atanhf((y - 1) / (y + 1)) + exponent * ln2;
}

float acoshf(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i < 0x3F800000)
        return NAN;

    if (i < 0x7F800000)
        return _finite(x);

    return x;
}
