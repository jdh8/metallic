#include "src/math/float/cbrtf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (uint32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        float y = cbrtf(x);
        verify(faithful(y, cbrt(x)), x);
        verify(identical(-y, cbrtf(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(cbrtf(x)));
        assert(isnan(cbrtf(-x)));
    }
}
