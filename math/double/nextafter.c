/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include <math.h>
#include <stdint.h>

double nextafter(double from, double to)
{
    const int64_t mini = 1;

    if (from == to || to != to)
        return to;

    if (from != from)
        return from;

    if (from == 0)
        return copysign(*(double*)&mini, to);

    int64_t d = *(int64_t*)&from;
    int64_t a = *(int64_t*)&to;

    a < d || (a ^ d) < 0 ? --d : ++d;

    return *(double*)&d;
}
