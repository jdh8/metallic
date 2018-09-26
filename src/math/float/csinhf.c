/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "cisf.h"
#include "expm1f.h"
#include <complex.h>

float _Complex csinhf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    double t = _expm1f(fabsf(x));
    double cosh = 0.5 * (t + 1) + 0.5 / (t + 1);
    double sinh = copysign(t * (0.5 + 0.5 / (t + 1)), x);
    double _Complex circular = _cisf(y);

    if (y == 0)
        return CMPLXF(sinh, y);

    if (y - y && (x == 0 || isinf(x)))
        return CMPLXF(x, y - y);

    return CMPLXF(sinh * creal(circular), cosh * cimag(circular));
}
