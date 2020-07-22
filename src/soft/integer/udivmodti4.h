#include <stdint.h>

static uint64_t divtri_(uint64_t a1, uint32_t a0, uint64_t b, uint64_t r[static 1])
{
    uint64_t b1 = b >> 32;
    uint64_t b0 = b & 0xFFFFFFFF;
    uint64_t q = a1 / b1;

    for (uint64_t rr = a1 - q * b1; q > 0xFFFFFFFF || q * b0 > (rr << 32 | a0); ) {
        --q;
        rr += b1;

        if (rr > 0xFFFFFFFF)
            break;
    }

    *r = (a1 << 32 | a0) - q * b;
    return q;
}

static uint64_t divq_normalized_(unsigned __int128 a, uint64_t b, uint64_t r[static 1])
{
    uint64_t q1 = divtri_(a >> 64, a >> 32, b, r);
    uint64_t q0 = divtri_(*r, a, b, r);

    return (q1 << 32) + q0;
}

static unsigned __int128 shl_(uint64_t high, uint64_t low, int shift)
{
    return shift
        ? (unsigned __int128)(high << shift | low >> (64 - shift)) << 64 | low << shift
        : (unsigned __int128)high << 64 | low;
}

static uint64_t divq_soft_(uint64_t a2, uint64_t a0, uint64_t b, uint64_t r[static 1])
{
    int s = __builtin_clzll(b);
    uint64_t q = divq_normalized_(shl_(a2, a0, s), b << s, r);
    *r >>= s;
    return q;
}

static uint64_t divq_(uint64_t a2, uint64_t a0, uint64_t b, uint64_t r[static 1])
{
#ifdef __x86_64__
    uint64_t result;
    __asm__("divq %[b]" : "=a"(result), "=d"(*r) : [b] "r"(b), "a"(a0), "d"(a2));
    return result;
#endif
    return divq_soft_(a2, a0, b, r);
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

    if (!b2) {
        uint64_t r0;
        uint64_t q2 = a2 / b0;
        uint64_t q0 = divq_(a2 % b0, a0, b0, &r0);

        *r = r0;
        return (unsigned __int128)q2 << 64 | q0;
    }

    uint64_t q = 0;
    int shift = __builtin_clzll(b >> 64) - __builtin_clzll(a >> 64);

    for (b = shl_(b2, b0, shift); shift >= 0; --shift) {
        __int128 s = (__int128)(b - a - 1) >> 127;
        q = (q << 1) | (s & 1);
        a -= b & s;
        b >>= 1;
    }

    *r = a;
    return q;
}
