#include <complex.h>

float _Complex conjf(float _Complex z)
{
    return CMPLXF(z, -cimagf(z));
}
