#include "../reinterpret.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

double nextafter(double from, double to)
{
    if (from == to || to != to)
        return to;

    if (from != from)
        return from;

    if (from == 0)
        return copysign(DBL_TRUE_MIN, to);

    int64_t a = reinterpret(int64_t, from);
    int64_t b = reinterpret(int64_t, to);

    return reinterpret(double, b < a || (a ^ b) < 0 ? a - 1 : a + 1);
}
