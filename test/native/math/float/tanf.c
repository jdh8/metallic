#include "src/math/float/tanf.c"
#include "src/math/float/rem_pio2f.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i < 0x7F800000; i += 81) {
        float x = reinterpret(float, i);
        verify(faithful(tanf(x), tan(x)), x);
    }

    for (uint32_t i = 0x7F800000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(tanf(x)));
        assert(isnan(tanf(-x)));
    }
}
