#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(tanpif(0.0f), 0.0f));
    assert(identical(tanpif(-0.0f), -0.0f));
    assert(isnan(tanpif(INFINITY)));
    assert(isnan(tanpif(-INFINITY)));
    assert(isnan(tanpif(NAN)));

    /* The exact quarter-integer lattice. */
    assert(identical(tanpif(0.25f), 1.0f));
    assert(identical(tanpif(-0.25f), -1.0f));
    assert(identical(tanpif(0.75f), -1.0f));
    assert(identical(tanpif(0.5f), INFINITY));
    assert(identical(tanpif(-0.5f), -INFINITY));
    assert(identical(tanpif(1.5f), -INFINITY));
    assert(identical(tanpif(1.0f), -0.0f));
    assert(identical(tanpif(-1.0f), 0.0f));
    assert(identical(tanpif(2.0f), 0.0f));
    assert(identical(tanpif(0x1p23f), 0.0f));
    assert(identical(tanpif(-0x1p24f), -0.0f));
}
