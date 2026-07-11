#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(exp10m1f(0.0f), 0.0f));
    assert(identical(exp10m1f(-0.0f), -0.0f));
    assert(identical(exp10m1f(INFINITY), INFINITY));
    assert(identical(exp10m1f(-INFINITY), -1.0f));
    assert(isnan(exp10m1f(NAN)));

    assert(identical(exp10m1f(1.0f), 9.0f));
    assert(identical(exp10m1f(2.0f), 99.0f));
    assert(identical(exp10m1f(3.0f), 999.0f));
    assert(identical(exp10m1f(-1.0f), -0.9f));

    assert(identical(exp10m1f(39.0f), INFINITY));
    assert(identical(exp10m1f(-8.0f), -1.0f));
}
