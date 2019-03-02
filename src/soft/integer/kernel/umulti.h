#include "umuldi.h"

static void _umulti(unsigned __int128 c[static 2], unsigned __int128 a, unsigned __int128 b)
{
    uint64_t a1 = a >> 64;
    uint64_t b1 = b >> 64;
    uint64_t a0 = a;
    uint64_t b0 = b;
    
    unsigned __int128 a1b1 = _umuldi(a1, b1);
    unsigned __int128 a1b0 = _umuldi(a1, b0);
    unsigned __int128 a0b1 = _umuldi(a0, b1);
    unsigned __int128 a0b0 = _umuldi(a0, b0);

    unsigned __int128 mid = (a0b0 >> 64) + (uint64_t)a1b0 + (uint64_t)a0b1;

    c[0] = mid << 64 | (uint64_t)a0b0;
    c[1] = (mid >> 64) + (a1b0 >> 64) + (a0b1 >> 64) + a1b1;
}
