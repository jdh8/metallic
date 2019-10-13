#include "../../math/reinterpret.h"
#include <stdint.h>

uint64_t __fixunstfdi(long double x)
{
    __int128 bits = reinterpret(__int128, x);
    int64_t high = bits >> 64;
    uint64_t low = bits;

    if (high < 0x3FFF000000000000)
        return 0;

    int shift = 112 + 0x3FFF - (int)(high >> 48);
    uint64_t significand = (high & 0x0000FFFFFFFFFFFF) | 0x0001000000000000;

    if (shift < 64)
        return significand << (64 - shift) | low >> shift;

    return significand >> (shift & 63);
}
