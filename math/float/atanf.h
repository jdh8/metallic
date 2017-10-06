/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>

static double pade_atanf(double x)
{
    double c[] = {
        14.765625,
        14.765625,
        16.40625,
        11.484375,
         3.515625,
    };

    double xx = x * x;

    return x * ((xx + c[3]) * xx + c[1]) / ((c[4] * xx + c[2]) * xx + c[0]);
}

static double octant_atanf(double x)
{
    const double sqrt3 = 1.7320508075688772935;
    const double pi_6 = 0.52359877559829887308;

    if (x > 2 - sqrt3)
        return pi_6 + pade_atanf((sqrt3 * x - 1) / (x + sqrt3));
    else
        return pade_atanf(x);
}
