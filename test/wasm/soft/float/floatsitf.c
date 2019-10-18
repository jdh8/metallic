#include "src/math/reinterpret.h"
#include <stdint.h>
#include <assert.h>

int32_t __fixtfsi(long double);

static _Bool run(int32_t a)
{
    return a == __fixtfsi(a);
}

int main(void)
{
    const int64_t delta = 0x0000002BA87B5A22;

    assert(run(INT32_MAX));
    assert(run(INT32_MIN));

    for (int64_t i = 0x3FFFFFFFFFFFFFFF; i < 0x41E0000000000000; i += delta) {
        double x = reinterpret(double, i);
        assert(run(x));
        assert(run(-x));
    }
}
