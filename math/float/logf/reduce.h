/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <stdint.h>

static double logf_reduce(double x, int* exp)
{
    const int64_t mantissa = 0x000FFFFFFFFFFFFF;
    const int64_t sqrt2 = 0x3FF6A09E667F3BCD;

    int64_t word = *(int64_t*)&x;

    *exp = (word >> 52) - 1023;
    word = (word & mantissa) | 0x3FF0000000000000;

    if (word >= sqrt2) {
        word &= 0xFFEFFFFFFFFFFFFF;
        ++*exp;
    }

    return *(double*)&word;
}
