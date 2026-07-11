#include <assert.h>
#include "identical.h"
#include <math.h>

int main(void)
{
    /* C23 F.10.4.2: (1 ± 0)^n = 1 and n = 0 -> 1, even against NaN; the
     * domain error x < -1 survives n = 0. */
    assert(identical(compoundn(0.0, 5), 1.0));
    assert(identical(compoundn(-0.0, -3), 1.0));
    assert(identical(compoundn(2.0, 0), 1.0));
    assert(identical(compoundn(NAN, 0), 1.0));
    assert(isnan(compoundn(-3.0, 0)));
    assert(isnan(compoundn(NAN, 3)));
    assert(isnan(compoundn(-2.0, 2))); /* 1 + x < 0 even for even n */
    assert(isnan(compoundn(-INFINITY, 7)));

    assert(identical(compoundn(INFINITY, 3), INFINITY));
    assert(identical(compoundn(INFINITY, -3), 0.0));
    assert(identical(compoundn(-1.0, 5), 0.0));
    assert(identical(compoundn(-1.0, -5), INFINITY));

    /* n = 1 is the single rounding of 1 + x. */
    assert(identical(compoundn(1.5, 1), 2.5));
    assert(identical(compoundn(0x1p-53, 1), 1.0));

    /* Small exact powers. */
    assert(identical(compoundn(0.5, 3), 0x1.bp+1));
    assert(identical(compoundn(-0.5, 2), 0x1p-2));

    /* 2^n at the range edges: exact overflow, last normal, the smallest
     * subnormal, and the 2^-1075 tie that rounds even to zero. */
    assert(identical(compoundn(1.0, 1024), INFINITY));
    assert(identical(compoundn(1.0, 1023), 0x1p+1023));
    assert(identical(compoundn(1.0, -1074), 0x1p-1074));
    assert(identical(compoundn(1.0, -1075), 0.0));

    /* Hard-to-round pairs from the metallic-rs f64_compound corpus. */
    assert(identical(compoundn(0x1.036d92bf2d082p-48, 47853425694171544LL), 0x1.786f39f0432a3p+248));
    assert(identical(compoundn(0x1.0f6578b8fc702p-48, -50669637826483056LL), 0x1.988dcc9a55ecdp-276));
    assert(identical(compoundn(0x1.0ff9611d8bc95p-48, -15319157706034212LL), 0x1.7f56f06514ef9p-84));
}
