#include <stdint.h>

__int128 __ashrti3(__int128 x, int shift)
{
    int64_t high = x >> 64;
    uint64_t low = x;

    if (shift & 64)
        return high >> shift;
    else if (shift)
        return (__int128)(high >> shift) << 64 | (high << (64 - shift) | low >> shift);
    else
        return x;
}
