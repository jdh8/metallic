#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    /* For directions that round-trip through double cleanly,
     * nexttoward(x, (double)y) must equal nextafter(x, (double)y). */
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += 0x00033158A6F1D0C4) {
        double x = reinterpret(double, i);
        if (x == 0) continue;
        double up = nextafter(x, INFINITY);
        double dn = nextafter(x, -INFINITY);
        assert(identical(nexttoward(x, (long double)up   * 2), up));
        assert(identical(nexttoward(x, (long double)dn   / 2), dn));
        assert(identical(nexttoward(-x, (long double)(-up) * 2), -up));
    }

    assert(identical(nexttoward(1.0, 1.0L), 1.0));
    assert(nexttoward(1.0, 2.0L) > 1.0);
    assert(nexttoward(1.0, 0.0L) < 1.0);
    assert(nexttoward(0.0, 1.0L) > 0.0);
    assert(isnan(nexttoward(NAN, 0.0L)));
}
