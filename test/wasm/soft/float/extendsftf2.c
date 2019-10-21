#include "src/math/reinterpret.h"
#include <math.h>
#include <stdint.h>
#include <assert.h>

float __trunctfsf2(long double);

static _Bool run(float x)
{
    return reinterpret(uint32_t, x) == reinterpret(uint32_t, __trunctfsf2(x));
}

int main(void)
{
    assert(run(INFINITY));
    assert(run(-INFINITY));

    for (uint32_t i = 0; i < 0x7F800000; i += 29) {
        float x = reinterpret(float, i);
        assert(run(x));
        assert(run(-x));
    }

    for (uint32_t i = 0x7F800001; i < 0x80000000; i += 37) {
        float x = reinterpret(float, i);
        assert(isnan(__trunctfsf2(x)));
        assert(isnan(__trunctfsf2(-x)));
    }
}
