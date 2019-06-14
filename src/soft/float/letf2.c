#include "letf2.h"

int __letf2(long double x, long double y)
{
    unsigned __int128 a = reinterpret(unsigned __int128, x);
    unsigned __int128 b = reinterpret(unsigned __int128, y);

    return letf2_(a, b);
}
