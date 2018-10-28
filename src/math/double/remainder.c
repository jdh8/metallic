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

double remainder(double numerator, double denominator)
{
    uint64_t bits = reinterpret(uint64_t, numerator) & 0x8000000000000000;

    numerator = fmod(fabs(numerator), 2 * denominator);
    denominator = fabs(denominator);

    if (denominator < 2 * DBL_MIN) {
        if (2 * numerator > denominator) {
            numerator -= denominator;
            if (2 * numerator >= denominator)
                numerator -= denominator;
        }
    }
    else if (numerator > 0.5 * denominator) {
        numerator -= denominator;
        if (numerator >= 0.5 * denominator)
            numerator -= denominator;
    }

    bits ^= reinterpret(uint64_t, numerator);

    return reinterpret(double, bits);
}
