/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "prec/clogf.h"
#include "../csqrt.h"
#include <complex.h>
#include <math.h>

static double _Complex _cosh_asinh(float _Complex z)
{
    double x = z;
    double y = cimagf(z);
    double re = (x + y) * (x - y) + 1;
    double im = 2 * x * y;

    return __csqrt(re, im);
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

    return __prec_clogf(z + _cosh_asinh(z));
}

#if defined(__OPTIMIZE__) && !defined(__OPTIMIZE_SIZE__)
__attribute__((__flatten__))
#endif
float _Complex casinf(float _Complex z)
{
    z = casinhf(CMPLXF(cimagf(z), z));

    return CMPLXF(cimagf(z), z);
}
