/* C23 acospif: acos(x)/pi, correctly rounded.
 *
 * Port of metallic-rs `acospif` (src/f32_/atan.rs): the one-step 16-cell
 * structure shared with asinpif -- acospi(x) = g*sqrt(1-|x|) for x >= 0,
 * folded about 1 below, and 1/2 - z*q(z^2) in the tiny cell.  Certified by
 * the exhaustive 2^32 sweep in the oracle suite. */

#include "../reinterpret.h"
#include "kernel/asinpif.h"
#include <math.h>

float acospif(float x)
{
    uint32_t bits = reinterpret(uint32_t, x);
    uint32_t ax = bits & (~0u >> 1);

    if (ax >= 0x3F800000) {
        if (bits == 0x3F800000)
            return 0.0f; /* x = 1 */

        if (bits == 0xBF800000)
            return 1.0f; /* x = -1 */

        return (x - x) / (x - x); /* |x| > 1 or NaN */
    }

    const double *cell = asinpi_cells_[asinpi_cell_index_(bits)];
    double z = x;

    /* |x| < 1/16: fold the odd cell-0 kernel about 1/2 */
    if (ax < 0x3D800000)
        return 0.5 - z * asinpi_poly_(z * z, cell);

    double f = sqrt(1.0 - fabs(z));
    double g = asinpi_poly_(fabs(z), cell);

    return g * copysign(f, z) + (double)(bits >> 31);
}
