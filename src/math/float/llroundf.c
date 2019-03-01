#include <math.h>
#include <limits.h>

long long llroundf(float x)
{
    double y = x + copysign(0.5, x);

    return y >= LLONG_MAX ? LLONG_MAX : y > LLONG_MIN ? (long long)y : LLONG_MIN;
}
