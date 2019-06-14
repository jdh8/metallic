#include "normalize.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

double frexp(double x, int exp[static 1])
{
    int64_t i = reinterpret(int64_t, fabs(x));

    if (i == 0 || i >= 0x7FF0000000000000) {
        *exp = 0;
        return x;
    }

    i = normalize_(i);
    *exp = (i >> 52) - 1022;

    return copysign(reinterpret(double, (i & 0x000FFFFFFFFFFFFF) | 0x3FE0000000000000), x);
}
