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

static double _remquo(double numerator, double denominator, int quotient[static 1])
{
    int64_t from = reinterpret(int64_t, numerator);
    int q = 0;

    numerator = fmod(fabs(numerator), 8 * denominator);
    denominator = fabs(denominator);

    if (numerator >= 4 * denominator) {
        numerator -= 4 * denominator;
        q |= 4;
    }

    if (numerator >= 2 * denominator) {
        numerator -= 2 * denominator;
        q |= 2;
    }

    if (denominator < 2 * DBL_MIN) {
        if (2 * numerator > denominator) {
            numerator -= denominator;
            ++q;

            if (2 * numerator >= denominator) {
                numerator -= denominator;
                ++q;
            }
        }
    }
    else if (numerator > 0.5 * denominator) {
        numerator -= denominator;
        ++q;

        if (numerator >= 0.5 * denominator) {
            numerator -= denominator;
            ++q;
        }
    }

    *quotient = from < 0 ? -q : q;

    uint64_t to = (from & 0x8000000000000000) ^ reinterpret(uint64_t, numerator);

    return reinterpret(double, to);
}
