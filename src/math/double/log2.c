#include "kernel/log.h"
#include "normalize.h"
#include "../reinterpret.h"
#include <math.h>

static double finite_(int64_t i)
{
    const double log2e[] = { 0x1.71547652p0, 0x1.705fc2eefa2p-33 };

    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double x = reinterpret(double, i - (exponent << 52)) - 1;
    double z = x / (x + 2);
    double h = 0.5 * x * x;
    
    uint64_t j = reinterpret(uint64_t, x - h) & 0xFFFFFFFF00000000;
    double a = reinterpret(double, j);
    double b = x - a - h + z * (h + kernel_log_(z));

    return (a + b) * log2e[1] + b * log2e[0] + a * log2e[0] + exponent;
}

double log2(double x)
{
    int64_t i = reinterpret(int64_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -HUGE_VAL : NAN;

    if (i < 0x7FF0000000000000)
        return finite_(normalize_(i));

    return x;
}
