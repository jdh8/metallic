#include "catanf.h"

float _Complex catanf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);
    float _Complex first = _catanf(fabsf(x), fabsf(y));

    return CMPLXF(copysignf(first, x), copysignf(cimagf(first), y));
}
