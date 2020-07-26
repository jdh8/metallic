#include "umulditi3.h"

static unsigned __int128 umulti4_(unsigned __int128 a, unsigned __int128 b, unsigned __int128 high[static 1])
{
    uint64_t a1 = a >> 64;
    uint64_t b1 = b >> 64;
    uint64_t a0 = a;
    uint64_t b0 = b;
    
    unsigned __int128 a1b1 = umulditi3_(a1, b1);
    unsigned __int128 a1b0 = umulditi3_(a1, b0);
    unsigned __int128 a0b1 = umulditi3_(a0, b1);
    unsigned __int128 a0b0 = umulditi3_(a0, b0);
    unsigned __int128 mid = (a0b0 >> 64) + (uint64_t)a1b0 + (uint64_t)a0b1;

    *high = (mid >> 64) + (a1b0 >> 64) + (a0b1 >> 64) + a1b1;
    return mid << 64 | (uint64_t)a0b0;
}
