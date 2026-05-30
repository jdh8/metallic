#include <complex.h>
#include <math.h>

double cabs(double _Complex z)
{
    return hypot(creal(z), cimag(z));
}
