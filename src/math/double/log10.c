#include "kernel/log.h"
#include "normalize.h"
#include "../reinterpret.h"
#include <math.h>

static double _finite(int64_t i)
{
    const double log10_2[] = { 0x1.34413509f78p-2, 0x1.fef311f12b358p-46 };
    const double log10_e[] = { 0x1.bcb7b152p-2, 0x1.b9438ca9aadd5p-36 };

    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;
    double x = reinterpret(double, i - (exponent << 52)) - 1;
    double z = x / (x + 2);
    double h = 0.5 * x * x;
    
    uint64_t j = reinterpret(uint64_t, x - h) & 0xFFFFFFFF00000000;
    double a = reinterpret(double, j);
    double b = x - a - h + z * (h + _kernel_log(z));

    return exponent * log10_2[1] + (a + b) * log10_e[1] + b * log10_e[0] + a * log10_e[0] + exponent * log10_2[0];
}

double log10(double x)
{
    int64_t i = reinterpret(int64_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -HUGE_VAL : NAN;

    if (i < 0x7FF0000000000000)
        return _finite(_normalize(i));

    return x;
}
