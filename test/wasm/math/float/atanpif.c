#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(atanpif(0.0f), 0.0f));
    assert(identical(atanpif(-0.0f), -0.0f));
    assert(identical(atanpif(INFINITY), 0.5f));
    assert(identical(atanpif(-INFINITY), -0.5f));
    assert(isnan(atanpif(NAN)));

    /* atanpi(+-1) = +-1/4 exactly. */
    assert(identical(atanpif(1.0f), 0.25f));
    assert(identical(atanpif(-1.0f), -0.25f));

    /* One of the sweep-pinned ties. */
    assert(identical(atanpif(0x1.d26a62p-1f), 0x1.e1a662p-3f));
}
