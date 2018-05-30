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

float nextafterf(float from, float to)
{
    if (from == to || to != to)
        return to;

    if (from != from)
        return from;

    if (from == 0)
        return copysignf(FLT_TRUE_MIN, to);

    int32_t a = reinterpret(int32_t, from);
    int32_t b = reinterpret(int32_t, to);

    return reinterpret(float, b < a || (a ^ b) < 0 ? a - 1 : a + 1);
}
