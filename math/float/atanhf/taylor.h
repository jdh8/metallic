/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double atanhf_taylor(double x)
{
    const double c3 = 0.333333333333333333;
    const double c5 = 0.2;
    const double c7 = 0.142857142857142857;

    double xx = x * x;

    return x + x * xx * (c3 + xx * (c5 + xx * c7));
}
