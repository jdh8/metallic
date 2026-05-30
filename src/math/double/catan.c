#include "catan.h"

double _Complex catan(double _Complex z)
{
    double x = creal(z);
    double y = cimag(z);
    double _Complex first = catan_(fabs(x), fabs(y));

    return CMPLX(copysign(creal(first), x), copysign(cimag(first), y));
}
