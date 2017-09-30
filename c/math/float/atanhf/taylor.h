/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static float atanhf_taylor(float x)
{
    const float c3 = 0.333333333333;
    const float c5 = 0.2;
    const float c7 = 0.142857142857;
    const float c9 = 0.111111111111;

    float xx = x * x;

    return x + x * xx * (c3 + xx * (c5 + xx * (c7 + xx * c9)));
}
