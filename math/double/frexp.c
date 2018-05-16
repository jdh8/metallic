/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "normalize.h"
#include "../reinterpret.h"
#include <float.h>
#include <math.h>
#include <stdint.h>

double frexp(double x, int exp[static 1])
{
    int64_t i = reinterpret(int64_t, fabs(x));

    if (i == 0 || i >= 0x7FF0000000000000) {
        *exp = 0;
        return x;
    }

    i = __normalize(i);
    *exp = (i >> (DBL_MANT_DIG - 1)) - 1023;

    return copysign(reinterpret(double, (i & 0x000FFFFFFFFFFFFF) | 0x3FE0000000000000), x);
}
