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
#include <complex.h>
#include <math.h>
#include <float.h>
#include <stdint.h>

static double _finite(double x, double y)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = __bits(x * x + y * y);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> (DBL_MANT_DIG - 1);
    double z = __reinterpret(i - (exponent << (DBL_MANT_DIG - 1)));

    return __kernel_atanhf((z - 1) / (z + 1)) + ln2 / 2 * exponent;
}

static float _real(float x, float y)
{
    if (isinf(x) || isinf(y))
        return HUGE_VALF;

    return _finite(x, y);
}

float _Complex clogf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    return CMPLXF(_real(x, y), atan2f(y, x));
}
