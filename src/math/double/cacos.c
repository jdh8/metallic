#include "cacosh.h"

double _Complex cacos(double _Complex z)
{
    const double pi = 3.14159265358979323846;

    double x = creal(z);
    double y = cimag(z);

    double _Complex first = cacosh_(fabs(x), fabs(y));

    return CMPLX(fabs(cimag(first) - (x < 0) * pi), copysign(creal(first), -y));
}
