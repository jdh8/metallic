/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/exp2f.h"
#include "prec/log2f.h"
#include "prec/sinpif.h"
#include "../gamma.h"
#include <math.h>

static double _product(float z)
{
    const double log2e = 1.442695040888963407;
    const double sqrt2pi = 2.50662827463100050;
    const int g = 7;

    double shifted = z - 0.5;
    double base = shifted + g;

    return sqrt2pi * __prec_exp2f(shifted * __prec_log2f(base) - log2e * base);
}

static double _right(float z)
{
    const float max = 35.0400984199;

    if (z > max)
        return HUGE_VALF;

    return _product(z) * __gamma_lanczos_sum(z);
}

float tgammaf(float z)
{
    const double pi = 3.14159265358979324;

    if (z == 0)
        return 1 / z;

    if (z < 0.5f) {
        if (rintf(z) == z)
            return (z - z) / (z - z);

        return pi / (__prec_sinpif(z) * _right(1 - z));
    }

    return _right(z);
}
