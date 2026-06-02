/* C11 §7.12.7.3 / Annex F.10.4.3 special-value behaviour of hypotf, plus the
 * symmetry invariants.
 *
 * Correct rounding of hypotf is gated by the oracle suite
 * (test/oracle/math/float/hypotf{,_cr}.c) against CORE-MATH's cr_hypotf and an
 * MPFR sweep, but its sampler only draws normal finite inputs -- it never
 * exercises the cases below, and it runs natively, not on wasm. */

#include "src/math/reinterpret.h"
#include "identical.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

int main(void)
{
    /* hypot(±∞, y) = +∞ for ANY y, even NaN; likewise hypot(x, ±∞). */
    assert(hypotf(INFINITY, 3.0f) == INFINITY);
    assert(hypotf(-INFINITY, 3.0f) == INFINITY);
    assert(hypotf(3.0f, INFINITY) == INFINITY);
    assert(hypotf(3.0f, -INFINITY) == INFINITY);
    assert(hypotf(INFINITY, INFINITY) == INFINITY);
    assert(hypotf(INFINITY, NAN) == INFINITY);
    assert(hypotf(NAN, INFINITY) == INFINITY);
    assert(hypotf(-INFINITY, NAN) == INFINITY);
    assert(hypotf(NAN, -INFINITY) == INFINITY);

    /* NaN propagates when neither argument is infinite. */
    assert(isnan(hypotf(NAN, 3.0f)));
    assert(isnan(hypotf(3.0f, NAN)));
    assert(isnan(hypotf(NAN, NAN)));

    /* hypot(±0, ±0) = +0. */
    assert(identical(hypotf(0.0f, 0.0f), 0.0f));
    assert(identical(hypotf(-0.0f, 0.0f), 0.0f));
    assert(identical(hypotf(0.0f, -0.0f), 0.0f));
    assert(identical(hypotf(-0.0f, -0.0f), 0.0f));

    /* hypot(±0, y) = |y| ; hypot(x, ±0) = |x|. */
    assert(hypotf(0.0f, -5.0f) == 5.0f);
    assert(hypotf(-0.0f, 5.0f) == 5.0f);
    assert(hypotf(-3.0f, 0.0f) == 3.0f);
    assert(hypotf(3.0f, -0.0f) == 3.0f);
    assert(hypotf(0.0f, FLT_MAX) == FLT_MAX);
    assert(hypotf(FLT_MIN, 0.0f) == FLT_MIN);

    /* Non-negative, and exact on Pythagorean triples regardless of sign/order. */
    assert(hypotf(3.0f, 4.0f) == 5.0f);
    assert(hypotf(-3.0f, -4.0f) == 5.0f);
    assert(hypotf(4.0f, -3.0f) == 5.0f);
    assert(hypotf(5.0f, 12.0f) == 13.0f);
    assert(hypotf(8.0f, 15.0f) == 17.0f);
    assert(hypotf(7.0f, 24.0f) == 25.0f);
    assert(hypotf(20.0f, 21.0f) == 29.0f);

    /* hypot is even in each argument and order-independent, bit-for-bit, across
     * every branch on the real wasm target (correct rounding guarantees it). */
    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00345679)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00345679) {
            float x = reinterpret(float, i), y = reinterpret(float, j);
            float r = hypotf(x, y);
            assert(identical(r, hypotf(y, x)));
            assert(identical(r, hypotf(-x, y)));
            assert(identical(r, hypotf(x, -y)));
            assert(identical(r, hypotf(-x, -y)));
            assert(identical(r, hypotf(-y, -x)));
        }
}
