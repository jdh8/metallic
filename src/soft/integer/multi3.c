#include "kernel/mulq.h"
#include <stdint.h>

unsigned __int128 __multi3(unsigned __int128 a, unsigned __int128 b)
{
    uint64_t a1 = a >> 64;
    uint64_t b1 = b >> 64;
    uint64_t a0 = a;
    uint64_t b0 = b;

    return ((unsigned __int128)(a1 * b0 + a0 * b1) << 64) + mulq_(a0, b0);
}
