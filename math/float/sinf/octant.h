/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
static float sinf_octant(float x)
{
    const float c[] = {
        1.00000000e+0f,
        3.33333333e-1f, /* 1 / 3! */
        8.33333333e-3f, /* 1 / 5! */
        1.98412698e-4f, /* 1 / 7! */
        2.75573192e-6f  /* 1 / 9! */
    };

    float xx = x * x;

    return x * (c[0] - xx * (c[1] - xx * (c[2] - xx * (c[3] - xx * (c[4])))));
}
