#include "src/math/float/rintf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        verify(identical(rintf(x), rintf_(x)), x);
        verify(identical(rintf(-x), rintf_(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(rintf_(x)));
        assert(isnan(rintf_(-x)));
    }
}
