#include "catanf.h"

float _Complex catanhf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);
    float _Complex first = catanf_(fabsf(y), fabsf(x));

    return CMPLXF(copysignf(cimagf(first), x), copysignf(first, y));
}
