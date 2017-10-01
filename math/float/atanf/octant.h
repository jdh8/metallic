/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

static double atanf_taylor(double x)
{
    const double c3 = 0.333333333333333333;
    const double c5 = 0.2;
    const double c7 = 0.142857142857142857;
    const double c9 = 0.111111111111111111;

    double xx = x * x;

    return x - x * xx * (c3 - xx * (c5 - xx * (c7 - xx * c9)));
}

static double atanf_octant(double x)
{
    const double sqrt3 = 1.7320508075688772935;
    const double pi_6 = 0.52359877559829887308;

    if (fabs(x) > 2 - sqrt3)
        return pi_6 + atanf_taylor((sqrt3 * x - 1) / (x + sqrt3));
    else
        return atanf_taylor(x);
}
