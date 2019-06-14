#ifndef METALLIC_NORMALIZE_H
#define METALLIC_NORMALIZE_H

#include <stdint.h>

/* Normalize nonzero magnitude
 *
 * Notation of subnormal numbers is a space-saving technique in IEEE 754.
 * However, processing subnormal numbers requires an extra step to get correct
 * exponents and significands.  This function provides that step.
 *
 * The signbits of results safely join exponent bits because negative
 * representations were unused.  Infinite and normal numbers remain intact
 * since exponent bias is preserved.  The exponent representation of a
 * subnormal number becomes 0 or negative to preserve exponent bias.
 *
 * Zero is excluded from the domain of this function because one half of the
 * smallest normal number is already mapped to 0 and there is no other suitable
 * result for 0.
 */
static int64_t normalize_(int64_t i)
{
    if (i < 0x0010000000000000) {
        int64_t shift = __builtin_clzll(i) - 11;
        return (i << shift) - (shift << 52);
    }
    return i;
}

#endif
