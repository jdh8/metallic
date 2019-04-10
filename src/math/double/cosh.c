#include "kernel/expm1.h"
#include "shift.h"
#include <math.h>
#include <float.h>

/* Restriction of (cosh ∘ √) to [0, (ln 2)^2] */
static double _kernel(double x)
{
    const double c[] = {
        1.0000000000000000083e-0,
        4.9999999999999829907e-1,
        4.1666666666723293644e-2,
        1.3888888881819757406e-3,
        2.4801591503375393704e-5,
        2.7556037535355798775e-7,
        2.1070483998613861298e-9
    };

    double xx = x * x;

    return (c[6] * xx + c[5] * x + c[4]) * (xx * xx) + (c[3] * x + c[2]) * xx + c[1] * x + c[0];
}

double cosh(double x)
{
    const double maximum = 710.475860073944;
    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    x = fabs(x);

    if (x > maximum)
        return maximum * DBL_MAX;

    if (x < ln2[0] + ln2[1])
        return _kernel(x * x);

    double n = rint(x * log2e);
    double a = x - n * ln2[0];
    double b = n * -ln2[1];
    double y[2];

    _kernel_expm1(y, a, b);

    int64_t exponent = n;
    int64_t head = 0x3FF - 2 * exponent;
    double z = y[1] + y[0] + 1;
    double c = reinterpret(double, (head * (head > 0)) << 52);

    return _shift(c / z + y[1] + y[0] + 1, exponent - 1);
}
