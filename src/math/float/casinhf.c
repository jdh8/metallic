#include "casinhf.h"

float _Complex casinhf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);
    float _Complex first = _casinhf(fabsf(x), fabsf(y));

    return CMPLXF(copysignf(first, x), copysignf(cimagf(first), y));
}
