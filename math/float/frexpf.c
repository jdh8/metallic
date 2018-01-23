/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
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

float frexpf(float x, int exp[static 1])
{
    int32_t i = reinterpret(int32_t, x) & 0x7FFFFFFF;

    if (i == 0 || i >= 0x7F800000) {
        *exp = 0;
        return x;
    }

    i = __normalizef(i);
    *exp = (i >> (FLT_MANT_DIG - 1)) - 127;

    return copysignf(reinterpret(float, (i & 0x007FFFFF) | 0x3F000000), x);
}
