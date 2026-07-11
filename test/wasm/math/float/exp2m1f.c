#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(exp2m1f(0.0f), 0.0f));
    assert(identical(exp2m1f(-0.0f), -0.0f));
    assert(identical(exp2m1f(INFINITY), INFINITY));
    assert(identical(exp2m1f(-INFINITY), -1.0f));
    assert(isnan(exp2m1f(NAN)));

    assert(identical(exp2m1f(1.0f), 1.0f));
    assert(identical(exp2m1f(2.0f), 3.0f));
    assert(identical(exp2m1f(10.0f), 1023.0f));
    assert(identical(exp2m1f(-1.0f), -0.5f));
    assert(identical(exp2m1f(-2.0f), -0.75f));

    assert(identical(exp2m1f(128.0f), INFINITY));
    assert(identical(exp2m1f(-25.0f), -1.0f));
}
