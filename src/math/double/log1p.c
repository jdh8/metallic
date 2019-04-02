#include "kernel/log.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _finite(double x)
{
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    int64_t i = reinterpret(int64_t, 1 + x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    double y = exponent ? reinterpret(double, i - (exponent << 52)) - 1 : x;
    double z = y / (y + 2);
    double h = 0.5 * y * y;

    return z * (h + _kernel_loga(z)) + exponent * ln2[1] - h + y + -exponent * -ln2[0];
}

double log1p(double x)
{
    int64_t i = reinterpret(int64_t, x);
    
    if (i == 0xBFF0000000000000)
        return -INFINITY;

    if (i > 0xBFF0000000000000)
        return NAN;

    if (i < 0x7FF0000000000000)
        return _finite(x);

    return x;
}
