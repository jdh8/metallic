#include <stdint.h>

static unsigned __int128 kernel_umuldi_(uint64_t a, uint64_t b)
{
    uint32_t a1 = a >> 32;
    uint32_t b1 = b >> 32;
    uint32_t a0 = a;
    uint32_t b0 = b;

    return ((unsigned __int128)a1 * b1 << 64 | (uint64_t)a0 * b0)
        + ((unsigned __int128)a1 * b0 << 32)
        + ((unsigned __int128)a0 * b1 << 32);
}

static inline unsigned __int128 umuldi_(uint64_t a, uint64_t b)
{
#if defined(__x86_64__) || defined(__aarch64__)
    return (unsigned __int128)a * b;
#endif
    return kernel_umuldi_(a, b);
}
