#include <math.h>
#include <stdint.h>

static __int128 _fixdfti(double x)
{
    double r = fabs(x);
    uint64_t high = 0x1p-64 * r;
    uint64_t low = r - 0x1p64 * high;
    __int128 abs = (__int128)high << 64 | low;

    return signbit(x) ? -abs : abs;
}
