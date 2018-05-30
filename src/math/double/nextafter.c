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

double nextafter(double from, double to)
{
    if (from == to || to != to)
        return to;

    if (from != from)
        return from;

    if (from == 0)
        return copysign(DBL_TRUE_MIN, to);

    int64_t a = reinterpret(int64_t, from);
    int64_t b = reinterpret(int64_t, to);

    return reinterpret(double, b < a || (a ^ b) < 0 ? a - 1 : a + 1);
}
