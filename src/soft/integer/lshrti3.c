#include <stdint.h>

unsigned __int128 __lshrti3(unsigned __int128 x, int shift)
{
    uint64_t high = x >> 64;
    uint64_t low = x;

    if (shift & 64)
        return high >> shift;
    else if (shift)
        return (unsigned __int128)(high >> shift) << 64 | (high << (64 - shift) | low >> shift);
    else
        return x;
}
