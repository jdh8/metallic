#include "aliased.h"
#include <stdint.h>

int strcmp(const char lhs[static 1], const char rhs[static 1])
{
    const unsigned char* x = (const unsigned char*)lhs;
    const unsigned char* y = (const unsigned char*)rhs;

    /* Word loads past the terminator are only safe when aligned (an
     * unaligned load could cross the end of linear memory), so align x
     * first and take the word loop only if y then lands aligned too. */
    for (; (uintptr_t)x % sizeof(uint64_t); ++x, ++y)
        if (*x != *y || !*x)
            return *x - *y;

    if ((uintptr_t)y % sizeof(uint64_t) == 0) {
        const uint64_t ALIASED* vx = (const uint64_t*)x;
        const uint64_t ALIASED* vy = (const uint64_t*)y;
        const uint64_t magic = 0x7EFEFEFEFEFEFEFF;

        /* Advance while the words are equal and the zero-byte probe stays
         * quiet; a false-positive probe merely finishes byte-wise. */
        while (*vx == *vy && !(((*vx + magic) ^ ~*vx) & ~magic)) {
            ++vx;
            ++vy;
        }

        x = (const unsigned char*)vx;
        y = (const unsigned char*)vy;
    }

    while (*x == *y && *x) {
        ++x;
        ++y;
    }

    return *x - *y;
}
