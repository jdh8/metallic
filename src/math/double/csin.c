#include "csinh.h"

double _Complex csin(double _Complex z)
{
    z = csinh_(cimag(z), creal(z));

    return CMPLX(cimag(z), creal(z));
}
