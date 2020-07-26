#include <stdint.h>

/* Unsigned division of (96, 64) -> (32, 64) bits
 *
 * Undefined behavior may occur if the quotient overflows.  The 32-bit
 * quotient is returned as uint64_t to potentially save a instruction.
 */
static uint64_t uldivmoddisi5_(uint64_t a1, uint32_t a0, uint64_t b, uint64_t r[static 1])
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

/* Unsigned division of (128, 64) -> (64, 64) bits
 *
 * Undefined behavior may occur if the quotient overflows.
 */
static uint64_t uldivmoddi5_(uint64_t a1, uint64_t a0, uint64_t b, uint64_t r[static 1])
{
#ifdef __x86_64__
    uint64_t result;
    __asm__("divq %[b]" : "=a"(result), "=d"(*r) : [b] "r"(b), "a"(a0), "d"(a1));
    return result;
#endif

    int s = __builtin_clzll(b);
    unsigned __int128 an = ((unsigned __int128)a1 << 64 | a0) << (s & 63);
    uint64_t bn = b << s;
    uint64_t q1 = uldivmoddisi5_(an >> 64, an >> 32, bn, r);
    uint64_t q0 = uldivmoddisi5_(*r, an, bn, r);

    *r >>= s;
    return (q1 << 32) | q0;
}

static unsigned __int128 udivmodti4_(unsigned __int128 a, unsigned __int128 b, unsigned __int128 r[static 1])
{
    uint64_t a1 = a >> 64;
    uint64_t a0 = a;
    uint64_t b1 = b >> 64;
    uint64_t b0 = b;

    if (a < b) {
        *r = a;
        return 0;
    }

    if (!b1) {
        uint64_t r0;
        uint64_t q0 = uldivmoddi5_(a1 % b0, a0, b0, &r0);

        *r = r0;
        return (unsigned __int128)(a1 / b0) << 64 | q0;
    }

    uint64_t q = 0;
    int shift = __builtin_clzll(b1) - __builtin_clzll(a1);

    for (b <<= shift & 63; shift >= 0; --shift) {
        __int128 s = (__int128)(b - a - 1) >> 127;
        q = (q << 1) | (s & 1);
        a -= b & s;
        b >>= 1;
    }

    *r = a;
    return q;
}
