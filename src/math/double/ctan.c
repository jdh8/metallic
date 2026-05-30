#include <complex.h>

double _Complex ctan(double _Complex z)
{
    z = ctanh(CMPLX(cimag(z), creal(z)));

    return CMPLX(cimag(z), creal(z));
}
