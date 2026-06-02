/* C11 §7.12.4.4 / Annex F.10.1.4 special-value behaviour of atan2f, plus the
 * odd-symmetry invariant.
 *
 * Correct rounding of atan2f is gated by the oracle suite
 * (test/oracle/math/float/atan2f{,_cr}.c) against CORE-MATH's cr_atan2f and an
 * MPFR sweep, but its sampler only draws normal finite inputs -- it never
 * exercises the cases below, and it runs natively, not on wasm.  The quadrant
 * angles are the correctly-rounded float values (verified against cr_atan2f). */

#include "src/math/reinterpret.h"
#include "identical.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

static const float pi_4 = 0x1.921fb6p-1f;
static const float pi_2 = 0x1.921fb6p+0f;
static const float pi   = 0x1.921fb6p+1f;
static const float pi3_4 = 0x1.2d97c8p+1f;

int main(void)
{
    /* atan2(±0, +0) = ±0 ; atan2(±0, -0) = ±π. */
    assert(identical(atan2f(0.0f, 0.0f), 0.0f));
    assert(identical(atan2f(-0.0f, 0.0f), -0.0f));
    assert(atan2f(0.0f, -0.0f) == pi);
    assert(atan2f(-0.0f, -0.0f) == -pi);

    /* atan2(±0, x>0) = ±0 ; atan2(±0, x<0) = ±π. */
    assert(identical(atan2f(0.0f, 3.0f), 0.0f));
    assert(identical(atan2f(-0.0f, 3.0f), -0.0f));
    assert(atan2f(0.0f, -3.0f) == pi);
    assert(atan2f(-0.0f, -3.0f) == -pi);

    /* atan2(y>0, ±0) = +π/2 ; atan2(y<0, ±0) = -π/2. */
    assert(atan2f(3.0f, 0.0f) == pi_2);
    assert(atan2f(3.0f, -0.0f) == pi_2);
    assert(atan2f(-3.0f, 0.0f) == -pi_2);
    assert(atan2f(-3.0f, -0.0f) == -pi_2);

    /* atan2(±y, +∞) = ±0 ; atan2(±y, -∞) = ±π   (finite y > 0). */
    assert(identical(atan2f(3.0f, INFINITY), 0.0f));
    assert(identical(atan2f(-3.0f, INFINITY), -0.0f));
    assert(atan2f(3.0f, -INFINITY) == pi);
    assert(atan2f(-3.0f, -INFINITY) == -pi);

    /* atan2(±∞, x) = ±π/2   (finite x). */
    assert(atan2f(INFINITY, 3.0f) == pi_2);
    assert(atan2f(INFINITY, -3.0f) == pi_2);
    assert(atan2f(-INFINITY, 3.0f) == -pi_2);
    assert(atan2f(-INFINITY, -3.0f) == -pi_2);

    /* atan2(±∞, +∞) = ±π/4 ; atan2(±∞, -∞) = ±3π/4. */
    assert(atan2f(INFINITY, INFINITY) == pi_4);
    assert(atan2f(-INFINITY, INFINITY) == -pi_4);
    assert(atan2f(INFINITY, -INFINITY) == pi3_4);
    assert(atan2f(-INFINITY, -INFINITY) == -pi3_4);

    /* NaN in either argument propagates. */
    assert(isnan(atan2f(NAN, 1.0f)));
    assert(isnan(atan2f(1.0f, NAN)));
    assert(isnan(atan2f(NAN, NAN)));

    /* Odd symmetry in y: atan2(-y, x) == -atan2(y, x) bit-for-bit, over every
     * finite/inf input and branch (correct rounding guarantees it; this guards
     * the sign handling on the real wasm target). */
    for (uint32_t j = 0; j <= 0x7F800000; j += 0x00345679)
        for (uint32_t i = 0; i <= 0x7F800000; i += 0x00345679) {
            float y = reinterpret(float, j), x = reinterpret(float, i);
            assert(identical(atan2f(-y, x), -atan2f(y, x)));
            assert(identical(atan2f(-y, -x), -atan2f(y, -x)));
        }
}
