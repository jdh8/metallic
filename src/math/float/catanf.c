/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/atan2f.h"
#include "prec/log1pf.h"
#include <complex.h>
#include <math.h>

float _Complex catanf(float _Complex z)
{
    const float pi_2 = 1.57079632679489661923;
    float x = z;
    float y = cimagf(z);

    if (isinf(x))
        return CMPLXF(copysignf(pi_2, x), copysignf(0, y));

    if (isinf(y))
        return CMPLXF(x == x ? copysignf(pi_2, x) : x, copysignf(0, y));

    double xx = x * (double)x;
    double yy = y * (double)y;

    double denominator = y - 1.0;
    denominator = xx + denominator * denominator;

    x = 0.5 * _atan2f(2.0 * x, 1 - xx - yy);
    y = 0.25 * _log1pf(4.0 * y / denominator);

    return CMPLXF(x, y);
}
