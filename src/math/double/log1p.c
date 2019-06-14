#include "kernel/log.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

double log1p(double x)
{
    int64_t i = reinterpret(int64_t, x);
    
    if (i == 0xBFF0000000000000)
        return -HUGE_VAL;

    if (i > 0xBFF0000000000000)
        return NAN;

    if (i >= 0x7FF0000000000000 || (uint64_t)i << 1 < 0x7940000000000000)
        return x;

    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    double u = 1 + x;
    int64_t j = reinterpret(int64_t, u);
    int64_t exponent = (j - 0x3FE6A09E667F3BCD) >> 52;

    double y = reinterpret(double, j - (exponent << 52)) - 1;
    double z = y / (y + 2);
    double h = 0.5 * y * y;

    return exponent * ln2[1] + (1 - u + x) / u + z * (h + kernel_log_(z)) - h + y + exponent * ln2[0];
}
