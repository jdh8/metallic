#include "casinhf.h"

float _Complex casinf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);
    float _Complex first = _casinhf(fabsf(y), fabsf(x));

    return CMPLXF(copysignf(cimagf(first), x), copysignf(first, y));
}
