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

long double __floatunsitf(uint32_t a)
{
    double x = a;
    uint64_t source = *(uint64_t*)&x;

    const int shift = (128 - LDBL_MANT_DIG) - (64 - DBL_MANT_DIG);
    uint64_t summand = (0x38ULL << 56) * !!a;
    uint64_t high = (source >> shift) + summand;

    unsigned __int128 representation = (unsigned __int128) high << 64;

    return *(long double*)&representation;
}
