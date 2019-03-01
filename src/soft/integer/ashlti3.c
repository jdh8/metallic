#include <stdint.h>

__int128 __ashlti3(__int128 x, int shift)
{
    int64_t high = x >> 64;
    uint64_t low = x;

    if (shift & 64)
        return (__int128)(low << shift) << 64;
    else if (shift)
        return (__int128)(high << shift | low >> (64 - shift)) << 64 | low << shift;
    else
        return x;
}
