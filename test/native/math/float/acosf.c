#include "src/math/float/acosf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(acosf(0) * 2 == acosf(-1));
    assert(acosf(0.5) == (float)acos(0.5));

    for (int32_t i = 0; i < 0x3F800000; i += 81) {
        float x = reinterpret(float, i);
        verify(approx(acosf(x), acos(x), 1), x);
        verify(approx(acosf(-x), acos(-x), 1), x);
    }

    for (uint32_t i = 0x3F800100; i < 0x80000000; i += 0x100) {
        float x = reinterpret(float, i);
        assert(isnan(acosf(x)));
        assert(isnan(acosf(-x)));
    }
}
