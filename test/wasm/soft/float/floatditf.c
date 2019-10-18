#include "src/math/reinterpret.h"
#include <stdint.h>
#include <assert.h>

int64_t __fixtfdi(long double);

static _Bool run(int64_t a)
{
    return a == __fixtfdi(a);
}

int main(void)
{
    const int64_t delta = 0x0000002BA87B5A22;

    assert(run(INT64_MAX));
    assert(run(INT64_MIN));

    for (int64_t i = 0x3FFFFFFFFFFFFFFF; i < 0x4340000000000000; i += delta) {
        double x = reinterpret(double, i);
        assert(run(x));
        assert(run(-x));
    }

    for (int64_t i = 0; i <= INT64_MAX / delta; ++i) {
        assert(run(i * delta));
        assert(run(i * -delta));
    }
}
