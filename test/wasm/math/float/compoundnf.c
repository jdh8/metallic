#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    /* C23 F.10.4.2: (1 ± 0)^n = 1 and n = 0 -> 1, even against NaN; the
     * domain error x < -1 survives n = 0. */
    assert(identical(compoundnf(0.0f, 5), 1.0f));
    assert(identical(compoundnf(-0.0f, -3), 1.0f));
    assert(identical(compoundnf(2.0f, 0), 1.0f));
    assert(identical(compoundnf(NAN, 0), 1.0f));
    assert(isnan(compoundnf(-3.0f, 0)));
    assert(isnan(compoundnf(NAN, 3)));
    assert(isnan(compoundnf(-2.0f, 2))); /* 1 + x < 0 even for even n */
    assert(isnan(compoundnf(-INFINITY, 7)));

    assert(identical(compoundnf(INFINITY, 3), INFINITY));
    assert(identical(compoundnf(INFINITY, -3), 0.0f));
    assert(identical(compoundnf(-1.0f, 5), 0.0f));
    assert(identical(compoundnf(-1.0f, -5), INFINITY));

    /* n = 1 is the single rounding of 1 + x. */
    assert(identical(compoundnf(1.5f, 1), 2.5f));
    assert(identical(compoundnf(0x1p-25f, 1), 1.0f));

    /* Small exact powers and the 2^n range edges (2^-150 ties even to 0). */
    assert(identical(compoundnf(0.5f, 3), 0x1.bp+1f));
    assert(identical(compoundnf(-0.5f, 2), 0x1p-2f));
    assert(identical(compoundnf(3.0f, 5), 0x1p+10f));
    assert(identical(compoundnf(1.0f, 128), INFINITY));
    assert(identical(compoundnf(1.0f, 127), 0x1p+127f));
    assert(identical(compoundnf(1.0f, -149), 0x1p-149f));
    assert(identical(compoundnf(1.0f, -150), 0.0f));

    /* Correctly rounded samples: the compound-interest regime and the
     * inexact-(double)n band (|n| > 2^53). */
    assert(identical(compoundnf(0x1p-30f, 1000000000), 0x1.44d8b6p+1f));
    assert(identical(compoundnf(0x1p-120f, 6000000000000000000LL), 1.0f));
    assert(identical(compoundnf(-0x1p-120f, 6000000000000000000LL), 1.0f));
}
