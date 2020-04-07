#include "src/math/float/atanf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        double y = atan(x);
        verify(approx(atanf(x), y, 1), x);
        verify(approx(atanf(-x), -y, 1), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(atanf(x)));
        assert(isnan(atanf(-x)));
    }
}
