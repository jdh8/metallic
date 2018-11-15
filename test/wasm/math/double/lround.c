/* This file is part of Metallic, a runtime library for WebAssembly.
 *
 * Copyright (C) 2018 Chen-Pang He <chen.pang.he@jdh8.org>
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at http://mozilla.org/MPL/2.0/
 */
#include "src/math/reinterpret.h"
#include "../../assert.h"
#include <math.h>
#include <limits.h>
#include <stdint.h>

int main(void)
{
    const uint64_t max = 0x5000000000000000;
    const uint64_t step = 0x00000007BD058423;

    for (uint64_t i = 0; i < max; i += step) {
        double x = reinterpret(double, i);
        double y = round(x);

        if (y > LONG_MAX)
            break;

        _assert(y == lround(x));
        _assert(-y == lround(-x));
    }
}
