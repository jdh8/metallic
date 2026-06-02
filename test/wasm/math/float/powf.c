/* C11 §7.12.7.4 / Annex F.10.4.4 special-value behaviour of powf.
 *
 * Correct rounding of powf is gated by the oracle suite
 * (test/oracle/math/float/powf{,_cr}.c) bit-for-bit against CORE-MATH's cr_powf
 * plus an MPFR sweep.  But that sampler only draws normal finite inputs, so it
 * never exercises the special cases below -- and it runs natively, not on wasm.
 * This is the only test that covers them on the actual ship target. */

#include "src/math/reinterpret.h"
#include "identical.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

int main(void)
{
    /* pow(x, ±0) == 1 for every x, including NaN and the infinities. */
    assert(powf(2.0f, 0.0f) == 1);
    assert(powf(-3.0f, 0.0f) == 1);
    assert(powf(0.0f, 0.0f) == 1);
    assert(powf(-0.0f, 0.0f) == 1);
    assert(powf(INFINITY, 0.0f) == 1);
    assert(powf(-INFINITY, 0.0f) == 1);
    assert(powf(NAN, 0.0f) == 1);
    assert(powf(NAN, -0.0f) == 1);

    /* pow(1, y) == 1 for every y, including NaN and the infinities. */
    assert(powf(1.0f, 5.0f) == 1);
    assert(powf(1.0f, -3.0f) == 1);
    assert(powf(1.0f, 0.5f) == 1);
    assert(powf(1.0f, INFINITY) == 1);
    assert(powf(1.0f, -INFINITY) == 1);
    assert(powf(1.0f, NAN) == 1);

    /* pow(-1, ±inf) == 1. */
    assert(powf(-1.0f, INFINITY) == 1);
    assert(powf(-1.0f, -INFINITY) == 1);

    /* pow(±0, y): the sign survives only for odd-integer y; y < 0 gives ∞. */
    assert(identical(powf(0.0f, 3.0f), 0.0f));       /* odd     > 0 */
    assert(identical(powf(-0.0f, 3.0f), -0.0f));
    assert(identical(powf(0.0f, 2.0f), 0.0f));       /* even    > 0 */
    assert(identical(powf(-0.0f, 2.0f), 0.0f));
    assert(identical(powf(0.0f, 0.5f), 0.0f));       /* non-int > 0 */
    assert(identical(powf(-0.0f, 0.5f), 0.0f));
    assert(powf(0.0f, -3.0f) == INFINITY);           /* odd     < 0 */
    assert(powf(-0.0f, -3.0f) == -INFINITY);
    assert(powf(0.0f, -2.0f) == INFINITY);           /* even    < 0 */
    assert(powf(-0.0f, -2.0f) == INFINITY);
    assert(powf(0.0f, -0.5f) == INFINITY);           /* non-int < 0 */
    assert(powf(-0.0f, -0.5f) == INFINITY);

    /* pow(±inf, y). */
    assert(powf(INFINITY, 2.0f) == INFINITY);
    assert(identical(powf(INFINITY, -2.0f), 0.0f));  /* +0, not -0 */
    assert(powf(INFINITY, 0.5f) == INFINITY);
    assert(identical(powf(INFINITY, -0.5f), 0.0f));
    assert(powf(-INFINITY, 3.0f) == -INFINITY);      /* odd     > 0 */
    assert(powf(-INFINITY, 2.0f) == INFINITY);       /* even    > 0 */
    assert(powf(-INFINITY, 0.5f) == INFINITY);       /* non-int > 0 */
    assert(identical(powf(-INFINITY, -3.0f), -0.0f));/* odd     < 0 */
    assert(identical(powf(-INFINITY, -2.0f), 0.0f)); /* even    < 0 */
    assert(identical(powf(-INFINITY, -0.5f), 0.0f)); /* non-int < 0 */

    /* pow(x, ±inf) for finite x, by |x| ≷ 1. */
    assert(powf(0.5f, INFINITY) == 0);
    assert(powf(0.5f, -INFINITY) == INFINITY);
    assert(powf(-0.5f, INFINITY) == 0);
    assert(powf(-0.5f, -INFINITY) == INFINITY);
    assert(powf(2.0f, INFINITY) == INFINITY);
    assert(powf(2.0f, -INFINITY) == 0);
    assert(powf(-2.0f, INFINITY) == INFINITY);
    assert(powf(-2.0f, -INFINITY) == 0);

    /* Domain error: finite x < 0 with finite non-integer y -> NaN. */
    assert(isnan(powf(-2.0f, 0.5f)));
    assert(isnan(powf(-3.0f, 1.5f)));
    assert(isnan(powf(-0.5f, -1.5f)));

    /* NaN propagation, outside the pow(1,y) / pow(x,0) exceptions. */
    assert(isnan(powf(NAN, 2.0f)));
    assert(isnan(powf(2.0f, NAN)));
    assert(isnan(powf(-2.0f, NAN)));
    assert(isnan(powf(NAN, NAN)));

    /* Finite negative base, integer exponent: sign from the exponent parity. */
    assert(powf(-2.0f, 3.0f) == -8.0f);
    assert(powf(-2.0f, 2.0f) == 4.0f);
    assert(powf(-2.0f, -3.0f) == -0.125f);
    assert(powf(-2.0f, -2.0f) == 0.25f);
    assert(powf(-3.0f, 3.0f) == -27.0f);

    /* The two C11 identities over every exponent pattern (incl. inf/NaN). */
    for (uint32_t i = 0; i < 1u << 10; ++i) {
        float any = reinterpret(float, i << 22);
        assert(powf(1.0f, any) == 1);
        assert(powf(any, 0.0f) == 1);
        assert(powf(any, -0.0f) == 1);
    }
}
