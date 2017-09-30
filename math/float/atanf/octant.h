/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

static float atanf_taylor(float x)
{
    const float c3 = 0.333333333333;
    const float c5 = 0.2;
    const float c7 = 0.142857142857;
    const float c9 = 0.111111111111;

    float xx = x * x;

    return x - x * xx * (c3 - xx * (c5 - xx * (c7 - xx * c9)));
}

static float atanf_octant(float x)
{
    const float sqrt3 = 1.73205080757;
    const float pi_6 = 0.523598775598;

    if (fabsf(x) > 2 - sqrt3)
        return pi_6 + atanf_taylor((sqrt3 * x - 1) / (x + sqrt3));
    else
        return atanf_taylor(x);
}
