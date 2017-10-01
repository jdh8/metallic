/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "cosf/octant.h"
#include "sinf/octant.h"
#include <math.h>
#include <stdint.h>

float sinf(float x)
{
    const float pi_2 = 1.570796326795;
    float q = nearbyintf(x / pi_2);
    float r = x - pi_2 * q;

    uint32_t i = *(uint32_t*)&q;
    int shift = 150 - (i << 1 >> 24);

    if (shift >= 0 && i & 1 << shift)
        return cosf_octant(r);
    else
        return sinf_octant(r);
}
