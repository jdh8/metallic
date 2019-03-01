#include "src/math/float/erfcf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(erfcf(INFINITY) == 0);
    assert(erfcf(-INFINITY) == 2);

    const uint32_t erfcinvmin = 0x41131CE0;

    for (uint32_t i = 0; i < erfcinvmin; i += 111) {
        float x = reinterpret(float, i);
        verify(faithful(erfcf(x), erfc(x)), x);
        verify(faithful(erfcf(-x), erfc(-x)), x);
    }

    for (uint32_t i = erfcinvmin; i < 0x7F800000; i += 137) {
        float x = reinterpret(float, i);
        verify(approx(erfcf(x), erfc(x), 1), x);
        verify(approx(erfcf(-x), erfc(-x), 1), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(erfcf(x)));
        assert(isnan(erfcf(-x)));
    }
}
