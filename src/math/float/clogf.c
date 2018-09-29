/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atan2f.h"
#include "kernel/atanhf.h"
#include "../reinterpret.h"
#include <complex.h>
#include <math.h>
#include <stdint.h>

static float _kernel(double x)
{
    return _kernel_atanhf(x / (2 + x));
}

static float _first(float x, float y)
{
    const double ln2 = 0.69314718055994530942;

    if (x == INFINITY || y == INFINITY)
        return INFINITY;

    double a = x;
    double b = y;
    int64_t i = reinterpret(int64_t, a * a + b * b);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    if (!i)
        return -INFINITY;

    if (i > 0x7FF0000000000000)
        return NAN;

    if (!exponent) {
        if (x == 1)
            return _kernel(b * b);
        if (y == 1)
            return _kernel(a * a);
    }

    double c = reinterpret(double, i - (exponent << 52));

    return _kernel_atanhf((c - 1) / (c + 1)) + ln2 / 2 * exponent;
}

float _Complex clogf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    return CMPLXF(_first(fabsf(x), fabsf(y)), _atan2f(y, x));
}
