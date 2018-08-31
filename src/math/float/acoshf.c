/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

static double _finite(double c)
{
    const double ln2 = 0.6931471805599453094;

    double s = sqrt(c * c - 1);
    int64_t i = reinterpret(int64_t, c + s);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> (DBL_MANT_DIG - 1);
    double y = reinterpret(double, i - (exponent << (DBL_MANT_DIG - 1)));

    return 2 * __kernel_atanhf((y - 1) / (y + 1)) + exponent * ln2;
}

static float _acoshf(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i < 0x3F800000)
        return NAN;

    if (i < 0x7F800000)
        return _finite(x);

    return x;
}

#ifdef _METALLIC
float acoshf(float x) { return _acoshf(x); }
#endif
