#include "round.h"
#include <limits.h>

long long llround(double x)
{
    double y = _round(x);

    return y >= LLONG_MAX ? LLONG_MAX : y > LLONG_MIN ? (long long)y : LLONG_MIN;
}
