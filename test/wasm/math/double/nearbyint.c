#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    /* nearbyint is an alias of rint on wasm — they must agree bitwise. */
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += 0x00033158A6F1D0C4) {
        double x = reinterpret(double, i);
        assert(identical(nearbyint(x), rint(x)));
        assert(identical(nearbyint(-x), rint(-x)));
    }

    assert(nearbyint(INFINITY) == INFINITY);
    assert(isnan(nearbyint(NAN)));

    assert(nearbyint(0.5) == 0.0);
    assert(nearbyint(1.5) == 2.0);
    assert(nearbyint(2.5) == 2.0);
    assert(nearbyint(-1.5) == -2.0);
}
