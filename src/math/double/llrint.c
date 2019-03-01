#include <math.h>
#include <limits.h>

long long llrint(double x)
{
    x = rint(x);

    return x >= LLONG_MAX ? LLONG_MAX : x > LLONG_MIN ? (long long)x : LLONG_MIN;
}
