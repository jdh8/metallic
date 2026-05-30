#include <complex.h>

double _Complex conj(double _Complex z)
{
    return CMPLX(creal(z), -cimag(z));
}
