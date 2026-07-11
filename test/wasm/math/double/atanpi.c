#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(identical(atanpi(0.0), 0.0));
    assert(identical(atanpi(-0.0), -0.0));
    assert(isnan(atanpi(NAN)));
    assert(identical(atanpi(INFINITY), 0.5));
    assert(identical(atanpi(-INFINITY), -0.5));

    /* atan(1) = pi/4: exactly a quarter half-turn. */
    assert(identical(atanpi(1.0), 0x1p-2));
    assert(identical(atanpi(-1.0), -0x1p-2));

    /* Correctly rounded samples: the tiny band (x/pi), the main band, and
     * the subnormal tail (2^-1074/pi rounds to zero). */
    assert(identical(atanpi(0x1p-30), 0x1.45f306dc9c883p-32));
    assert(identical(atanpi(3.0), 0x1.972028ecef984p-2));
    assert(identical(atanpi(0x1p-1074), 0.0));
    assert(identical(atanpi(-0x1p-1074), -0.0));
}
