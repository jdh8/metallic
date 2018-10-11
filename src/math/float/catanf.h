/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "finite/log1pf.h"
#include "atan2f.h"
#include <complex.h>

static float _Complex _finite_catanf(double x, double y)
{
    double xx = x * x;
    double yy = y * y;
    double t = y - 1;
    float a = _atan2f(2 * x, 1 - xx - yy);
    float b = _log1pf(4 * y / (xx + t * t));

    return CMPLXF(0.5f * a, 0.25f * b);
}

static float _Complex _catanf(float x, float y)
{
    const float pi_2 = 1.57079632679489661923;

    if (x == INFINITY)
        return pi_2;

    if (y == INFINITY)
        return x == x ? pi_2 : x;

    if (x == 0 && y == 1)
        return CMPLXF(0, INFINITY);

    return _finite_catanf(x, y);
}
