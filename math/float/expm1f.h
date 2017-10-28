/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double kernel_expm1f(double x)
{
    const double c[] = {
        2.000000003560133861,
        1.666661326163883623e-1,
       -2.765904870177422290e-3
    };

    double xx = x * x;
    double xcothx_2 = c[0] + c[1] * xx + c[2] * (xx * xx);

    return 2 * x / (xcothx_2 - x);
}
