/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atan2f.h"
#include "prec/loghypotf.h"
#include <complex.h>
#include <math.h>

static float _real(float x, float y)
{
    if (isinf(x) || isinf(y))
        return HUGE_VALF;

    return _loghypotf(x, y);
}

float _Complex clogf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    return CMPLXF(_real(x, y), _atan2f(y, x));
}
