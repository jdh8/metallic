#include "unordtf2.h"
#include "../../math/reinterpret.h"

int __cmptf2(long double x, long double y)
{
    __int128 a = reinterpret(__int128, x);
    __int128 b = reinterpret(__int128, y);
    __int128 sign = (a & b) >> 127;

    if ((a | b) << 1 == 0)
        return 0;

    if (unordtf2_(a, b))
        return 1;

    return ((a > b) - (a < b) + sign) ^ sign;
}
