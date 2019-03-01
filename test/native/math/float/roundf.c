#include "src/math/float/roundf.h"
#include "unary.h"
#include <assert.h>

int main(void)
{
    for (int32_t i = 0; i <= 0x7F800000; i += 64) {
        float x = reinterpret(float, i);
        float y = roundf(x);
        verify(identical(y, _roundf(x)), x);
        verify(identical(-y, _roundf(-x)), x);
    }

    for (uint32_t i = 0x7FC00000; i < 0x80000000; i += 81) {
        float x = reinterpret(float, i);
        assert(isnan(_roundf(x)));
        assert(isnan(_roundf(-x)));
    }
}
