#include "src/math/float/erff.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (uint32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        float y = erff(x);
        verify(approx(y, erf(x), 1), x);
        verify(identical(-y, erff(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(erff(x)));
        assert(isnan(erff(-x)));
    }
}
