/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "atanf/octant.h"
#include <math.h>

static double asinfd(double x)
{
    double s = fabs(x);
    double c = sqrt(1 - s * s);

    return copysign(2 * atanf_octant(s / (1 + c)), x);
}

float f(float x)
{
    const double pi_2 = 1.5707963267948966192;

    return pi_2 - asinfd(x);
}
