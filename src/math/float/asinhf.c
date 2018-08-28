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

static double _finite(double s)
{
    const double ln2 = 0.6931471805599453094;

    double c = sqrt(s * s + 1);
    int64_t i = reinterpret(int64_t, c + s);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> (DBL_MANT_DIG - 1);
    double y = reinterpret(double, i - (exponent << (DBL_MANT_DIG - 1)));

    if (exponent)
        return 2 * __kernel_atanhf((y - 1) / (y + 1)) + exponent * ln2;
    else
        return 2 * __kernel_atanhf(s / (c + 1));
}

static float _asinhf(float x)
{
    if (x - x == 0)
        return _finite(x);

    return x;
}

#ifdef _METALLIC
float asinhf(float x) { return _asinhf(x); }
#endif
