#include <math.h>
#include <limits.h>

long lrint(double x)
{
    x = rint(x);

    return x >= LONG_MAX ? LONG_MAX : x > LONG_MIN ? (long)x : LONG_MIN;
}
