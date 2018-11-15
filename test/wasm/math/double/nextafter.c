
/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "../../assert.h"
#include "identical.h"
#include <math.h>

static _Bool run(double from, double to)
{
    double x = from ? from : copysignf(from, to);
    double y = nextafter(from, to);
    _Bool near = reinterpret(uint64_t, x) - reinterpret(uint64_t, y) + 1 <= 2;

    if (from == to)
        return identical(to, y);

    if (from < to)
        return from < y && near;

    if (from > to)
        return from > y && near;

    return isnan(y);
}

int main(void)
{
    for (uint64_t j = 0; j < 0x8000000000000000; j += 0x0003F46E86FBE2AA)
        for (uint64_t i = 0; i < 0x8000000000000000; i += 0x00033158A6F1D0C4)
            _assert(run(reinterpret(double, i), reinterpret(double, j)));

    for (uint64_t i = 0; i < 0x8000000000000000; i += 0x00033158A6F1D0C4)
        _assert(run(reinterpret(double, i), INFINITY));

    for (uint64_t j = 0; j < 0x8000000000000000; j += 0x0003F46E86FBE2AA)
        _assert(run(INFINITY, reinterpret(double, j)));
}
