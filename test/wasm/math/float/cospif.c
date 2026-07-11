#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(cospif(0.0f), 1.0f));
    assert(identical(cospif(-0.0f), 1.0f));
    assert(isnan(cospif(INFINITY)));
    assert(isnan(cospif(-INFINITY)));
    assert(isnan(cospif(NAN)));

    /* Half-integers land on +0, integers on +-1. */
    assert(identical(cospif(0.5f), 0.0f));
    assert(identical(cospif(-0.5f), 0.0f));
    assert(identical(cospif(1.5f), 0.0f));
    assert(identical(cospif(1.0f), -1.0f));
    assert(identical(cospif(-1.0f), -1.0f));
    assert(identical(cospif(2.0f), 1.0f));
    assert(identical(cospif(0x1p23f), 1.0f));
    assert(identical(cospif(4194304.5f), 0.0f));

    /* |x| < 2^-15 rounds to exactly 1. */
    assert(identical(cospif(0x1p-20f), 1.0f));
    assert(identical(cospif(0x1p-149f), 1.0f));

    /* Correctly rounded samples. */
    assert(identical(cospif(0.25f), 0x1.6a09e6p-1f));
    assert(identical(cospif(0.75f), -0x1.6a09e6p-1f));
}
