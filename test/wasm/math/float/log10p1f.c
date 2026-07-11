#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(log10p1f(0.0f), 0.0f));
    assert(identical(log10p1f(-0.0f), -0.0f));
    assert(identical(log10p1f(-1.0f), -INFINITY));
    assert(isnan(log10p1f(-2.0f)));
    assert(isnan(log10p1f(-INFINITY)));
    assert(identical(log10p1f(INFINITY), INFINITY));
    assert(isnan(log10p1f(NAN)));

    /* log10p1(10^n - 1) = n exactly. */
    assert(identical(log10p1f(9.0f), 1.0f));
    assert(identical(log10p1f(99.0f), 2.0f));
    assert(identical(log10p1f(999.0f), 3.0f));
    assert(identical(log10p1f(9999999.0f), 7.0f));
}
