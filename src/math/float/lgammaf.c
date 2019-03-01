#include "kernel/atanhf.h"
#include "kernel/lanczos.h"
#include "finite/sinpif.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _logf(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    x = reinterpret(double, i - (exponent << 52));

    return 2 * _kernel_atanhf((x - 1) / (x + 1)) + exponent * ln2;
}

static double _lcoeff(double z)
{
    double base = _lanczos_g + 0.5 + z;

    return (0.5 + z) * _logf(base) - base;
}

static double _kernel_lgamma1pf(double x)
{
    const double c[] = {
        -0.57721566490148570655,
         0.82246722695868583587,
        -0.40068588160955324422
    };

    return c[0] * x + (c[1] + c[2] * x) * (x * x);
}

static double _kernel_lgamma2pf(double x)
{
    const double c[] = {
         0.42278433509889703662,
         0.32246726895319824010,
        -0.067352441921785509791
    };

    return c[0] * x + (c[1] + c[2] * x) * (x * x);
}

float lgammaf(float z)
{
    const double pi = 3.14159265358979324;

    if (z == 0 || z == INFINITY)
        return INFINITY;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return INFINITY;
        return _logf(pi / (fabs(_sinpif(z)) * _lanczos_series(-z))) - _lcoeff(-z);
    }

    if (fabs(z - 1.0) < 0x1p-10)
        return _kernel_lgamma1pf(z - 1.0);

    if (fabs(z - 2.0) < 0x1p-8)
        return _kernel_lgamma2pf(z - 2.0);

    return _lcoeff(z - 1.0) + _logf(_lanczos_series(z - 1.0));
}
