#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(asinpif(0.0f), 0.0f));
    assert(identical(asinpif(-0.0f), -0.0f));
    assert(identical(asinpif(1.0f), 0.5f));
    assert(identical(asinpif(-1.0f), -0.5f));
    assert(isnan(asinpif(2.0f)));
    assert(isnan(asinpif(-2.0f)));
    assert(isnan(asinpif(INFINITY)));
    assert(isnan(asinpif(NAN)));

    /* asinpi(1/2) = 1/6 exactly, correctly rounded. */
    assert(identical(asinpif(0.5f), 0x1.555556p-3f));
    assert(identical(asinpif(-0.5f), -0x1.555556p-3f));
}
