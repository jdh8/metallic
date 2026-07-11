#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(log10p1(0.0), 0.0));
    assert(identical(log10p1(-0.0), -0.0));
    assert(identical(log10p1(-1.0), -INFINITY));
    assert(isnan(log10p1(-2.0)));
    assert(isnan(log10p1(-INFINITY)));
    assert(identical(log10p1(INFINITY), INFINITY));
    assert(isnan(log10p1(NAN)));

    /* log10p1(10^n - 1) = n exactly. */
    assert(identical(log10p1(9.0), 1.0));
    assert(identical(log10p1(99.0), 2.0));
    assert(identical(log10p1(999.0), 3.0));
    assert(identical(log10p1(999999.0), 6.0));
    assert(identical(log10p1(1e15 - 1.0), 15.0));
}
