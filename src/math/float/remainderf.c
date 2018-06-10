/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017, 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <math.h>
#include <float.h>
#include <stdint.h>

static float _quadrant(float numerator, float denominator)
{
    numerator = fmodf(numerator, 2 * denominator);

    if (denominator < 2 * FLT_MIN) {
        if (2 * numerator > denominator) {
            numerator -= denominator;
            if (2 * numerator >= denominator)
                numerator -= denominator;
        }
    }
    else if (numerator > 0.5f * denominator) {
        numerator -= denominator;
        if (numerator >= 0.5f * denominator)
            numerator -= denominator;
    }

    return numerator;
}

float remainderf(float numerator, float denominator)
{
    float r = _quadrant(fabsf(numerator), fabsf(denominator));
    uint32_t i = reinterpret(uint32_t, r);
    uint32_t sign = reinterpret(uint32_t, numerator) & 0x80000000;

    return reinterpret(float, i ^ sign);
}
