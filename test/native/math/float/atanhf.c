#include "src/math/float/atanhf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x3F800000; i += 64) {
        float x = reinterpret(float, i);
        double y = atanhf(x);
        verify(approx(y, atanh(x), 1), x);
        verify(identical(-y, atanhf(-x)), x);
    }

    for (uint32_t i = 0x3F800100; i < 0x80000000; i += 256) {
        float x = reinterpret(float, i);
        assert(isnan(atanhf(x)));
        assert(isnan(atanhf(-x)));
    }
}
