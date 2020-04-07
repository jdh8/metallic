#include "src/math/float/expm1f.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(expm1f(INFINITY) == INFINITY);
    assert(expm1f(-INFINITY) == -1);

    const uint32_t max = reinterpret(uint32_t, (float)log(0x1p+128));

    for (uint32_t i = 0; i < max; i += 77) {
        float x = reinterpret(float, i);
        verify(approx(expm1f(x), expm1(x), 1), x);
    }

    for (uint32_t i = 0x80000000; i < 0xFF800000; i += 111) {
        float x = reinterpret(float, i);
        verify(approx(expm1f(x), expm1(x), 1), x);
    }

    for (uint32_t i = max; i < 0x7F800000; i += 777) {
        float x = reinterpret(float, i);
        verify(expm1f(x) == HUGE_VALF, x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(expm1f(x)));
        assert(isnan(expm1f(-x)));
    }
}
