#include "aliased.h"
#include <stddef.h>
#include <stdint.h>

int strncmp(const char lhs[static 1], const char rhs[static 1], size_t length)
{
    const unsigned char* x = (const unsigned char*)lhs;
    const unsigned char* y = (const unsigned char*)rhs;

    /* Same alignment discipline as strcmp: aligned word loads only. */
    for (; (uintptr_t)x % sizeof(uint64_t); ++x, ++y, --length) {
        if (!length)
            return 0;

        if (*x != *y || !*x)
            return *x - *y;
    }

    if ((uintptr_t)y % sizeof(uint64_t) == 0) {
        const uint64_t ALIASED* vx = (const uint64_t*)x;
        const uint64_t ALIASED* vy = (const uint64_t*)y;
        const uint64_t magic = 0x7EFEFEFEFEFEFEFF;

        for (; length >= sizeof(uint64_t); length -= sizeof(uint64_t)) {
            if (*vx != *vy || ((*vx + magic) ^ ~*vx) & ~magic)
                break;

            ++vx;
            ++vy;
        }

        x = (const unsigned char*)vx;
        y = (const unsigned char*)vy;
    }

    while (length--) {
        unsigned char a = *x++;
        unsigned char b = *y++;

        if (a != b || !a)
            return a - b;
    }

    return 0;
}
