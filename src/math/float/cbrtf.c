/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "normalizef.h"
#include "../reinterpret.h"
#include <math.h>
#include <stdint.h>

static float _kernel(float x)
{
    int32_t i = reinterpret(int32_t, x);

    if (i == 0 || i >= 0x7F800000)
        return x;

    i = 0x2A512CE3 + __normalizef(i) / 3;

    double y = reinterpret(float, i);

    y *= 0.5 + 1.5 * x / (2 * y * (y * y) + x);
    y += 0.33333333333333333333 * (x / (y * y) - y);

    return y;
}

float cbrtf(float x)
{
    return copysignf(_kernel(fabsf(x)), x);
}
