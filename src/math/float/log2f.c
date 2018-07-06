/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "normalizef.h"
#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <float.h>
#include <math.h>

static float _finite(int32_t i)
{
    const double log2e = 1.44269504088896340736;

    int32_t exponent = (i - 0x3F3504F4) >> (FLT_MANT_DIG - 1);
    double x = reinterpret(float, i - (exponent << (FLT_MANT_DIG - 1)));

    return 2 * log2e * __kernel_atanhf((x - 1) / (x + 1)) + exponent;
}

static float _log2f(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i <= 0)
        return i << 1 == 0 ? -HUGE_VALF : NAN;

    if (i < 0x7F800000)
        return _finite(__normalizef(i));

    return x;
}

#ifdef _METALLIC
float log2f(float x) { return _log2f(x); }
#endif
