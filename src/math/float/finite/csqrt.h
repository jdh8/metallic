#include <complex.h>
#include <math.h>

/* Finite nonzero complex square root */
static double _Complex _csqrt(double x, double y)
{
    double s = sqrt(0.5 * (fabs(x) + sqrt(x * x + y * y)));
    double t = 0.5 * y / s;

    if (signbit(x))
        return CMPLX(fabs(t), copysign(s, y));
    else
        return CMPLX(s, t);
}
