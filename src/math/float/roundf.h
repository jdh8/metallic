#include <math.h>

static float roundf_(float x)
{
    return trunc(x + copysign(0.5, x));
}
