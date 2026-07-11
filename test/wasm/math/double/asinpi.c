#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(asinpi(0.0), 0.0));
    assert(identical(asinpi(-0.0), -0.0));
    assert(isnan(asinpi(NAN)));
    assert(isnan(asinpi(2.0)));
    assert(isnan(asinpi(-2.0)));
    assert(isnan(asinpi(INFINITY)));

    /* The endpoints and asin(1/2) = pi/6 are exact in half-turns. */
    assert(identical(asinpi(1.0), 0.5));
    assert(identical(asinpi(-1.0), -0.5));
    assert(identical(asinpi(0.5), 0x1.5555555555555p-3));
    assert(identical(asinpi(-0.5), -0x1.5555555555555p-3));

    /* Correctly rounded samples: tiny band and near sqrt(2)/2. */
    assert(identical(asinpi(0x1p-30), 0x1.45f306dc9c883p-32));
    assert(identical(asinpi(0x1.6a09e667f3bcdp-1), 0x1.fffffffffffffp-3));
}
