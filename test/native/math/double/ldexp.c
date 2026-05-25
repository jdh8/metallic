/* ldexp is already provided by src/math/double/scalbn.c as an alias of
 * scalbn (see that file). The pre-existing scalbn coverage in
 * test/wasm/math/double/scalbn.c is exhaustive — here we add a small
 * native check that the alias itself is exposed and well-behaved.
 */
#include "src/math/double/scalbn.c"
#include "unary.h"
#include <assert.h>

int main(void)
{
    assert(ldexp(1.0, 0) == 1.0);
    assert(ldexp(1.0, 1) == 2.0);
    assert(ldexp(1.0, 10) == 1024.0);
    assert(ldexp(0.5, 1) == 1.0);
    assert(ldexp(-1.0, 3) == -8.0);
    assert(ldexp(0.0, 100) == 0.0);
    assert(identical(ldexp(-0.0, 100), -0.0));

    /* Underflow to subnormal, then to zero. */
    assert(ldexp(1.0, -1074) > 0.0);
    assert(ldexp(1.0, -1075) == 0.0);

    /* Overflow to infinity. */
    assert(ldexp(1.0, 1024) == INFINITY);

    /* NaN propagates. */
    assert(isnan(ldexp(NAN, 5)));

    /* Match scalbn on a broad sweep. */
    const uint64_t step = 0x000000A1B2C3D4E5;
    for (uint64_t i = 0; i < 0x7FF0000000000000; i += step) {
        double x = reinterpret(double, i);
        for (int n = -100; n <= 100; n += 25)
            verify(identical(ldexp(x, n), scalbn(x, n)), x);
    }
}
