#include "../../math/reinterpret.h"
#include <stdint.h>

long double __floatunsitf(uint32_t a)
{
    if (!a)
        return 0;

    uint64_t high = (reinterpret(uint64_t, (double)a) >> 4) + 0x3C00000000000000;

    return reinterpret(long double, (unsigned __int128)high << 64);
}
