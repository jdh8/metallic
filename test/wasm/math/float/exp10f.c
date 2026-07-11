#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(exp10f(0.0f), 1.0f));
    assert(identical(exp10f(-0.0f), 1.0f));
    assert(identical(exp10f(1.0f), 10.0f));
    assert(identical(exp10f(2.0f), 100.0f));
    assert(identical(exp10f(10.0f), 1e10f));
    assert(identical(exp10f(INFINITY), INFINITY));
    assert(identical(exp10f(-INFINITY), 0.0f));
    assert(isnan(exp10f(NAN)));

    assert(identical(exp10f(39.0f), INFINITY));
    assert(identical(exp10f(-46.0f), 0.0f));
}
