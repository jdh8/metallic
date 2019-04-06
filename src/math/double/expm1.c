#include "kernel/expm1.h"
#include "shift.h"
#include <math.h>
#include <float.h>

double expm1(double x)
{
    const double minimum = -37.42994775023704;
    const double maximum = 709.782712893384;

    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    if (x < minimum)
        return -1;

    if (x > maximum)
        return maximum * DBL_MAX;

    if (fabsf(x) < 0x1p-53)
        return x;

    double n = rint(x * log2e);
    double a = x - n * ln2[0];
    double b = n * -ln2[1];
    double y[2];

    _kernel_expm1(y, a, b);

    int64_t exponent = n;

    switch (exponent) {
        case 0:
            return y[0] + y[1];
        case -1:
            return 0.5 * (y[0] + y[1]) - 0.5;
        case 1:
            return y[0] < -0.25 ? 2 * (y[0] + 0.5 + y[1]) : 2 * (y[0] + y[1]) + 1;
    }

    if ((uint64_t)exponent > 56)
        return _shift(y[0] + y[1] + 1, exponent) - 1;

    double small = reinterpret(double, (uint64_t)(0xBFF - exponent) << 52);
    double base = exponent < 20 ? y[0] + y[1] + (1 + small) : y[1] + small + y[0] + 1;

    return _shift(base, exponent);
}
