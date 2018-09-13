/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <complex.h>
#include <math.h>

static double _Complex _csqrt(double x, double y)
{
    double s = sqrt(0.5 * (fabs(x) + sqrt(x * x + y * y)));
    double t = 0.5 * y / s;

    if (signbit(x))
        return CMPLX(fabs(t), copysign(s, y));
    else
        return CMPLX(s, t);
}
