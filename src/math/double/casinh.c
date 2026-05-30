#include "casinh.h"

double _Complex casinh(double _Complex z)
{
    double x = creal(z);
    double y = cimag(z);
    double _Complex first = casinh_(fabs(x), fabs(y));

    return CMPLX(copysign(creal(first), x), copysign(cimag(first), y));
}
