#include "kernel/log.h"
#include "normalize.h"
#include "../reinterpret.h"
#include <math.h>

static double finite_(int64_t i)
{
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double x = reinterpret(double, i - (exponent << 52)) - 1;
    double z = x / (x + 2);
    double h = 0.5 * x * x;

    return exponent * ln2[1] + z * (h + kernel_log_(z)) - h + x + exponent * ln2[0];
}

double log(double x)
{
    int64_t i = reinterpret(int64_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -HUGE_VAL : NAN;

    if (i < 0x7FF0000000000000)
        return finite_(normalize_(i));

    return x;
}
