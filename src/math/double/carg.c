#include <complex.h>
#include <math.h>

double carg(double _Complex z)
{
    return atan2(cimag(z), creal(z));
}
