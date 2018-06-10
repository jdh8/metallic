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

float remainderf(float numerator, float denominator)
{
    uint32_t bits = reinterpret(uint32_t, numerator) & 0x80000000;

    numerator = fmodf(fabsf(numerator), 2 * denominator);
    denominator = fabsf(denominator);

    if (denominator < 2 * FLT_MIN) {
        if (2 * numerator > denominator) {
            numerator -= denominator;
            if (2 * numerator >= denominator)
                numerator -= denominator;
        }
    }
    else {
        float threshold = 0.5f * denominator;
        if (numerator > threshold) {
            numerator -= denominator;
            if (numerator >= threshold)
                numerator -= denominator;
        }
    }

    bits ^= reinterpret(uint32_t, numerator);

    return reinterpret(float, bits);
}
