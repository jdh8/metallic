#include <math.h>
#include <limits.h>

long lrintf(float x)
{
    x = rintf(x);

    return x >= LONG_MAX ? LONG_MAX : x > LONG_MIN ? (long)x : LONG_MIN;
}
