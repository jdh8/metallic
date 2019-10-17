#include "src/math/reinterpret.h"
#include <math.h>
#include <stdint.h>
#include <assert.h>

static _Bool run(uint64_t a)
{
    return a == truncl(a);
}

int main(void)
{
    const uint64_t delta = 0x0008D46BA87B5A22;

    for (uint64_t i = 0x3FFFFFFFFFFFFFFF; i < 0x4340000000000000; i += delta)
        assert(run(reinterpret(double, i)));

    for (uint64_t i = 0; i <= UINT64_MAX / delta; ++i)
        assert(run(i * delta));
}
