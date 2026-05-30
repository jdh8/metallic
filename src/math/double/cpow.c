#include "clog.h"

double _Complex cpow(double _Complex base, double _Complex power)
{
    double _Complex z = power * clog_(base);
    double x = creal(z);
    double y = cimag(z);

    if (y == 0)
        return CMPLX(exp(x), y);

    if (y - y) {
        if (x == INFINITY)
            return CMPLX(x, y - y);

        if (x == -INFINITY)
            return 0;
    }

    return CMPLX(exp(x) * cos(y), exp(x) * sin(y));
}
