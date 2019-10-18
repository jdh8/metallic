#include "src/math/reinterpret.h"
#include <stdint.h>
#include <assert.h>

uint32_t __fixunstfsi(long double);

static _Bool run(uint32_t a)
{
    return a == __fixunstfsi(a);
}

int main(void)
{
    const uint64_t delta = 0x0000002BA87B5A22;

    for (uint64_t i = 0x3FFFFFFFFFFFFFFF; i < 0x41F0000000000000; i += delta)
        assert(run(reinterpret(double, i)));
}
