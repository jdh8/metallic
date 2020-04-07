#include "src/math/float/tanhf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (uint32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        float y = tanhf(x);
        verify(approx(y, tanh(x), 1), x);
        verify(identical(-y, tanhf(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(tanhf(x)));
        assert(isnan(tanhf(-x)));
    }
}
