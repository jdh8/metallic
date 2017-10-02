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

float cosf(float x)
{
    const double pi_2 = 1.57079632679489662;
    const float _2_pi = 0.6366197723676;
    const uint32_t thresh = 0x4F000000; /* 2 ** 31 */

    float q = nearbyintf(x * _2_pi);
    double r = x - pi_2 * q;
    uint32_t i = *(uint32_t*)&q;

    if (i << 1 < thresh << 1) {
        switch (3 & (unsigned)(int_least32_t) q) {
            case 1:
                return sinf_octant(-r);
            case 2:
                return -cosf_octant(r);
            case 3:
                return sinf_octant(r);
        }
    }

    return cosf_octant(r);
}
