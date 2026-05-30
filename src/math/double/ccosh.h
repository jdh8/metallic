#include "../reinterpret.h"
#include <complex.h>
#include <math.h>
#include <stdint.h>

static double _Complex ccosh_(double x, double y)
{
    double t = expm1(fabs(x));
    double cosh = 0.5 * (t + 1) + 0.5 / (t + 1);
    double sinh = copysign(t, x) * (0.5 + 0.5 / (t + 1));
    double cos_y = cos(y);
    double sin_y = sin(y);

    if (y == 0) {
        uint64_t i = reinterpret(uint64_t, copysign(0.0, x));
        uint64_t j = reinterpret(uint64_t, y);
        return CMPLX(cosh, reinterpret(double, i ^ j));
    }

    if (y - y) {
        if (x == 0)
            return CMPLX(y - y, x);

        if (isinf(x))
            return CMPLX(INFINITY, y - y);
    }

    return CMPLX(cosh * cos_y, sinh * sin_y);
}
