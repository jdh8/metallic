#include "finite/csqrt.h"
#include <complex.h>
#include <math.h>

double _Complex csqrt(double _Complex z)
{
    double x = creal(z);
    double y = cimag(z);

    if (z == 0)
        return CMPLX(0, y);

    if (isinf(y))
        return CMPLX(INFINITY, y);

    if (x == INFINITY)
        return CMPLX(x, 0 * y);

    if (x == -INFINITY)
        return CMPLX(y - y, copysign(x, y));

    return csqrt_(x, y);
}
