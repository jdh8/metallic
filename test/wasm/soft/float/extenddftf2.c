#include "src/math/reinterpret.h"
#include <math.h>
#include <stdint.h>
#include <assert.h>

double __trunctfdf2(long double);

static _Bool run(double x)
{
    return reinterpret(uint64_t, x) == reinterpret(uint64_t, __trunctfdf2(x));
}

int main(void)
{
    const uint64_t step = 0x00000034CBF126F8;

    assert(run(INFINITY));
    assert(run(-INFINITY));

    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        assert(run(x));
        assert(run(-x));
    }

    for (uint64_t i = 0x7FF0000000000001; i < 0x8000000000000000; i += step) {
        double x = reinterpret(double, i);
        assert(isnan(__trunctfdf2(x)));
        assert(isnan(__trunctfdf2(-x)));
    }
}
