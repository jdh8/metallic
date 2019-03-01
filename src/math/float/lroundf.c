#include <math.h>
#include <limits.h>

long lroundf(float x)
{
    double y = x + copysign(0.5, x);

    return y >= LONG_MAX ? LONG_MAX : y > LONG_MIN ? (long)y : LONG_MIN;
}
