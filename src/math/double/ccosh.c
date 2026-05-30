#include "ccosh.h"

double _Complex ccosh(double _Complex z)
{
    return ccosh_(creal(z), cimag(z));
}
