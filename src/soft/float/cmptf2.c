#include "unordtf2.h"
#include "../../math/reinterpret.h"

static int cmptf2_(long double x, long double y, int unordered)
{
    __int128 a = reinterpret(__int128, x);
    __int128 b = reinterpret(__int128, y);
    __int128 sign = (a & b) >> 127;

    if ((a | b) << 1 == 0)
        return 0;

    if (unordtf2_(a, b))
        return unordered;

    return ((a > b) - (a < b) + sign) ^ sign;
}

int __cmptf2(long double x, long double y)
{
    return cmptf2_(x, y, 1);
}

int __letf2(long double, long double) __attribute__((__alias__("__cmptf2")));
int __lttf2(long double, long double) __attribute__((__alias__("__cmptf2")));

int __getf2(long double x, long double y)
{
    return cmptf2_(x, y, -1);
}

int __gttf2(long double, long double) __attribute__((__alias__("__getf2")));
