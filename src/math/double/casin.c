#include "casinh.h"

double _Complex casin(double _Complex z)
{
    double x = creal(z);
    double y = cimag(z);
    double _Complex first = casinh_(fabs(y), fabs(x));

    return CMPLX(copysign(cimag(first), x), copysign(creal(first), y));
}
