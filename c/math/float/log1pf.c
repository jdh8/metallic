/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "log1pf.h"
#include "quietf.h"
#include <math.h>

float log1pf(float x)
{
    const double ln2 = 0.6931471805599452862;
    const int64_t sqrt2 = 0x3FF6A09E667F3BCD;
    const uint32_t n1 = 0xBF800000;
    const int32_t inf = 0x7F800000;

    int32_t i = *(int32_t*)&x;
    uint32_t j = *(uint32_t*)&x;

    if (j == n1)
        return -HUGE_VALF;

    if (j > n1)
        return quietf(x);

    if (x == 0 || i >= inf)
        return x;

    double y = x + 1;
    int64_t word = *(int64_t*)&y;
    int exponent = (word >> 52) - 1023;

    word = (word & 0x000FFFFFFFFFFFFF) | 0x3FF0000000000000;

    if (word >= sqrt2) {
        word &= 0xFFEFFFFFFFFFFFFF;
        ++exponent;
    }

    if (exponent)
        return __log1pf(*(double*)&word - 1) + exponent * ln2;
    else
        return __log1pf(x);
}
