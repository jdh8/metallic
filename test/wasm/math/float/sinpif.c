#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(sinpif(0.0f), 0.0f));
    assert(identical(sinpif(-0.0f), -0.0f));
    assert(isnan(sinpif(INFINITY)));
    assert(isnan(sinpif(-INFINITY)));
    assert(isnan(sinpif(NAN)));

    /* Integers land on signed zeros, half-integers on +-1. */
    assert(identical(sinpif(1.0f), 0.0f));
    assert(identical(sinpif(-1.0f), -0.0f));
    assert(identical(sinpif(2.0f), 0.0f));
    assert(identical(sinpif(0x1p23f), 0.0f));
    assert(identical(sinpif(-0x1p23f), -0.0f));
    assert(identical(sinpif(0.5f), 1.0f));
    assert(identical(sinpif(-0.5f), -1.0f));
    assert(identical(sinpif(1.5f), -1.0f));
    assert(identical(sinpif(131072.5f), 1.0f));

    /* Correctly rounded samples. */
    assert(identical(sinpif(0.25f), 0x1.6a09e6p-1f));
    assert(identical(sinpif(-0.25f), -0x1.6a09e6p-1f));
}
