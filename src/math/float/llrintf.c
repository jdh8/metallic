#include <math.h>
#include <limits.h>

long long llrintf(float x)
{
    x = rintf(x);

    return x >= LLONG_MAX ? LLONG_MAX : x > LLONG_MIN ? (long long)x : LLONG_MIN;
}
