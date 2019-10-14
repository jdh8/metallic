#include "isnanl.h"

int __eqtf2(long double x, long double y)
{
    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return (a != b || isnanl_(a)) && (a | b) << 1;
}

int __netf2(long double, long double) __attribute__((__alias__("__eqtf2")));
