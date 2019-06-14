#include "eqtf2.h"

int __netf2(long double x, long double y)
{
    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return !eqtf2_(a, b);
}
