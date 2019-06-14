#include "expf.h"
#include <math.h>

float coshf(float x)
{
    double y = expf_(fabsf(x));

    return 0.5 * y + 0.5 / y;
}
