#include "round.h"
#include <limits.h>

long long llround(double x)
{
    double y = round_(x);

    return y >= LLONG_MAX ? LLONG_MAX : y > LLONG_MIN ? (long long)y : LLONG_MIN;
}
