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
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static double _series(double z)
{
    const double p[] = {
        2.50662827563479526904,
        225.525584619175212544,
        -268.295973841304927459,
        80.9030806934622512966,
        -5.00757863970517583837,
        0.0114684895434781459556
    };

    return p[5] / (z + 5) + p[4] / (z + 4)
        + (p[3] / (z + 3) + p[2] / (z + 2))
        + (p[1] / (z + 1) + p[0]);
}

static double _logf(double x)
{
    const double ln2 = 0.69314718055994530942;

    int64_t i = reinterpret(int64_t, x);
    int64_t exponent = (i - 0x3FE6A09E667F3BCD) >> 52;

    x = reinterpret(double, i - (exponent << 52));

    return 2 * _kernel_atanhf((x - 1) / (x + 1)) + exponent * ln2;
}

static double _lnproduct(double z)
{
    double base = 5.65 + z;

    return (0.5 + z) * _logf(base) - base;
}

float lgammaf(float z)
{
    const double pi = 3.14159265358979324;

    if (z == 0 || z == INFINITY)
        return INFINITY;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return INFINITY;
        return _logf(pi / (fabs(_sinpif(z)) * _series(-z))) - _lnproduct(-z);
    }

    return _lnproduct(z - 1.0) + _logf(_series(z - 1.0));
}
