#include "kernel/atanh.h"
#include "kernel/lanczos.h"
#include "finite/sinpif.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double logf_(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    x = reinterpret(double, i - (exponent << 52));

    return 2 * kernel_atanh_((x - 1) / (x + 1)) + exponent * ln2;
}

static double lcoeff_(double z)
{
    double base = lanczos_g_ + 0.5 + z;

    return (0.5 + z) * logf_(base) - base;
}

static double kernel_lgamma1pf_(double x)
{
    const double c[] = {
        -0.57721566490148570655,
         0.82246722695868583587,
        -0.40068588160955324422
    };

    return c[0] * x + (c[1] + c[2] * x) * (x * x);
}

static double kernel_lgamma2pf_(double x)
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
        return logf_(pi / (fabs(sinpif_(z)) * lanczos_series_(-z))) - lcoeff_(-z);
    }

    if (fabs(z - 1.0) < 0x1p-10)
        return kernel_lgamma1pf_(z - 1.0);

    if (fabs(z - 2.0) < 0x1p-8)
        return kernel_lgamma2pf_(z - 2.0);

    return lcoeff_(z - 1.0) + logf_(lanczos_series_(z - 1.0));
}
