#include "src/math/float/copysignf.c"
#include "binary.h"
#include <assert.h>

int main(void)
{
    assert(identical(copysignf(1.0f, 2.0f), 1.0f));
    assert(identical(copysignf(1.0f, -2.0f), -1.0f));
    assert(identical(copysignf(-1.0f, 2.0f), 1.0f));
    assert(identical(copysignf(-1.0f, -2.0f), -1.0f));

    /* Signed zeros. */
    assert(identical(copysignf(0.0f, 1.0f), 0.0f));
    assert(identical(copysignf(0.0f, -1.0f), -0.0f));
    assert(identical(copysignf(-0.0f, 1.0f), 0.0f));
    assert(identical(copysignf(5.0f, -0.0f), -5.0f));

    /* Magnitude preserved across the whole finite range. */
    for (uint32_t i = 0; i < 0x7F800000; i += 0x00010CDF) {
        float x = reinterpret(float, i);
        verify(identical(copysignf(x, 1.0f), x), x);
        verify(identical(copysignf(x, -1.0f), -x), x);
        verify(identical(copysignf(-x, 1.0f), x), x);
    }

    /* Infinities and NaN. */
    assert(copysignf(INFINITY, -1.0f) == -INFINITY);
    assert(copysignf(-INFINITY, 1.0f) == INFINITY);
    assert(isnan(copysignf(NAN, 1.0f)));
}
