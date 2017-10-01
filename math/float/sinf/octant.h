/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double sinf_octant(double x)
{
    const double c3 = 1.6666666666666666667e-1; /* 1 / 3! */
    const double c5 = 8.3333333333333333333e-3; /* 1 / 5! */
    const double c7 = 1.9841269841269841270e-4; /* 1 / 7! */
    const double c9 = 2.7557319223985890653e-6; /* 1 / 9! */

    double xx = x * x;

    return x - x * xx * (c3 - xx * (c5 - xx * (c7 - xx * c9)));
}
