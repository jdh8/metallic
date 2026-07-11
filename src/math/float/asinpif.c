/* C23 asinpif: asin(x)/pi, correctly rounded.
 *
 * Port of metallic-rs `asinpif` (src/f32_/atan.rs): a one-step 16-cell
 * lookup with no fallback tier -- see kernel/asinpif.h.  The reference
 * combines with FMA; the plain mul/add here re-passes the exhaustive 2^32
 * sweep in the oracle suite, which is the proof. */

#include "../reinterpret.h"
#include "kernel/asinpif.h"
#include <math.h>

float asinpif(float x)
{
    uint32_t bits = reinterpret(uint32_t, x);
    uint32_t ax = bits & (~0u >> 1);

    if (ax >= 0x3F800000) {
        if (ax == 0x3F800000)
            return copysignf(0.5f, x); /* |x| = 1 */

        return (x - x) / (x - x); /* |x| > 1 or NaN */
    }

    const double *cell = asinpi_cells_[asinpi_cell_index_(bits)];
    double z = x;

    /* |x| < 1/16: the odd cell-0 kernel z*q(z^2) */
    if (ax < 0x3D800000)
        return z * asinpi_poly_(z * z, cell);

    double f = sqrt(1.0 - fabs(z));
    double g = asinpi_poly_(fabs(z), cell);

    return copysign(0.5, z) - g * copysign(f, z);
}
