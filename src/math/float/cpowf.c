/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atan2f.h"
#include "cisf.h"
#include "expf.h"
#include "finite/loghypotf.h"
#include <complex.h>
#include <math.h>

static double _real(float x, float y)
{
    if (isinf(x) || isinf(y))
        return INFINITY;

    return _loghypotf(x, y);
}

float _Complex cpowf(float _Complex base, float _Complex exp)
{
    float x = base;
    float y = cimagf(base);

    double _Complex z = CMPLX(_real(x, y), _atan2f(y, x)) * exp;

    return _expf(z) * _cisf(cimag(z));
}
