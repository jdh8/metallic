/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "expm1f.h"
#include <math.h>

static float _right(float x)
{
    if (x >= 256)
        return 1;

    double y = _expm1f(2 * x);

    return y / (y + 2);
}

float tanhf(float x)
{
    return copysignf(_right(fabsf(x)), x);
}
