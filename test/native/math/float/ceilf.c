#include "src/math/float/ceilf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        verify(identical(ceilf(x), _ceilf(x)), x);
        verify(identical(ceilf(-x), _ceilf(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(_ceilf(x)));
        assert(isnan(_ceilf(-x)));
    }
}
