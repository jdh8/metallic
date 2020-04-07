#include "src/math/float/asinf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x3F800000; i += 64) {
        float x = reinterpret(float, i);
        float y = asinf(x);
        verify(approx(y, asin(x), 1), x);
        verify(identical(-y, asinf(-x)), x);
    }

    for (uint32_t i = 0x3F800100; i < 0x80000000; i += 256) {
        float x = reinterpret(float, i);
        assert(isnan(asinf(x)));
        assert(isnan(asinf(-x)));
    }
}
