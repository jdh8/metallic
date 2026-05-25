/* On wasm this is an alias of rint via src/math/double/rint.c. On
 * native we add a wrapper in src/math/double/nearbyint.c. Both
 * produce the same bit pattern as rint, so test that equivalence.
 *
 * The native wrapper calls rint(), so we pull in rint.c to provide
 * the project's own implementation rather than relying on libm.
 */
#include "src/math/double/rint.c"
#include "src/math/double/nearbyint.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    /* Standard half-to-even rounding (FE_TONEAREST). */
    assert(nearbyint(0.0) == 0.0);
    assert(identical(nearbyint(-0.0), -0.0));
    assert(nearbyint(0.5) == 0.0);
    assert(nearbyint(1.5) == 2.0);
    assert(nearbyint(2.5) == 2.0);
    assert(nearbyint(-0.5) == 0.0);
    assert(nearbyint(-1.5) == -2.0);
    assert(nearbyint(-2.5) == -2.0);
    assert(nearbyint(0.49999999999999994) == 0.0);
    assert(nearbyint(0.5000000000000001) == 1.0);

    /* Infinities and NaN. */
    assert(nearbyint(INFINITY) == INFINITY);
    assert(nearbyint(-INFINITY) == -INFINITY);
    assert(isnan(nearbyint(NAN)));

    /* nearbyint(x) must equal rint(x) bitwise across the whole
     * finite range — this libc's fenv is a no-op shim. */
    const uint64_t step = 0x000000034E3F771F;
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        verify(identical(nearbyint(x), rint_(x)), x);
        verify(identical(nearbyint(-x), rint_(-x)), x);
    }
}
