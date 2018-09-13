/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/clogf.h"
#include "csqrt.h"
#include <complex.h>
#include <math.h>

static const double pi = 3.14159265358979323846;

static double _Complex _sinh_acosh(float _Complex z)
{
    double x = z;
    double y = cimagf(z);
    double re = (x + y) * (x - y) - 1;
    double im = 2 * x * y;
    double _Complex right = __csqrt(re, im);

    return signbit((float)z) ? -right : right;
}

static float _resolve(float y)
{
    if (y - y == 0) // isfinite(y)
        return pi;

    if (y == y) // isinf(y)
        return 0.75 * pi;

    return y;
}

float _Complex cacoshf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    if (x == INFINITY)
        return CMPLXF(HUGE_VALF, isinf(y) ? copysignf(0.25 * pi, y) : 0 * y);

    if (x == -INFINITY)
        return CMPLXF(HUGE_VALF, copysignf(_resolve(y), y));

    if (isinf(y))
        return CMPLXF(HUGE_VALF, x == x ? copysignf(0.5 * pi, x) : x);

    return __prec_clogf(z + _sinh_acosh(z));
}
