/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "cacoshf.h"

float _Complex cacosf(float _Complex z)
{
    const double pi = 3.14159265358979323846;

    float x = z;
    float y = cimagf(z);

    double _Complex first = _cacoshf(fabsf(x), fabsf(y));

    return CMPLXF(fabs(cimag(first) - (x < 0) * pi), copysignf(first, -y));
}
