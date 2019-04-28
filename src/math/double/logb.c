#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

double logb(double x)
{
    double y = fabs(x);
    int64_t word = reinterpret(int64_t, y);

    if (word == 0)
        return -HUGE_VAL;

    if (word < 0x0010000000000000)
        return -(1011 + __builtin_clzll(word));

    if (word < 0x7FF0000000000000)
        return (word >> 52) - 1023;

    return y;
}
