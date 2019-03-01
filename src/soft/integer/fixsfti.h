#include <math.h>
#include <stdint.h>

static __int128 _fixsfti(float x)
{
    float r = fabsf(x);
    uint64_t high = 0x1p-64f * r;
    uint64_t low = r - 0x1p64f * high;
    __int128 abs = (__int128)high << 64 | low;

    return signbit(x) ? -abs : abs;
}
