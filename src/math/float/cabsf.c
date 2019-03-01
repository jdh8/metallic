#include <complex.h>
#include <math.h>

float cabsf(float _Complex z)
{
    return hypotf(z, cimagf(z));
}
