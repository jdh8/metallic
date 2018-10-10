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
#include "finite/log1pf.h"
#include <complex.h>
#include <math.h>

static double _Complex _coshasinh(double x, double y)
{
    double re = (x + y) * (x - y) + 1;
    double im = 2 * x * y;

    return _csqrt(re, im);
}

static float _Complex _first(double x, double y)
{
    if (x < 0.5) {
        double xx = x * x;

        if (y < 1) {
            double yy = y * y;
            double s = xx * (2 + xx + 2 * yy);
            double t = 1 - yy;
            double u = sqrt(s + t * t) + t;
            double dx = sqrt(0.5 * (u + xx));
            double dy = x * y / dx;
            double re = 0.5 * _log1pf(xx + s / u + 2 * (x * dx + y * dy));
            double im = _atan2f(y + dy, x + dx);

            return CMPLXF(re, im);
        }
        if (y == 1) {
            double r = x * sqrt(4 + xx);
            double dx = sqrt(0.5 * (r + xx));
            double dy = sqrt(0.5 * (r - xx));
            double re = 0.5 * _log1pf(xx + r + 2 * (x * dx + dy));
            double im = _atan2f(1 + dy, x + dx);

            return CMPLXF(re, im);
        }
    }

    return _clogf(CMPLXF(x, y) + _coshasinh(x, y));
}

float _Complex casinhf(float _Complex z)
{
    const double pi = 3.14159265358979323846;

    float x = z;
    float y = cimagf(z);

    if (isinf(x))
        return CMPLXF(x, isinf(y) ? copysignf(pi / 4, y) : 0 * y);

    if (isinf(y))
        return CMPLXF(copysignf(y, x), x == x ? copysignf(pi / 2, y) : x);

    float _Complex first = _first(fabsf(x), fabsf(y));

    return CMPLXF(copysignf(first, x), copysignf(cimagf(first), y));
}
