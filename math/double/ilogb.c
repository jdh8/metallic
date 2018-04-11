/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <math.h>
#include <limits.h>
#include <stdint.h>

int ilogb(double x)
{
    int64_t word = reinterpret(int64_t, fabs(x));

    if (word == 0)
        return FP_ILOGB0;

    if (word < 0x0010000000000000)
        return -(1011 + __builtin_clzll(word));

    if (word < 0x7FF0000000000000)
        return (word >> 52) - 1023;

    if (FP_ILOGBNAN != INT_MAX && word == 0x7FF0000000000000)
        return INT_MAX;

    return FP_ILOGBNAN;
}
