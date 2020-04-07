#include "src/math/float/erfcf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(erfcf(INFINITY) == 0);
    assert(erfcf(-INFINITY) == 2);

    for (uint32_t i = 0; i < 0x7F800000; i += 111) {
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
