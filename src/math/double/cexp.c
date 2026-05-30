#include <complex.h>
#include <math.h>

double _Complex cexp(double _Complex z)
{
    double x = creal(z);
    double y = cimag(z);
    double r = exp(x);

    if (y == 0)
        return CMPLX(r, y);

    if (y - y) {
        if (x == INFINITY)
            return CMPLX(x, y - y);

        if (x == -INFINITY)
            return 0;
    }

    return CMPLX(r * cos(y), r * sin(y));
}
