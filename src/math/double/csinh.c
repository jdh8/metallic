#include "csinh.h"

double _Complex csinh(double _Complex z)
{
    return csinh_(creal(z), cimag(z));
}
