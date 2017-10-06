/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static double kernel_expf(double a)
{
    const double b[] = { 120, 60, 12, 1 };

    double aa = a * a;
    double u = a * (b[3] * aa + b[1]);
    double v = b[2] * aa + b[0];

    return (v + u) / (v - u);
}
