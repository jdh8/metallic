#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(tanpi(0.0), 0.0));
    assert(identical(tanpi(-0.0), -0.0));
    assert(isnan(tanpi(INFINITY)));
    assert(isnan(tanpi(-INFINITY)));
    assert(isnan(tanpi(NAN)));

    /* The exact quarter-integer lattice. */
    assert(identical(tanpi(0.25), 1.0));
    assert(identical(tanpi(-0.25), -1.0));
    assert(identical(tanpi(0.75), -1.0));
    assert(identical(tanpi(0.5), INFINITY));
    assert(identical(tanpi(-0.5), -INFINITY));
    assert(identical(tanpi(1.5), -INFINITY));
    assert(identical(tanpi(1.0), -0.0));
    assert(identical(tanpi(-1.0), 0.0));
    assert(identical(tanpi(2.0), 0.0));
    assert(identical(tanpi(0x1p60), 0.0));
}
