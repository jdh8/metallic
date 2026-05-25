#include "src/math/double/nexttoward.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    /* Same-value short-circuit. */
    assert(identical(nexttoward(1.0, 1.0L), 1.0));
    assert(identical(nexttoward(0.0, 0.0L), 0.0));
    /* nexttoward(0.0, -0.0L) returns (double)to == -0.0 per C11. */
    assert(identical(nexttoward(0.0, -0.0L), -0.0));

    /* Step away from zero. */
    double up = nexttoward(1.0, 2.0L);
    assert(up > 1.0);
    assert(up == nextafter(1.0, 2.0));

    double down = nexttoward(1.0, 0.0L);
    assert(down < 1.0);
    assert(down == nextafter(1.0, 0.0));

    /* Zero -> smallest subnormal. */
    assert(nexttoward(0.0, 1.0L) > 0.0);
    assert(nexttoward(0.0, 1.0L) == nextafter(0.0, 1.0));
    assert(nexttoward(0.0, -1.0L) < 0.0);

    /* NaN propagation. */
    assert(isnan(nexttoward(NAN, 0.0L)));
    assert(isnan(nexttoward(0.0, (long double)NAN)));

    /* The defining property: nexttoward(from, y) should agree with
     * nextafter(from, (double)y) whenever (double)y differs from from
     * in the expected direction. */
    const uint64_t step = 0x000000A1B2C3D4E5;
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        if (x == 0) continue;
        double up_d = nextafter(x, INFINITY);
        double down_d = nextafter(x, -INFINITY);
        verify(identical(nexttoward(x,  (long double)up_d   * 2), up_d), x);
        verify(identical(nexttoward(x,  (long double)down_d * 0.5), down_d), x);
        verify(identical(nexttoward(-x, (long double)(-up_d)   * 2), -up_d), x);
        verify(identical(nexttoward(-x, (long double)(-down_d) * 0.5), -down_d), x);
    }

    /* A direction finer than a double ulp must still step `from`.
     * nextafterl gives a long double strictly above 1.0 that is much
     * closer than the double ulp at 1.0 (2^-52). */
    double one = 1.0;
    long double tiny_above = nextafterl(1.0L, 2.0L);
    assert(tiny_above > 1.0L);          /* sanity: the step is real */
    assert((double)tiny_above == one);  /* sanity: but invisible to double */
    double r = nexttoward(one, tiny_above);
    assert(r > one);
    assert(r == nextafter(one, 2.0));
}
