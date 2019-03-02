#include <assert.h>
#include "identical.h"
#include <math.h>

static _Bool run(double from, double to)
{
    double x = from ? from : copysignf(from, to);
    double y = nextafter(from, to);
    _Bool near = reinterpret(uint64_t, x) - reinterpret(uint64_t, y) + 1 <= 2;

    if (from == to)
        return identical(to, y);

    if (from < to)
        return from < y && near;

    if (from > to)
        return from > y && near;

    return isnan(y);
}

int main(void)
{
    for (uint64_t j = 0; j < 0x8000000000000000; j += 0x0003F46E86FBE2AA)
        for (uint64_t i = 0; i < 0x8000000000000000; i += 0x00033158A6F1D0C4)
            assert(run(reinterpret(double, i), reinterpret(double, j)));

    for (uint64_t i = 0; i < 0x8000000000000000; i += 0x00033158A6F1D0C4)
        assert(run(reinterpret(double, i), INFINITY));

    for (uint64_t j = 0; j < 0x8000000000000000; j += 0x0003F46E86FBE2AA)
        assert(run(INFINITY, reinterpret(double, j)));
}
