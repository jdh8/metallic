#include <complex.h>
#include <math.h>

double _Complex ctanh(double _Complex z)
{
    double x = creal(z);
    double y = cimag(z);

    double t = expm1(2 * fabs(x));
    double sinhcosh = copysign(t, x) * (0.25 + 0.25 / (t + 1));
    double sinhsinh = 0.25 * t * t / (t + 1);

    double cos_y = cos(y);
    double sin_y = sin(y);

    if (x != x && y == 0)
        return z;

    if (y - y)
        return isinf(x) ? copysign(1.0, x) : CMPLX(y - y, y - y);

    if (t >= 0x1p53)
        return CMPLX(copysign(1.0, x), sin_y * cos_y / (0.25 * t));

    return CMPLX(sinhcosh, sin_y * cos_y) / (sinhsinh + cos_y * cos_y);
}
