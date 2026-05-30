#include "ccosh.h"

double _Complex ccos(double _Complex z)
{
    return ccosh_(-cimag(z), creal(z));
}
