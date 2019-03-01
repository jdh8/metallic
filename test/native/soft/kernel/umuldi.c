#include "src/soft/integer/kernel/umuldi.h"
#include <assert.h>

int main(void)
{
#if defined(__x86_64__) || defined(__aarch64__)
    uint64_t a = 0;
    uint64_t b = 0;

    for (long i = 0; i < 1L << 28; ++i) {
        assert(_kernel_umuldi(a, b) == (unsigned __int128)a * b);
        a = a + 1993 + (a >> 3);
        b = b + 2019 + (b >> 4);
    }
#else
#warning _umuldi is untested becase host does not support 64 × 64 → 128 multiplication
#endif
}
