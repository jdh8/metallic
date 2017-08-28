/* This file is part of Metallic, a C++ library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <float.h>
#include <stdint.h>

int32_t __fixtfsi(long double x)
{
    const int64_t bias = 0x3CLL << 56;
    int64_t high = *(__int128*)&x >> 64;

    /* These numbers may turn invalid with exponent bias subtracted. */
    if ((high & INT64_MAX) < 0x3FFFLL << 48) /* |x| < 1 */
        return 0;

    const int shift = (128 - LDBL_MANT_DIG) - (64 - DBL_MANT_DIG);
    int64_t signbit = high & (unsigned __int128) 1 << 127;

    high = (high - bias) << shift | signbit;

    return *(double*)&high;
}
