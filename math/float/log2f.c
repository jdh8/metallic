/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include "kernel/atanhf.h"
#include "normalizef.h"
#include "quietf.h"
#include <float.h>
#include <math.h>

static float _finite(float x)
{
    const double log2e = 1.44269504088896340736;
    const int32_t sqrt1_2 = 0x3F3504F4;

    int32_t magnitude = __normalizef(__bitsf(x));
    int32_t exponent = (magnitude - sqrt1_2) >> (FLT_MANT_DIG - 1);
    double y = __reinterpretf(magnitude - (exponent << (FLT_MANT_DIG - 1)));

    return 2 * log2e * __kernel_atanhf((y - 1) / (y + 1)) + exponent;
}

float f(float x)
{
    const int32_t inf = 0x7F800000;

    if (x == 0)
        return -HUGE_VALF;

    int32_t i = *(int32_t*)&x;

    if (i < 0)
        return __quietf(x);

    if (i < inf)
        return _finite(x);

    return x;
}

float test(void)
{
    return f(8);
}
