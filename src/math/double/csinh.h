#include <complex.h>
#include <math.h>

static double _Complex csinh_(double x, double y)
{
    double t = expm1(fabs(x));
    double cosh = 0.5 * (t + 1) + 0.5 / (t + 1);
    double sinh = copysign(t, x) * (0.5 + 0.5 / (t + 1));
    double cos_y = cos(y);
    double sin_y = sin(y);

    if (y == 0)
        return CMPLX(sinh, y);

    if (y - y && (x == 0 || isinf(x)))
        return CMPLX(x, y - y);

    return CMPLX(sinh * cos_y, cosh * sin_y);
}
