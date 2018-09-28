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
#include <math.h>

float _Complex ctanhf(float _Complex z)
{
    float x = z;
    float y = cimagf(z);

    double t = _expm1f(2 * fabsf(x));
    double sinhcosh = copysign(t, x) * (0.25 + 0.25 / (t + 1));
    double sinhsinh = 0.25 * t * t / (t + 1);

    double _Complex circular = _cisf(y);
    double cos = circular;
    double sin = cimag(circular);

    if (x != x && y == 0)
        return z;

    if (y - y)
        return isinf(x) ? copysignf(1, x) : CMPLXF(y - y, y - y);

    if (t >= 0x1p53)
        return CMPLXF(copysignf(1, x), sin * cos / (0.25 * t));

    return CMPLX(sinhcosh, sin * cos) / (sinhsinh + cos * cos);
}
