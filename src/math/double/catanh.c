#include "catan.h"

double _Complex catanh(double _Complex z)
{
    double x = creal(z);
    double y = cimag(z);
    double _Complex first = catan_(fabs(y), fabs(x));

    return CMPLX(copysign(cimag(first), x), copysign(creal(first), y));
}
