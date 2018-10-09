/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "finite/clogf.h"
#include "finite/csqrt.h"
#include <complex.h>
#include <math.h>

static double _Complex _cube(double x, double y)
{
    double xx = x * x;
    double yy = y * y;

    return CMPLX(x * (xx - 3 * yy), y * (3 * xx - yy));
}

static double _Complex _coshasinh(float x, float y)
{
    double re = (x + y) * (x - y) + 1;
    double im = 2 * x * y;

    return _csqrt(re, im);
}

static float _Complex _first(float x, float y)
{
    float _Complex z = CMPLXF(x, y);

    if (x < 0.01 || y < 0.01)
        return z - 1/6. * _cube(x, y);

    return _clogf(z + _coshasinh(x, y));
}

float _Complex casinhf(float _Complex z)
{
    const double pi = 3.14159265358979323846;

    float x = z;
    float y = cimagf(z);

    if (isinf(x))
        return CMPLXF(x, isinf(y) ? copysignf(pi / 4, y) : 0 * y);

    if (isinf(y))
        return CMPLXF(copysignf(y, x), x == x ? copysignf(pi / 2, x) : x);

    float _Complex first = _first(fabsf(x), fabsf(y));

    return CMPLXF(copysignf(first, x), copysignf(cimagf(first), y));
}
