/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/atanhf.h"
#include "finite/sinpif.h"
#include "../gamma.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _logf(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    x = reinterpret(double, i - (exponent << 52));

    return 2 * _kernel_atanhf((x - 1) / (x + 1)) + exponent * ln2;
}

static double _lnproduct(float z)
{
    const double lnsqrt2pi = 0.918938533204672742;
    const int g = 7;

    double base = g + 0.5 + z;

    return lnsqrt2pi + (0.5 + z) * _logf(base) - base;
}

float lgammaf(float z)
{
    const double pi = 3.14159265358979324;

    if (z == 0 || z == INFINITY)
        return INFINITY;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return INFINITY;
        return _logf(pi / fabs(_sinpif(z) * _gamma_lanczos_sum(-z))) - _lnproduct(-z);
    }

    return _lnproduct(z - 1) + _logf(_gamma_lanczos_sum(z - 1));
}
