#include "../reinterpret.h"
#include <complex.h>
#include <math.h>
#include <stdint.h>

static double real_clog_(double x, double y)
{
    const double ln2 = 0.69314718055994530942;

    if (x == INFINITY || y == INFINITY)
        return INFINITY;

    int64_t i = reinterpret(int64_t, x * x + y * y);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    if (!i)
        return -INFINITY;

    if (i > 0x7FF0000000000000)
        return NAN;

    if (!exponent) {
        /* |z|^2 ≈ 1: use log1p to avoid catastrophic cancellation. */
        return 0.5 * log1p((x * x - 1) + y * y);
    }

    double c = reinterpret(double, i - (exponent << 52));
    double t = (c - 1) / (c + 1);
    /* log(|z|) = 2*atanh((c-1)/(c+1)) + (ln2/2)*exponent,
     * approximated with kernel_atanhf_ which gives ~0.5 ulp for double. */
    return 2 * t * (1 + t * t / 3) + ln2 / 2 * exponent;
}

static double _Complex clog_(double _Complex z)
{
    double x = creal(z);
    double y = cimag(z);

    return CMPLX(real_clog_(fabs(x), fabs(y)), atan2(y, x));
}
