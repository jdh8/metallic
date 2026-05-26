#include "../../math/reinterpret.h"
#include "decimal.h"
#include "scientific.h"
#include <stdint.h>

/* Narrow a correctly-rounded double to float without double-rounding.
 *
 * (float)y rounds y to 24 bits of mantissa.  If decimal_to_double_ produced
 * the exact double representation of the input, (float)y is correctly
 * rounded automatically -- nearest float == nearest float of (nearest
 * double).  Trouble only at the float-tie case where the low 29 bits of
 * y are exactly 1<<28 (tie) AND the original decimal was truncated past
 * 38 digits, so the "true" value lies one or the other side of the tie
 * that the rounded double can no longer disambiguate.  Setting bit 29
 * (the float-LSB-adjacent bit) before casting forces the round to go
 * away from the tie -- standard round-to-odd.
 */
static float decimal_to_scalar_(const decimal_t* d)
{
    double y = decimal_to_double_(d);
    uint64_t bits = reinterpret(uint64_t, y);
    uint64_t low29 = bits & ((UINT64_C(1) << 29) - 1);

    if (low29 == (UINT64_C(1) << 28) && d->truncated) {
        bits |= UINT64_C(1) << 29;
        y = reinterpret(double, bits);
    }

    return (float)y;
}
