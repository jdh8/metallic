/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "kernel/atanhf.h"
#include "prec/sinpif.h"
#include "../gamma.h"
#include "../reinterpret.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

static double _logf(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> (DBL_MANT_DIG - 1);

    x = reinterpret(double, i - (exponent << (DBL_MANT_DIG - 1)));

    return 2 * __kernel_atanhf((x - 1) / (x + 1)) + exponent * ln2;
}

static double _lnproduct(float z)
{
    const double lnsqrt2pi = 0.918938533204672742;
    const int g = 7;

    double shifted = z - 0.5;
    double base = shifted + g;

    return lnsqrt2pi + shifted * _logf(base) - base;
}

float lgammaf(float z)
{
    const double pi = 3.14159265358979324;

    if (z == INFINITY)
        return z;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return HUGE_VALF;

        return _logf(pi / fabs(__prec_sinpif(z) * __gamma_lanczos_sum(1 - z))) - _lnproduct(1 - z);
    }

    return _lnproduct(z) + _logf(__gamma_lanczos_sum(z));
}
