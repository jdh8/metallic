#include "src/math/float/rintf.c"
#include "src/math/float/nearbyintf.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    /* Half-to-even (default rounding). */
    assert(nearbyintf(0.0f) == 0.0f);
    assert(identical(nearbyintf(-0.0f), -0.0f));
    assert(nearbyintf(0.5f) == 0.0f);
    assert(nearbyintf(1.5f) == 2.0f);
    assert(nearbyintf(2.5f) == 2.0f);
    assert(nearbyintf(-1.5f) == -2.0f);

    /* Infinities and NaN. */
    assert(nearbyintf(INFINITY) == (float)INFINITY);
    assert(isnan(nearbyintf(NAN)));

    /* Equals rintf bitwise across the finite range. */
    for (uint32_t i = 0; i < 0x7F800000; i += 0x00010CDF) {
        float x = reinterpret(float, i);
        verify(identical(nearbyintf(x), rintf_(x)), x);
        verify(identical(nearbyintf(-x), rintf_(-x)), x);
    }
}
