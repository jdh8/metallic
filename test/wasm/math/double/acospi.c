#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(isnan(acospi(NAN)));
    assert(isnan(acospi(2.0)));
    assert(isnan(acospi(-2.0)));
    assert(isnan(acospi(-INFINITY)));

    /* Exact half-turn lattice: acos(1) = 0, acos(-1) = pi, acos(0) = pi/2,
     * acos(+-1/2) = pi/3, 2pi/3. */
    assert(identical(acospi(1.0), 0.0));
    assert(identical(acospi(-1.0), 1.0));
    assert(identical(acospi(0.0), 0.5));
    assert(identical(acospi(-0.0), 0.5));
    assert(identical(acospi(0.5), 0x1.5555555555555p-2));
    assert(identical(acospi(-0.5), 0x1.5555555555555p-1));

    /* Correctly rounded sample in the tiny band (1/2 - x/pi). */
    assert(identical(acospi(0x1p-30), 0x1.fffffffae833ep-2));
}
