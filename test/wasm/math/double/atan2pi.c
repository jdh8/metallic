#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    assert(isnan(atan2pi(NAN, 1.0)));
    assert(isnan(atan2pi(1.0, NAN)));

    /* C23 F.10.1.4 rays, all exactly representable in half-turns. */
    assert(identical(atan2pi(0.0, 2.0), 0.0));
    assert(identical(atan2pi(-0.0, 2.0), -0.0));
    assert(identical(atan2pi(0.0, -2.0), 1.0));
    assert(identical(atan2pi(-0.0, -2.0), -1.0));
    assert(identical(atan2pi(0.0, 0.0), 0.0));
    assert(identical(atan2pi(-0.0, 0.0), -0.0));
    assert(identical(atan2pi(0.0, -0.0), 1.0));
    assert(identical(atan2pi(-0.0, -0.0), -1.0));
    assert(identical(atan2pi(3.0, 0.0), 0.5));
    assert(identical(atan2pi(-3.0, -0.0), -0.5));
    assert(identical(atan2pi(INFINITY, INFINITY), 0.25));
    assert(identical(atan2pi(-INFINITY, INFINITY), -0.25));
    assert(identical(atan2pi(INFINITY, -INFINITY), 0.75));
    assert(identical(atan2pi(-INFINITY, -INFINITY), -0.75));
    assert(identical(atan2pi(INFINITY, 5.0), 0.5));
    assert(identical(atan2pi(-INFINITY, 5.0), -0.5));
    assert(identical(atan2pi(2.0, INFINITY), 0.0));
    assert(identical(atan2pi(-2.0, INFINITY), -0.0));
    assert(identical(atan2pi(2.0, -INFINITY), 1.0));
    assert(identical(atan2pi(-2.0, -INFINITY), -1.0));

    /* The finite diagonal reaches +-1/4 and +-3/4 exactly. */
    assert(identical(atan2pi(1.0, 1.0), 0.25));
    assert(identical(atan2pi(1.0, -1.0), 0.75));
    assert(identical(atan2pi(-1.0, 1.0), -0.25));
    assert(identical(atan2pi(-1.0, -1.0), -0.75));

    /* Correctly rounded samples, plus the underflowed-quotient corners. */
    assert(identical(atan2pi(3.0, 5.0), 0x1.604c7a4a11c61p-3));
    assert(identical(atan2pi(5.0, 3.0), 0x1.4fd9c2daf71cfp-2));
    assert(identical(atan2pi(0x1p-1074, 1.0), 0.0));
    assert(identical(atan2pi(1.0, 0x1p-1074), 0.5));
}
