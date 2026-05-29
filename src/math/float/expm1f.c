#include "expm1f.h"

float expm1f(float x)
{
    /* expm1 overflows binary32 at the same argument as exp.  The shared
     * expm1f_ helper works in the wider double range and would overflow its
     * exponent near the double limit, so clamp here. */
    if (x > 88.7228391116729996f)
        return INFINITY;

    return expm1f_(x);
}
