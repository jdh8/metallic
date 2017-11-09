/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#ifndef METALLIC_NORMALIZE_H
#define METALLIC_NORMALIZE_H

#include <float.h>
#include <stdint.h>
/*!
 * \brief Normalize nonzero magnitude
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
 *
 * \param i - Representation of a nonzero magnitude
 * \return  Normalized magnitude
 */
inline int64_t __normalize(int64_t i)
{
    if (i < 1LL << (DBL_MANT_DIG - 1)) {
        int64_t shift = __builtin_clzll(i) - (64 - DBL_MANT_DIG);
        return (i << shift) - (shift << (DBL_MANT_DIG - 1));
    }
    return i;
}

#endif
