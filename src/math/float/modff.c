#include <math.h>

float modff(float x, float i[static 1])
{
    *i = truncf(x);

    return copysignf(isinf(x) ? 0 : x - *i, x);
}
