#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(cospi(0.0), 1.0));
    assert(identical(cospi(-0.0), 1.0));
    assert(isnan(cospi(INFINITY)));
    assert(isnan(cospi(-INFINITY)));
    assert(isnan(cospi(NAN)));

    /* Half-integers land on +0, integers on +-1. */
    assert(identical(cospi(0.5), 0.0));
    assert(identical(cospi(-0.5), 0.0));
    assert(identical(cospi(1.5), 0.0));
    assert(identical(cospi(1.0), -1.0));
    assert(identical(cospi(-1.0), -1.0));
    assert(identical(cospi(2.0), 1.0));
    assert(identical(cospi(0x1p60), 1.0));

    /* Correctly rounded sample. */
    assert(identical(cospi(0.25), 0x1.6a09e667f3bcdp-1));
    assert(identical(cospi(0.75), -0x1.6a09e667f3bcdp-1));
}
