#include "clzti2.h"
#include <stdint.h>

static unsigned __int128 shl_(unsigned __int128 x, int shift)
{
    uint64_t high = x >> 64;
    uint64_t low = x;

    if (shift)
        return (unsigned __int128)(high << shift | low >> (64 - shift)) << 64 | low << shift;

    return x;
}

static uint64_t iterate_(unsigned __int128 a, unsigned __int128 b, unsigned __int128 r[static 1])
{
    uint64_t q = 0;
    int shift = clzti2_(b) - clzti2_(a);
    b = shl_(b, shift);

    for (uint64_t bit = (uint64_t)1 << shift; bit; bit >>= 1) {
        if (a >= b) {
            a -= b;
            q |= bit;
        }
        b >>= 1;
    }
    *r = a;
    return q;
}

static unsigned __int128 udivmodti4_(unsigned __int128 a, unsigned __int128 b, unsigned __int128 r[static 1])
{
    uint64_t a2 = a >> 64;
    uint64_t b2 = b >> 64;
    uint64_t a0 = a;
    uint64_t b0 = b;

    if (a < b) {
        *r = a;
        return 0;
    }

    if (!a2) {
        *r = a0 % b0;
        return a0 / b0;
    }

    if (!b0) {
        *r = (unsigned __int128)(a2 % b2) << 64 | a0;
        return a2 / b2;
    }

    if (!b2) {
        uint64_t q2 = a2 / b0;
        uint64_t q0 = iterate_((unsigned __int128)(a2 % b0) << 64 | a0, b0, r);
        return (unsigned __int128)q2 << 64 | q0;
    }

    return iterate_(a, b, r);
}
