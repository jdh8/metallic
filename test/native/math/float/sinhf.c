#include "src/math/float/sinhf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(sinhf(INFINITY) == INFINITY);
    assert(sinhf(-INFINITY) == -INFINITY);

    const uint32_t max = reinterpret(uint32_t, (float)log(0x1p129));

    for (uint32_t i = 0; i < max; i += 77) {
        float x = reinterpret(float, i);
        float y = sinhf(x);
        verify(faithful(y, sinh(x)), x);
        verify(identical(-y, sinhf(-x)), x);
    }

    for (uint32_t i = max + 1; i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(sinhf(x) == HUGE_VALF, x);
        verify(sinhf(-x) == -HUGE_VALF, x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(sinhf(x)));
        assert(isnan(sinhf(-x)));
    }
}
