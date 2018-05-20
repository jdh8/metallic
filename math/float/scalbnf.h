/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "normalizef.h"
#include "../reinterpret.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

float SCALBNF(float x, Integer exp)
{
    int32_t i = reinterpret(int32_t, fabsf(x));

    if (i == 0 || i >= 0x7F800000)
        return x;

    i = __normalizef(i);

    Integer biased = exp + (i >> (FLT_MANT_DIG - 1));

    if (biased >= 0xFF || biased < -FLT_MANT_DIG)
        return x * (exp < 0 ? 0 : HUGE_VALF);

    i &= 0x007FFFFF;

    if (biased > 0)
        i |= biased << (FLT_MANT_DIG - 1);
    else
        i = (i | 0x00800000) >> (1 - biased);

    return copysignf(reinterpret(float, i), x);
}
