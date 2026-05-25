#include "src/math/double/copysign.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    /* Basic sign transfer. */
    assert(identical(copysign(1.0, 2.0), 1.0));
    assert(identical(copysign(1.0, -2.0), -1.0));
    assert(identical(copysign(-1.0, 2.0), 1.0));
    assert(identical(copysign(-1.0, -2.0), -1.0));

    /* Signed zeros must be respected on both arguments. */
    assert(identical(copysign(0.0, 1.0), 0.0));
    assert(identical(copysign(0.0, -1.0), -0.0));
    assert(identical(copysign(-0.0, 1.0), 0.0));
    assert(identical(copysign(0.0, -0.0), -0.0));
    assert(identical(copysign(5.0, -0.0), -5.0));

    /* Magnitude preserved across the whole finite range. */
    const uint64_t step = 0x000000034E3F771F;
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(identical(copysign(x, 1.0), x), x);
        verify(identical(copysign(x, -1.0), -x), x);
        verify(identical(copysign(-x, 1.0), x), x);
        verify(identical(copysign(-x, -1.0), -x), x);
    }

    /* Infinities. */
    assert(copysign(INFINITY, -1.0) == -INFINITY);
    assert(copysign(-INFINITY, 1.0) == INFINITY);

    /* NaN keeps its absolute value, the sign comes from y. */
    assert(isnan(copysign(NAN, 1.0)));
    assert(isnan(copysign(NAN, -1.0)));
}
