#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(sinpi(0.0), 0.0));
    assert(identical(sinpi(-0.0), -0.0));
    assert(isnan(sinpi(INFINITY)));
    assert(isnan(sinpi(-INFINITY)));
    assert(isnan(sinpi(NAN)));

    /* Integers land on signed zeros, half-integers on +-1. */
    assert(identical(sinpi(1.0), 0.0));
    assert(identical(sinpi(-1.0), -0.0));
    assert(identical(sinpi(2.0), 0.0));
    assert(identical(sinpi(0x1p60), 0.0));
    assert(identical(sinpi(-0x1p60), -0.0));
    assert(identical(sinpi(0.5), 1.0));
    assert(identical(sinpi(-0.5), -1.0));
    assert(identical(sinpi(1.5), -1.0));

    /* Correctly rounded sample. */
    assert(identical(sinpi(0.25), 0x1.6a09e667f3bcdp-1));
    assert(identical(sinpi(-0.25), -0x1.6a09e667f3bcdp-1));
}
