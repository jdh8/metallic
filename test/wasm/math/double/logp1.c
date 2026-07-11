#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    /* logp1 is the C23 name for log1p. */
    assert(identical(logp1(0.0), 0.0));
    assert(identical(logp1(-0.0), -0.0));
    assert(identical(logp1(-1.0), -INFINITY));
    assert(isnan(logp1(-2.0)));
    assert(identical(logp1(INFINITY), INFINITY));
    assert(isnan(logp1(NAN)));
    assert(identical(logp1(0x1p-30), log1p(0x1p-30)));
    assert(identical(logp1(12345.678), log1p(12345.678)));

    assert(identical(logp1f(0.0f), 0.0f));
    assert(identical(logp1f(-1.0f), -INFINITY));
    assert(isnan(logp1f(-2.0f)));
    assert(identical(logp1f(0x1p-20f), log1pf(0x1p-20f)));
    assert(identical(logp1f(1.5f), log1pf(1.5f)));
}
