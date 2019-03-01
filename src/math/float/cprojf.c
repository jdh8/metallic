#include <complex.h>
#include <math.h>

float _Complex cprojf(float _Complex z)
{
    if (isinf((float)z) || isinf(cimagf(z)))
        return CMPLXF(INFINITY, copysignf(0, cimagf(z)));

    return z;
}
