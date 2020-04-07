#include "src/math/float/asinhf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        float y = asinhf(x);
        verify(approx(y, asinh(x), 1), x);
        verify(identical(-y, asinhf(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(asinhf(x)));
        assert(isnan(asinhf(-x)));
    }
}
