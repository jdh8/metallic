#include <stddef.h>
#include <stdint.h>

int memcmp(const void* lhs, const void* rhs, size_t length)
{
    const unsigned char* x = lhs;
    const unsigned char* y = rhs;

    /* Length is known, so unaligned word loads stay in bounds (wasm allows
     * them).  Little-endian: the lowest differing byte is the first one. */
    for (; length >= sizeof(uint64_t); x += 8, y += 8, length -= 8) {
        uint64_t a, b;
        __builtin_memcpy(&a, x, sizeof(a));
        __builtin_memcpy(&b, y, sizeof(b));

        if (a != b) {
            int k = __builtin_ctzll(a ^ b) >> 3;
            return x[k] - y[k];
        }
    }

    while (length--) {
        unsigned char a = *x++;
        unsigned char b = *y++;

        if (a != b)
            return a - b;
    }

    return 0;
}
