#include "kernel/expm1.h"
#include "shift.h"
#include <math.h>
#include <float.h>

double expm1(double x)
{
    const double minimum = -37.42994775023705;
    const double maximum = 709.782712893385;

    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    if (x < minimum)
        return -1;

    if (x > maximum)
        return maximum * DBL_MAX;

    double n = rint(x * log2e);
    double a = x - n * ln2[0];
    double b = n * -ln2[1];
    double y[2];

    _kernel_expm1(y, a, b);

    if (n == 0)
        return copysign(y[0] + y[1], x);

    if (n == 1)
        return 2 * (y[0] + y[1]) + 1;

    return _shift(y[0] + y[1] + 1, n) - 1;
}
