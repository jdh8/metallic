/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static float cosf_octant(float x)
{
    const float c[] = {
        1.00000000e+0f,
        5.00000000e-1f, /* 1 / 2! */
        4.16666667e-2f, /* 1 / 4! */
        1.38888889e-3f, /* 1 / 6! */
        2.48015873e-5f  /* 1 / 8! */
    };

    float xx = x * x;

    return c[0] - xx * (c[1] - xx * (c[2] - xx * (c[3] - xx * (c[4]))));
}
