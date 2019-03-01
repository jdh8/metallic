#include <stdint.h>

static unsigned __int128 _fixunsdfti(double x)
{
    if (x < 1)
        return 0;

    uint64_t high = 0x1p-64 * x;
    uint64_t low = x - 0x1p64 * high;

    return (unsigned __int128)high << 64 | low;
}
