#include "atan2f.h"
#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <complex.h>
#include <math.h>
#include <stdint.h>

static double kernel_clog1pf_(double x)
{
    return kernel_atanhf_(x / (2 + x));
}

static double real_clogf_(float x, float y)
{
    const double ln2 = 0.69314718055994530942;

    if (x == INFINITY || y == INFINITY)
        return INFINITY;

    double a = x;
    double b = y;
    int64_t i = reinterpret(int64_t, a * a + b * b);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    if (!i)
        return -INFINITY;

    if (i > 0x7FF0000000000000)
        return NAN;

    if (!exponent) {
        if (x == 1)
            return kernel_clog1pf_(b * b);
        if (y == 1)
            return kernel_clog1pf_(a * a);
    }

    double c = reinterpret(double, i - (exponent << 52));

    return kernel_atanhf_((c - 1) / (c + 1)) + ln2 / 2 * exponent;
}

static double _Complex clogf_(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    return CMPLX(real_clogf_(fabsf(x), fabsf(y)), atan2f_(y, x));
}
