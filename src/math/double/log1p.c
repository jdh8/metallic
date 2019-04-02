#include "kernel/log.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _finite(double x)
{
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    double u = 1 + x;

    int64_t i = reinterpret(int64_t, u);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    double c = exponent < 2 ? 1 - u + x : x - u + 1;
    double y = reinterpret(double, i - (exponent << 52)) - 1;
    double z = y / (y + 2);
    double h = 0.5 * y * y;

    return exponent * ln2[1] + c / u + z * (h + _kernel_loga(z)) - h + y + exponent * ln2[0];
}

double log1p(double x)
{
    int64_t i = reinterpret(int64_t, x);
    
    if (i == 0xBFF0000000000000)
        return -INFINITY;

    if (i > 0xBFF0000000000000)
        return NAN;

    if (i >= 0x7FF0000000000000 || (uint64_t)i << 1 < 0x7940000000000000)
        return x;

    return _finite(x);
}
