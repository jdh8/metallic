/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "expm1f.h"
#include <math.h>

float sinhf(float x)
{
    float r = fabsf(x);
    double y = _expm1f(r);

    return copysignf(y * (0.5 + 0.5 / (y + 1)), x);
}
