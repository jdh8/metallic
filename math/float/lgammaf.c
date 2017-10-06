/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "gamma.h"
#include "logf.h"
#include "sinpif.h"
#include <math.h>

static double _lnproduct(float z)
{
    const double lnsqrt2pi = 0.918938533204672742;
    const int g = 7;

    double shifted = z - 0.5;
    double base = shifted + g;

    return lnsqrt2pi + shifted * finite_logf(base) - base;
}

float lgammaf(float z)
{
    const double pi = 3.14159265358979324;

    if (isinf(z))
        return HUGE_VALF;

    if (z < 0.5f)
        return finite_logf(pi / (sinpif(z) * gamma_sum(1 - z))) - _lnproduct(1 - z);

    return _lnproduct(z) + finite_logf(gamma_sum(z));
}
