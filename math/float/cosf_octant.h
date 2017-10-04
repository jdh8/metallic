/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double cosf_octant(double x)
{
    const double c2 = 5.0000000000000000000e-1; /* 1 / 2! */
    const double c4 = 4.1666666666666666667e-2; /* 1 / 4! */
    const double c6 = 1.3888888888888888889e-3; /* 1 / 6! */
    const double c8 = 2.4801587301587301587e-5; /* 1 / 8! */

    x *= x;

    return 1 - x * (c2 - x * (c4 - x * (c6 - x * c8)));
}
