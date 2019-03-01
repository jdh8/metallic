#include <math.h>

static float _roundf(float x)
{
    return trunc(x + copysign(0.5, x));
}
