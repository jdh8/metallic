#include "kernel/expm1.h"
#include "shift.h"
#include <math.h>
#include <float.h>

/* Restriction of (x -> sinh(√x) / √x - 1) to [0, (1.25 ln 2)^2] */
static double _kernel(double x)
{
    const double c[] = {
        1.66666666666666662045e-01,
        8.33333333333402566563e-03,
        1.98412698395892126606e-04,
        2.75573207158623445088e-06,
        2.50515099034693706003e-08,
        1.61696409097928967071e-10
    };

    double xx = x * x;

    return (c[5] * xx + c[4] * x + c[3]) * (xx * xx) + (c[2] * x + c[1]) * xx + c[0] * x;
}

static double _base(const double y[static 2], uint64_t n)
{
    double yy = y[0] + y[1] + 1;

    return n > 56 ? yy : y[1] - 1 / _shift(yy, 2 * n) + y[0] + 1;
}

double sinh(double x)
{
    const double log2e = 1.44269504088896340736;
    const double ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

    double r = fabs(x);

    if (reinterpret(uint64_t, r) > 0x408633CE8FB9F87D)
        return x * HUGE_VAL;

    if (r < 0x1p-52)
        return x;

    if (r < ln2[0] + ln2[1])
        return x + x * _kernel(x * x);

    double n = rint(r * log2e);
    double a = r - n * ln2[0];
    double b = n * -ln2[1];
    double y[2];

    _kernel_expm1(y, a, b);

    return _shift(copysign(0.5, x) * _base(y, n), n);
}
