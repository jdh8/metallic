/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/clogf.h"
#include "csqrt.h"
#include <complex.h>
#include <math.h>

static double _Complex _isinacos(float _Complex z)
{
    double x = z;
    double y = cimagf(z);
    double re = 1 - (x + y) * (x - y);
    double im = -2 * x * y;
    double _Complex sin = _csqrt(re, im);

    return CMPLX(-cimag(sin), sin);
}

float _Complex cacosf(float _Complex z)
{
    const double pi = 3.14159265358979323846;

    float x = z;
    float y = cimagf(z);

    if (isinf(y))
        return CMPLXF(pi / 2 - (isinf(x) ? copysignf(pi / 4, x) : 0 * x), -y);

    if (isinf(x))
        return CMPLXF(x == x ? signbit(x) * pi : x, copysignf(x, -y));

    float _Complex iw = _clogf(z + _isinacos(z));

    return CMPLXF(cimag(iw), -(float)iw);
}
