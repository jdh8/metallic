/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanhf/taylor.h"
#include "quietf.h"
#include <math.h>

static float _normal(float x)
{
    const double ln2 = 0.6931471805599452862;
    const int32_t mantissa = 0x007FFFFF;
    const int32_t sqrt2 = 0x3FB504F3;

    float y = x + 1;
    int32_t word = *(int32_t*)&y;
    int exponent = (word >> 23) - 127;

    word = (word & mantissa) | 0x3F800000;

    if (word >= sqrt2) {
        word &= 0xFF7FFFFF;
        ++exponent;
    }

    y = *(float*)&word;
    
    if (exponent)
        return 2 * atanhf_taylor((y - 1) / (y + 1)) + exponent * ln2;
    else
        return 2 * atanhf_taylor(x / (2 + x));
}

float log1pf(float x)
{
    const uint32_t n1 = 0xBF800000;
    const int32_t inf = 0x7F800000;
    const int32_t eps = 0x34000000;

    int32_t i = *(int32_t*)&x;
    uint32_t j = *(uint32_t*)&x;

    if (j == n1)
        return -HUGE_VALF;

    if (j > n1)
        return quietf(x);

    if (i >= inf || j << 1 < eps << 1)
        return x;

    return _normal(x);
}
