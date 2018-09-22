/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../reinterpret.h"
#include <math.h>
#include <limits.h>
#include <stdint.h>

int ilogbf(float x)
{
    int32_t magnitude = reinterpret(int32_t, x) & 0x7FFFFFFF;

    if (magnitude == 0)
        return FP_ILOGB0;

    if (magnitude < 0x00800000)
        return -(118 + __builtin_clz(magnitude));

    if (magnitude < 0x7F800000)
        return (magnitude >> 23) - 127;

    if (FP_ILOGBNAN != INT_MAX && magnitude == 0x7F800000)
        return INT_MAX;

    return FP_ILOGBNAN;
}
