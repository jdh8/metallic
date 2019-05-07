#include "kernel/exp.h"
#include "shift.h"
#include <math.h>
#include <float.h>

static const double _log2e = 1.44269504088896340736;
static const double _ln2[] = { 0x1.62e42fefa4p-1, -0x1.8432a1b0e2634p-43 };

static double _kernel(double x)
{
    const double c[] = {
        3.33333333333333325270e-1,
       -2.22222222222133128770e-2,
        2.11640211480687751936e-3,
       -2.11640107146572980826e-4,
        2.13747028936886696782e-5,
       -2.12203166597434505056e-6
    };

    double xx = x * x;

    return (c[5] * xx + c[4] * x + c[3]) * (xx * xx) + (c[2] * x + c[1]) * xx + c[0] * x + 1;
}

static double _half(double x)
{
    double n = rint(x * _log2e);
    double a = x - n * _ln2[0];
    double b = n * -_ln2[1];
    double y = _kernel_expb(a, b);
    double t = n == 2 ? 4 * y + 3 : 2 * y + 1;

    return t / (t + 2);
}

static double _exp(double x)
{
    double n = rint(x * _log2e);
    double a = x - n * _ln2[0];
    double b = n * -_ln2[1];

    return _shift(_kernel_expb(a, b) + 1, n);
}

static double _right(double x)
{
    if (x < 0.2554128118829953416)
        return x / _kernel(x * x);

    if (x < 0.5493061443340548457)
        return _half(2 * x);

    if (x < 19.061547465398495995)
        return 1 - 2 / (_exp(2 * x) + 1);

    return 1 + 0 / x;
}

double tanh(double x)
{
    return copysign(_right(fabs(x)), x);
}
