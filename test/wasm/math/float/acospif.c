#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(acospif(1.0f), 0.0f));
    assert(identical(acospif(-1.0f), 1.0f));
    assert(identical(acospif(0.0f), 0.5f));
    assert(identical(acospif(-0.0f), 0.5f));
    assert(isnan(acospif(2.0f)));
    assert(isnan(acospif(-2.0f)));
    assert(isnan(acospif(-INFINITY)));
    assert(isnan(acospif(NAN)));

    /* acospi(+-1/2) = 1/3 and 2/3 exactly, correctly rounded. */
    assert(identical(acospif(0.5f), 0x1.555556p-2f));
    assert(identical(acospif(-0.5f), 0x1.555556p-1f));
}
