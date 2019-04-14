#include "kernel/expm1.h"
#include "shift.h"
#include <math.h>
#include <float.h>

/* Restriction of (x -> sinh(√x) / √x - 1) to [0, (1.25 ln 2)^2] */
static double _kernel(double x)
{
    const double c[] = {
        1.66666666666666599184e-01,
        8.33333333333980256984e-03,
        1.98412698312210868100e-04,
        2.75573249315927915790e-06,
        2.50506435040197608755e-08,
        1.62321815943872254558e-10
    };

    double xx = x * x;

    return (c[5] * xx + c[4] * x + c[3]) * (xx * xx) + (c[2] * x + c[1]) * xx + c[0] * x;
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

    if (r < 0.86643397569993163677)
        return x + x * _kernel(x * x);

    double n = rint(r * log2e);
    double a = r - n * ln2[0];
    double b = n * -ln2[1];
    double y[2];

    _kernel_expm1(y, a, b);

    uint64_t exponent = n;

    if (exponent > 56)
        return _shift(copysign(y[0] + y[1] + 1, x), exponent - 1);

    double small = reinterpret(double, (0xBFF - exponent) << 52);
    double base = exponent < 20 ? y[0] + y[1] + (1 + small) : y[1] + small + y[0] + 1;
    double t = _shift(base, exponent);

    return copysign(0.5, x) * (t + t / (t + 1));
}
