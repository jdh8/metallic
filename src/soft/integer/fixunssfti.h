#include <stdint.h>

static unsigned __int128 _fixunssfti(float x)
{
    if (x < 1)
        return 0;

    uint64_t high = 0x1p-64f * x;
    uint64_t low = x - 0x1p64f * high;

    return (unsigned __int128)high << 64 | low;
}
