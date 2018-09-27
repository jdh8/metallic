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

static float _Complex _ccoshf(float x, float y)
{
    double t = _expm1f(fabsf(x));
    double cosh = 0.5 * (t + 1) + 0.5 / (t + 1);
    double sinh = copysign(t * (0.5 + 0.5 / (t + 1)), x);
    double _Complex circular = _cisf(y);

    if (y == 0)
        return CMPLXF(cosh, y);

    if (y - y) {
        if (x == 0)
            return CMPLXF(y - y, x);

        if (isinf(x))
            return CMPLXF(INFINITY, y - y);
    }

    return CMPLXF(cosh * creal(circular), sinh * cimag(circular));
}
