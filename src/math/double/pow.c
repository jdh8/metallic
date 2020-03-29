#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double unsigned_(double x, double y)
{
    if (x == 1)
        return 1;

    if (x == 0)
        return signbit(y) ? HUGE_VAL : 0;

    if (isinf(x))
        return signbit(y) ? 0 : HUGE_VAL;

    if (signbit(x))
        return NAN;

    //TODO
    return exp2(y * log2(x));
}

double pow(double x, double y)
{
    uint64_t sign = 0;

    if (y == 0)
        return 1;

    if (signbit(x) && rintf(y) == y) {
        x = -x;
        sign = (uint64_t)(rintf(y / 2) != y / 2) << 63;
    }

    uint64_t magnitude = reinterpret(uint64_t, unsigned_(x, y));
    return reinterpret(double, magnitude | sign);
}

